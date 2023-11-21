
#include "App.hpp"

//bool isPrime(uint64_t num, uint64_t* total) {
//    for (uint64_t i = 2; i < num; i++) {
//        *total++;
//        if (num % i == 0) {
//            return false;
//        }
//    }
//    return true;
//}
//
//void test(uint64_t max) {
//    uint64_t total = 0;
//    for (uint64_t i = 2; i < max; i++) {
//        isPrime(i, &total);
//    }
//    b::print("Max: {}, total number of divisions: {}, average number of divisions: {}\n",
//             max, total, (double)total / max);
//}
//
//int main(int argc, const char** argv) {
////    App app;
////    return app.run("Battery_SimpleGraphicsExample", argc, argv);
//
////    test(5);
////    test(10);
////    test(100);
////    test(1000);
////    test(10000);
////    test(100000);
////    test(1000000);
////    test(10000000);
//
//    std::vector<std::thread> threads;
//
//    for (int i = 0; i < 16; i++) {
//        threads.emplace_back(test, 10000000);
//    }
//
//    for (auto & thread : threads) {
//        thread.join();
//    }
//
//    return 0;
//
//}

int main(int argc, const char** argv) {
    App app;
    return app.run("Battery_SimpleGraphicsExample", argc, argv);
}
