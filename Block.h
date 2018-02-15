//
// Created by Isaac on 2018/2/14.
//

#ifndef PQCMINER_BLOCK_H
#define PQCMINER_BLOCK_H

#include <string>
#include <utility>
#include <vector>
#include "types.h"

/**
 * like:
 * version: 1,
    previousblockhash: '0000000000000000000000000000000000000000000000000000000000000000',
    merkleroot: '4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b',
    time: 1518511836,
    bits: '1d00ffff'
 */
class Block {
public:
    int32_t version;
    std::string previousblockhash;
    std::string merkleroot;
    int32_t time;
    int32_t bits;
    Block(int32_t v, const std::string &p, const std::string &m, int32_t t, int32_t b)
            : version(v), previousblockhash(p), merkleroot(m), time(t), bits(b) {

    }
    ~Block() = default;

    /**
     * convert double value to hex string
     * @param bits {uint32_t}
     * @return
     */
    static std::string bits2Target(uint32_t bits);
};


#endif //PQCMINER_BLOCK_H
