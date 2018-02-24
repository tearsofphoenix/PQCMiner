#include <cfloat>
#include "PQCMiner.h"

static void buffer_test() {
    std::string str = "4b094f5c4b00f721a7edf47c36862e4f1e08c5c7d35558c157a9574daeaed796";
    Buffer *buffer = BufferUtil::from(str, BufferUtil::Hex);
    BufferUtil::print(*buffer);
    std::string s2 = BufferUtil::toHex(*buffer);
    std::cout << s2 << " equal:" << (str == s2) << std::endl;

    delete buffer;
}

static void block_hex_double_test() {

    std::cout << Block::bits2Target(0x1d00ffff) << std::endl;
}

static void reverse_test() {
    std::string str = "4b094f5c4b00f721a7edf47c36862e4f1e08c5c7d35558c157a9574daeaed796";
    Buffer *buffer = BufferUtil::from(str);
    BufferUtil::print(*buffer);
    BufferUtil::reverse(*buffer);
    BufferUtil::print(*buffer);

    delete buffer;
}

static void mine_test() {
    Block block(1, "0000000000000000000000000000000000000000000000000000000000000000",
                "4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b",
                1518511836, 0x1d00ffff);
    PQCMiner miner(block);
    miner.setNonce(1014000000);
    miner.run();
}

static void auto_multi_thread() {
    Block block(1, "0000000000000000000000000000000000000000000000000000000000000000",
                "4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b",
                1518511836, 0x1d00ffff);
//    PQCMiner::runMultiThread(block, 1423400000);
    // found: 1614136539  00000000fbe6eeb849b0c12925dfdb08626923e799190adbe2e074c3300042c2
    PQCMiner::runMultiThread(block, 1614036539, PQCMixHash);
}

static void bitcoin_test() {
    {
        Block block(1, "0000000000000000000000000000000000000000000000000000000000000000",
                    "4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b",
                    1518511836, 0x1d00ffff);
        PQCMiner miner(block);
        miner.setHashFunc(BitcoinHash);
        Buffer hash = miner.getHash(230900000);
        std::cout << BufferUtil::toHex(hash) << std::endl;
        // 7a24125621104f0074eb1395b141751811206a84d4dd35cbc3afb00a1cc4c7bc
    }
    {
        Block block(1, "0000000000000000000000000000000000000000000000000000000000000000",
                    "4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b",
                    1231006505, 0x1d00ffff);
        PQCMiner miner(block);
        miner.setHashFunc(BitcoinHash);
        Buffer hash = miner.getHash(2083236893);
        std::cout << BufferUtil::toHex(hash) << std::endl;
        // 000000000019d6689c085ae165831e934ff763ae46a2a6c172b3f1b60a8ce26f
    }
}

static void bitcoin_test2() {
    Block block(1, "0000000000000000000000000000000000000000000000000000000000000000",
                "4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b",
                1518511836, 0x1d00ffff);
    PQCMiner miner(block);
    miner.setHashFunc(BitcoinHash);
    miner.setNonce(0);
    miner.run();
}

static void bitcoin_mt_test() {
    Block block(1, "0000000000000000000000000000000000000000000000000000000000000000",
                "4a5e1e4baab89f3a32518a88c31bc87f618f76673e2cc77ab2127b7afdeda33b",
                1518511836, 0x1d00ffff);
    PQCMiner::runMultiThread(block, 72100000, BitcoinHash);
}

static void compare_test() {
    std::string str = "4b094f5c4b00f721a7edf47c36862e4f1e08c5c7d35558c157a9574daeaed796";
    Buffer *buffer = BufferUtil::from(str);
    std::string str2 = "00000000ffff0000000000000000000000000000000000000000000000000000";
    Buffer *buffer2 = BufferUtil::from(str2);
    std::cout << BufferUtil::compare(*buffer, *buffer2) << std::endl;

    std::cout << BufferUtil::compare(*buffer2, *buffer) << std::endl;

    std::cout << BufferUtil::compare(*buffer2, *buffer2) << std::endl;

    str = "00000000ffff0000000000000000000000000000000000000000000000000000";
    delete buffer;
    delete buffer2;
}

int main() {
//    buffer_test();
//    reverse_test();
//    mine_test();
    auto_multi_thread();
//    bitcoin_mt_test();
//    compare_test();
//    block_hex_double_test();
//    bitcoin_test2();

    return 0;
}