//
// Created by Isaac on 2018/2/14.
//

#ifndef PQCMINER_PQCMINER_H
#define PQCMINER_PQCMINER_H

#include <iostream>
#include <cmath>
#include <vector>
#include "cube_hash.h"
#include "Block.h"
#include "Buffer.h"

extern std::vector<byte> PQCHash(const std::vector<byte> &buffer);

extern std::vector<byte> PQCMixHash(const std::vector<byte> &buffer);

extern std::vector<byte> BitcoinHash(const std::vector<byte> &buffer);

class PQCMiner {
private:
    const Buffer *_targetBuffer;
    Buffer *_versionBuffer;
    const Buffer *_reversedPrevBlockHash;
    const Buffer *_reversedMrklRoot;
    Buffer *_timeBitsNonceBuffer;
    BufferHashFunc _hashfunc;
    uint64_t _nonce;
    uint64_t _maxNonce;
public:
    static const uint64_t MaxNonce = 0xFFFFFFFF;

    PQCMiner(const Block &block);
    ~PQCMiner() = default;

    static float difficulty(unsigned int bits);

    const Buffer getHash(uint64_t nonce);

    inline void setHashFunc(BufferHashFunc f) {
        _hashfunc = f;
    }
    inline BufferHashFunc getHashFunc() {
        return _hashfunc;
    }
    inline void setNonce(uint64_t nonce) {
        _nonce = nonce;
    }
    inline uint64_t getNonce() {
        return _nonce;
    }

    inline void setMaxNonce(uint64_t maxNonce) {
        _maxNonce = maxNonce;
    }
    inline uint64_t getMaxNonce(void) {
        return _maxNonce;
    }

    bool verifyNonce(const Buffer &block, int32_t checknonce);

    /**
     *
     * @param hash {Buffer}
     * @return {boolean}
     */
    bool checkHash(const Buffer &hash);

    /**
     * start mine
     */
    void run();

    /**
     * alias of run()
     */
    void operator() (void);

    static void runMultiThread(const Block &block, uint64_t nonce, BufferHashFunc func = PQCHash);

    static void runMultiThread(const Block &block, const std::vector<uint64_t> &start, const std::vector<uint64_t> &end,
                               BufferHashFunc func = PQCHash);
};

extern float fast_log(float val);

#endif //PQCMINER_PQCMINER_H
