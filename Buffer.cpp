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

Buffer::Buffer(size_t count) {
    _data = new std::vector<byte>;
    if (count > 0) {
        _data->resize(count);
    }
}

Buffer::~Buffer() {
    delete _data;
    _data = nullptr;
}

void Buffer::resize(size_t count, byte value) {
    _data->resize(count, value);
}

void Buffer::append(const Buffer &b2) {
    _data->insert(_data->end(), b2._data->begin(), b2._data->end());
}

Buffer *Buffer::from(const std::string &str, Type type) {
    switch (type) {
        case Hex: {
            auto *buffer = new Buffer;

            for (int i = 0; i < str.length(); i = i + 2) {
                byte val = char2int(str[i]) * 16 + char2int(str[i + 1]);
                buffer->_data->push_back(val);
            }
            return buffer;
        }
        case Base64: {

        }
        case UTF8: {
            auto *buffer = new Buffer;
            for (int i = 0; i < str.length(); ++i) {
                byte val = str[i];
                buffer->_data->push_back(val);
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
void Buffer::writeInt32LE(size_t startIndex, int32_t val) {
    (*_data)[startIndex + 3] = (byte)(val >> 24);
    (*_data)[startIndex + 2] = (byte)(val >> 16);
    (*_data)[startIndex + 1] = (byte)(val >> 8);
    (*_data)[startIndex] = (byte)(val);
}

/**
 *  convert buffer to hex string
 * @param buffer {Buffer}
 * @return {std::string}
 */
std::string Buffer::toHex() const {
    return toString();
}

std::string Buffer::toString(Type type) const {
    switch (type) {
        case Hex: {
            std::string hex;
            size_t size = _data->size();
            hex.resize(size * 2);
            char temp[3] = {0};
            for (int i = 0; i < size; ++i) {
                sprintf(temp, "%02x", (*_data)[i]);
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
void Buffer::reverse() {
    auto start = _data->begin();
    auto end = _data->end();
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
void Buffer::print() const {
    std::cout << toHex() << std::endl;
}

/**
 * compare two buffers. if b1 > b2, return 1, if b1 == b2, return 0, else return -1
 * @param b1 {Buffer}
 * @param b2 {Buffer}
 * @return
 */
int Buffer::compare(const Buffer &other) const {
    const std::vector<byte> &b1 = *_data;
    const std::vector<byte> &b2 = *(other._data);

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

Buffer Buffer::hash(BufferHashFunc func) const {
    std::vector<byte> h = func(*_data);
    size_t size = h.size();
    Buffer b(size);
    std::copy(h.begin(), h.end(), b._data->begin());
    return b;
}
