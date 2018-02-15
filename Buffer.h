//
// Created by Isaac on 2018/2/15.
//

#ifndef PQCMINER_BUFFER_H
#define PQCMINER_BUFFER_H

#include <vector>
#include "types.h"

class Buffer;
typedef std::vector<byte> (* BufferHashFunc)(const std::vector<byte> &data);

class Buffer {
private:
    std::vector<byte> *_data;
public:
    typedef enum {
        Hex,
        UTF8,
        Base64
    } Type;

    Buffer(size_t count = 0);

    ~Buffer();

    static Buffer *from(const std::string &str, Type type = Hex);

    /**
     * append buffer b2 to this buffer
     * @param b2
     */
    void append(const Buffer &b2);

    void resize(size_t count, byte value);
    /**
     *  write an int32 value at index into a Buffer
     * @param buffer {Buffer}
     * @param startIndex {size_t}
     * @param val {int32_t}
     */
    void writeInt32LE(size_t startIndex, int32_t val);

    /**
     *  convert buffer to hex string
     * @param buffer {Buffer}
     * @return {std::string}
     */
    std::string toHex() const;

    std::string toString(Type type = Hex) const;

    /**
     * reverse a buffer
     * @param buffer {Buffer}
     */
    void reverse();

    /**
     * print a buffer
     * @param buffer {Buffer}
     */
    void print() const;

    /**
     * compare two buffers. if b1 > b2, return 1, if b1 == b2, return 0, else return -1
     * @param b1 {Buffer}
     * @param b2 {Buffer}
     * @return
     */
    int compare(const Buffer &b2) const;

    Buffer hash(BufferHashFunc func) const;
};


#endif //PQCMINER_BUFFER_H
