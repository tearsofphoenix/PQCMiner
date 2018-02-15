//
// Created by Isaac on 2018/2/14.
//

#include <iostream>
#include <iomanip>
#include "Block.h"
#include "Buffer.h"

std::string Block::bits2Target(uint32_t bits) {
    int32_t exponent = bits >> 24;
    int32_t mantissa = bits & 0xFFFFFF;

    int32_t len = exponent;
    if (mantissa == 0xffff) {
        // need add zero to padd
        len -= 1;
    } else {

    }

    const size_t total = 32;
    Buffer target;
    target.resize(total, 0);
    target.writeInt32LE(total - len, mantissa);
    return target.toHex();
}