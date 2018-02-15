//
// Created by Isaac on 2018/2/14.
//
#include "PQCMiner.h"
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
    _timeBitsNonceBuffer->writeInt32LE(8, nonce);
    Buffer buffer;
    buffer.append(*_versionBuffer);
    buffer.append(*_reversedPrevBlockHash);
    buffer.append(*_reversedMrklRoot);
    buffer.append(*_timeBitsNonceBuffer);

    // Double sha256 hash the header
    Buffer result = buffer.hash(_hashfunc);
    result.reverse();
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
    return _targetBuffer->compare(hash) > 0;
}

PQCMiner::PQCMiner(const Block &block) {

    // Initialize local variables with Block data
    Buffer *prevBlockHash = Buffer::from(block.previousblockhash);
    Buffer *mrklRoot = Buffer::from(block.merkleroot);
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

    _targetBuffer = Buffer::from(targetHex);

    // Create little-endian long int (4 bytes) with the version on the first byte
    _versionBuffer = new Buffer(4);
    _versionBuffer->writeInt32LE(0, ver);

    // Reverse the previous Block Hash and the merkle_root
    prevBlockHash->reverse();
    _reversedPrevBlockHash = prevBlockHash;
    mrklRoot->reverse();
    _reversedMrklRoot = mrklRoot;

    // Buffer with time (4 Bytes), bits (4 Bytes) and nonce (4 Bytes) (later added and updated on each hash)

    _timeBitsNonceBuffer = new Buffer;
    _timeBitsNonceBuffer->resize(12, 0);
    _timeBitsNonceBuffer->writeInt32LE(0, time);
    _timeBitsNonceBuffer->writeInt32LE(4, bits);

    _hashfunc = PQCHash;
}

void PQCMiner::run(uint64_t nonce) {
    bool found = false;
    while (nonce < 8561950000 && !found) {
        auto hash = getHash(nonce);
        if (nonce % 100000 == 0) {
            std::cout << nonce << "  " << hash.toHex() << std::endl;
        }
        found = checkHash(hash);
        if (found) {
            std::cout << nonce << hash.toHex() << std::endl;
            break;
        }
        nonce++;
    }
}

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
