//
// Created by Isaac on 2018/2/15.
//

#ifndef PQCMINER_BUFFER_H
#define PQCMINER_BUFFER_H

#include <vector>
#include "types.h"

typedef std::vector<byte> Buffer;
typedef Buffer (* BufferHashFunc)(const Buffer &data);

class BufferUtil {
public:
    typedef enum {
        Hex,
        UTF8,
        Base64
    } Type;

    static Buffer *from(const std::string &str, Type type = Hex);

    /**
     * append buffer b2 to this buffer
     * @param b2
     */
    static void append(Buffer &b1, const Buffer &b2);

    /**
     *  write an int32 value at index into a Buffer
     * @param buffer {Buffer}
     * @param startIndex {size_t}
     * @param val {int32_t}
     */
    static void writeInt32LE(Buffer &b, size_t startIndex, int32_t val);

    /**
     *  convert buffer to hex string
     * @param buffer {Buffer}
     * @return {std::string}
     */
    static std::string toHex(const Buffer &b);

    static std::string toString(const Buffer &b, Type type = Hex);

    /**
     * reverse a buffer
     * @param buffer {Buffer}
     */
    static void reverse(Buffer &b);

    /**
     * print a buffer
     * @param buffer {Buffer}
     */
    static void print(const Buffer &b);

    /**
     * compare two buffers. if b1 > b2, return 1, if b1 == b2, return 0, else return -1
     * @param b1 {Buffer}
     * @param b2 {Buffer}
     * @return
     */
    static int compare(const Buffer &b1, const Buffer &b2);
};


#endif //PQCMINER_BUFFER_H
