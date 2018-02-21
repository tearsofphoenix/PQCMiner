//
// Created by Isaac on 2018/2/14.
//
#include "PQCMiner.h"
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>
#if CRYPTOPP_DIR_PLUS
#  include <crypto++/sha.h>
#else
#  include <cryptopp/sha.h>
#endif

float fast_log(float val) {
    int *const exp_ptr = reinterpret_cast <int *>(&val);
    int x = *exp_ptr;
    const int log_2 = ((x >> 23) & 255) - 128;
    x &= ~(255 << 23);
    x += 127 << 23;
    *exp_ptr = x;

    val = ((-1.0f / 3) * val + 2) * val - 2.0f / 3;
    return ((val + log_2) * 0.69314718f);
}

float PQCMiner::difficulty(unsigned int bits) {
    static double max_body = fast_log(0x00ffff), scaland = fast_log(256);
    return exp(max_body - fast_log(bits & 0x00ffffff) + scaland * (0x1d - ((bits & 0xff000000) >> 24)));
}

const Buffer PQCMiner::getHash(uint64_t nonce)  {
    // Update nonce in header Buffer
    BufferUtil::writeInt32LE(*_timeBitsNonceBuffer, 8, nonce);
    Buffer buffer;
    BufferUtil::append(buffer, *_versionBuffer);
    BufferUtil::append(buffer, *_reversedPrevBlockHash);
    BufferUtil::append(buffer, *_reversedMrklRoot);
    BufferUtil::append(buffer, *_timeBitsNonceBuffer);

    // Double sha256 hash the header
    Buffer result = _hashfunc(buffer);
    BufferUtil::reverse(result);
    return result;
}

bool PQCMiner::verifyNonce(const Buffer &block, int32_t checknonce)  {
    // This is a (maybe easier) way to build the header from scratch, it should generate the same hash:
    std::cout << "[Verify Nonce " << checknonce << std::endl;
//    const version = reverseString(block.version.toString(16));
//    const prevhash = reverseString(block.previousblockhash);
//    const merkleroot = reverseString(block.merkleroot);
//    const nbits = reverseString(block.bits);
//    const ntime = reverseString(block.time.toString(16));
//    const nonce = reverseString(checknonce.toString(16));
//
//    const header = version + prevhash + merkleroot + ntime + nbits + nonce;
//    const hash = reverseString(_hash(Buffer.from(header, 'hex')));
//    console.log('Target: ', this.getTarget().toString('hex'));
//    console.log('Hash:   ', hash.toString('hex'));
//
//    const isvalid = this.getTarget().toString('hex') > hash;
//    const result = isvalid ? 'valid' : 'not a valid';
//    console.log('Result: ', `${checknonce} is a ${result} nonce`);
//    return isvalid;
}

bool PQCMiner::checkHash(const Buffer &hash) {
    return BufferUtil::compare(*_targetBuffer, hash) > 0;
}

static int64_t foundNonce;
static std::mutex nonceLock;

PQCMiner::PQCMiner(const Block &block) {

    // Initialize local variables with Block data
    Buffer *prevBlockHash = BufferUtil::from(block.previousblockhash);
    Buffer *mrklRoot = BufferUtil::from(block.merkleroot);
    const int32_t ver = block.version;
    const int32_t time = block.time;

    // Calculate target based on block's "bits",
    // The "bits" variable is a packed representation of the Difficulty in 8 bytes, to unpack it:
    // First two bytes make the "exponent", and the following 4 bytes make the "mantissa":
    // https://en.bitcoin.it/wiki/Difficulty#What_is_the_formula_for_difficulty
    const int32_t bits = block.bits;

    // FIXME
    std::string targetHex = Block::bits2Target(bits);
    std::cout << "target:" << targetHex << std::endl;

    _targetBuffer = BufferUtil::from(targetHex);

    // Create little-endian long int (4 bytes) with the version on the first byte
    _versionBuffer = new Buffer(4);
    BufferUtil::writeInt32LE(*_versionBuffer, 0, ver);

    // Reverse the previous Block Hash and the merkle_root
    BufferUtil::reverse(*prevBlockHash);
    _reversedPrevBlockHash = prevBlockHash;
    BufferUtil::reverse(*mrklRoot);
    _reversedMrklRoot = mrklRoot;

    // Buffer with time (4 Bytes), bits (4 Bytes) and nonce (4 Bytes) (later added and updated on each hash)

    _timeBitsNonceBuffer = new Buffer;
    _timeBitsNonceBuffer->resize(12, 0);
    BufferUtil::writeInt32LE(*_timeBitsNonceBuffer, 0, time);
    BufferUtil::writeInt32LE(*_timeBitsNonceBuffer, 4, bits);

    _hashfunc = PQCHash;
    _maxNonce = MaxNonce;
    foundNonce = 0;
}

void PQCMiner::run() {
    uint64_t nonce = _nonce;
    uint64_t maxNonce = _maxNonce;
    uint64_t total = maxNonce - nonce;
    std::string threadId = boost::lexical_cast<std::string>(boost::this_thread::get_id());
    std::cout << "thread:" << threadId << "range: [" << nonce << ":" << maxNonce << "]" << std::endl;

    while (nonce <= maxNonce && foundNonce == 0) {
        auto hash = getHash(nonce);
        if (nonce % 100000 == 0) {
            std::cout << nonce << "  " << (nonce - _nonce) * 100.0 / total << "%" << std::endl;
        }
        bool found = checkHash(hash);
        if (found) {
            nonceLock.lock();
            foundNonce = nonce;
            std::cout << "found: " << nonce << "  " << BufferUtil::toHex(hash) << std::endl;
            nonceLock.unlock();
            break;
        }
        nonce++;
    }
}

void PQCMiner::operator() (void) {
    run();
}

void PQCMiner::runMultiThread(const Block &block, uint64_t nonce, BufferHashFunc func) {
    unsigned int count = boost::thread::hardware_concurrency();
    uint64_t step = (MaxNonce - nonce) / count;

    std::vector<uint64_t> start;
    std::vector<uint64_t> end;

    for (int i = 0; i < count; ++i) {
        start.push_back(nonce + step * i);
        end.push_back(nonce + step * i + step);
    }

    runMultiThread(block, start, end, func);
}

void PQCMiner::runMultiThread(const Block &block, const std::vector<uint64_t> &start, const std::vector<uint64_t> &end,
                         BufferHashFunc func) {
    auto count = start.size();
    boost::thread *threads[count];

    for (auto i = 0; i < count; ++i) {
        PQCMiner miner(block);
        miner.setNonce(start[i]);
        miner.setHashFunc(func);
        miner.setMaxNonce(end[i]);
        threads[i] = new boost::thread(miner);
    }

    for (int j = 0; j < count; ++j) {
        threads[j]->join();
        delete threads[j];
        threads[j] = nullptr;
    }
}
//-----------
static cube256hash hash;
std::vector<byte> PQCHash(const std::vector<byte> &buffer) {
    return hash(hash(buffer));
}

static std::vector<byte> sha256(const std::vector<byte> &a) {
    size_t size = CryptoPP::SHA256::DIGESTSIZE;
    std::vector<byte> r;
    r.resize (size);
    CryptoPP::SHA256().CalculateDigest(& (r[0]),
                                       & (a[0]),
                                       a.size());
    return r;
}

std::vector<byte> BitcoinHash(const std::vector<byte> &buffer) {
    return sha256(sha256(buffer));
}


std::vector<byte> PQCMixHash(const std::vector<byte> &buffer) {
    return sha256(hash(buffer));
}
