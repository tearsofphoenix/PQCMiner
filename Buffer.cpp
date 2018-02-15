//
// Created by Isaac on 2018/2/15.
//

#include <iostream>
#include "Buffer.h"

static inline int char2int(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return -1;
}


void BufferUtil::append(Buffer &b1, const Buffer &b2) {
    b1.insert(b1.end(), b2.begin(), b2.end());
}

Buffer *BufferUtil::from(const std::string &str, Type type) {
    switch (type) {
        case Hex: {
            auto *buffer = new Buffer;

            for (int i = 0; i < str.length(); i = i + 2) {
                byte val = char2int(str[i]) * 16 + char2int(str[i + 1]);
                buffer->push_back(val);
            }
            return buffer;
        }
        case Base64: {

        }
        case UTF8: {
            auto *buffer = new Buffer;
            for (int i = 0; i < str.length(); ++i) {
                byte val = str[i];
                buffer->push_back(val);
            }
            return buffer;
        }
        default: {
            return nullptr;
        }
    }
}

/**
 *  write an int32 value at index into a Buffer
 * @param buffer {Buffer}
 * @param startIndex {size_t}
 * @param val {int32_t}
 */
void BufferUtil::writeInt32LE(Buffer &b, size_t startIndex, int32_t val) {
    b[startIndex + 3] = (byte)(val >> 24);
    b[startIndex + 2] = (byte)(val >> 16);
    b[startIndex + 1] = (byte)(val >> 8);
    b[startIndex] = (byte)(val);
}

/**
 *  convert buffer to hex string
 * @param buffer {Buffer}
 * @return {std::string}
 */
std::string BufferUtil::toHex(const Buffer &b) {
    return toString(b);
}

std::string BufferUtil::toString(const Buffer &b, Type type) {
    switch (type) {
        case Hex: {
            std::string hex;
            size_t size = b.size();
            hex.resize(size * 2);
            char temp[3] = {0};
            for (int i = 0; i < size; ++i) {
                sprintf(temp, "%02x", b[i]);
                hex[2 * i] = (temp[0]);
                hex[2 * i + 1] = (temp[1]);
            }
            return hex;
        }
        case Base64: {

        }
        case UTF8: {

        }
        default: {

        }
    }
}

/**
 * reverse a buffer
 * @param buffer {Buffer}
 */
void BufferUtil::reverse(Buffer &b) {
    auto start = b.begin();
    auto end = b.end();
    while (start != end) {
        if (start == --end)
            break;
        byte temp = *start;
        *start = *end;
        *end = temp;
        ++start;
    }
}

/**
 * print a buffer
 * @param buffer {Buffer}
 */
void BufferUtil::print(const Buffer &b) {
    std::cout << toHex(b) << std::endl;
}

/**
 * compare two buffers. if b1 > b2, return 1, if b1 == b2, return 0, else return -1
 * @param b1 {Buffer}
 * @param b2 {Buffer}
 * @return
 */
int BufferUtil::compare(const Buffer &b1, const Buffer &b2) {

    size_t b1s = b1.size();
    size_t b2s = b2.size();
    if (b1s == b2s) {
        int ret = 0;
        for (int i = 0; i < b1s; ++i) {
            byte b1L = b1[i];
            byte b2L = b2[i];
            if (b1L > b2L) {
                ret = 1;
                break;
            } else if (b1L < b2L) {
                ret = -1;
                break;
            } else {
                // equal
                continue;
            }
        }
        return ret;
    } else if (b1s > b2s) {
        return 1;
    } else {
        return -1;
    }
}
