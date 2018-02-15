#include <cfloat>
#include "PQCMiner.h"

static void buffer_test() {
    std::string str = "4b094f5c4b00f721a7edf47c36862e4f1e08c5c7d35558c157a9574daeaed796";
    Buffer *buffer = Buffer::from(str, Buffer::Hex);
    buffer->print();
    std::string s2 = buffer->toHex();
    std::cout << s2 << " equal:" << (str == s2) << std::endl;

    delete buffer;
}

static void block_hex_double_test() {

    std::cout << Block::bits2Target(0x1d00ffff) << std::endl;
}

static void reverse_test() {
    std::string str = "4b094f5c4b00f721a7edf47c36862e4f1e08c5c7d35558c157a9574daeaed796";
    Buffer *buffer = Buffer::from(str);
    buffer->print();
    buffer->reverse();
    buffer->print();

    delete buffer;
}

static void mine_test() {
    // 00000000a141216a896c54f211301c436e557a8d55900637bbdce14c6c7bddef
    // 790229043
    Block block(1, "0000000000000000000000000000000000000000000000000000000000000000",
                "4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b",
                1518511836, 0x1d00ffff);
    PQCMiner miner(block);
    miner.run(778000000);
    // 4b094f5c4b00f721a7edf47c36862e4f1e08c5c7d35558c157a9574daeaed796
}

static void bitcoin_test() {
    Block block(1, "0000000000000000000000000000000000000000000000000000000000000000",
                "4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b",
                1518511836, 0x1d00ffff);
    PQCMiner miner(block);
    miner.setHashFunc(BitcoinHash);
    Buffer hash = miner.getHash(230900000);
    std::cout << hash.toHex() << std::endl;
    // 7a24125621104f0074eb1395b141751811206a84d4dd35cbc3afb00a1cc4c7bc
}

static void compare_test() {
    std::string str = "4b094f5c4b00f721a7edf47c36862e4f1e08c5c7d35558c157a9574daeaed796";
    Buffer *buffer = Buffer::from(str);
    std::string str2 = "00000000ffff0000000000000000000000000000000000000000000000000000";
    Buffer *buffer2 = Buffer::from(str2);
    std::cout << buffer->compare(*buffer2) << std::endl;

    std::cout << buffer2->compare(*buffer) << std::endl;

    std::cout << buffer2->compare(*buffer2) << std::endl;

    str = "00000000ffff0000000000000000000000000000000000000000000000000000";
    delete buffer;
    delete buffer2;
}

int main() {
//    buffer_test();
//    reverse_test();
    mine_test();
//    compare_test();
//    block_hex_double_test();
//    bitcoin_test();

    return 0;
}