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

class PQCMiner {
private:
    const Buffer *_targetBuffer;
    Buffer *_versionBuffer;
    const Buffer *_reversedPrevBlockHash;
    const Buffer *_reversedMrklRoot;
    Buffer *_timeBitsNonceBuffer;
    BufferHashFunc _hashfunc;
public:
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

    bool verifyNonce(const Buffer &block, int32_t checknonce);

    /**
     *
     * @param hash {Buffer}
     * @return {boolean}
     */
    bool checkHash(const Buffer &hash);

    void run(uint64_t nonce);
};

extern std::vector<byte> PQCHash(const std::vector<byte> &buffer);

extern std::vector<byte> BitcoinHash(const std::vector<byte> &buffer);

extern float fast_log(float val);

#endif //PQCMINER_PQCMINER_H
