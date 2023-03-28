
#include "battery/core/all.hpp"
#include <gtest/gtest.h>

class S {
public:
    S() {
        success = false;
    }

    void run1() {
        thread = b::thread([this]() {
            async();
            bool_data = !bool_data;
        });
    }

    void run1_int() {
        thread = b::thread([this](int c) {
            async_int(c);
            bool_data = !bool_data;
        }, 187);
    }

    void run1_static() {
        thread = b::thread([]() {
            S::static_async_int(187);
        });
    }

    void run2() {
        thread = b::thread(&S::async, this);
    }

    void run2_int() {
        thread = b::thread(&S::async_int, this, 187);
    }

    void run2_static() {
        thread = b::thread(&S::static_async_int, 187);
    }

    bool async() {
        for (int i = 0; i < 5; i++) {
            data = "Private class members";
            std::string str = data;
            b::sleep(0.1);
        }
        success = true;
        return true;
    }

    void async_int(int i) {
        if (i != 187)
            throw std::invalid_argument("Expected value 187, but got value " + std::to_string(i) + ": Thread is broken");
        success = true;
    }

    static void static_async_int(int i) {
        if (i != 187)
            throw std::invalid_argument("Expected value 187, but got value " + std::to_string(i) + ": Thread is broken");
        success = true;
    }

    bool join() {
        thread.join();
        return true;
    }

    inline static std::atomic<bool> success = false;

private:
    std::string data;
    b::thread thread;
    std::atomic<bool> bool_data = false;
};

TEST(BatteryCore_Thread, MemberFunc) {
    S s1;
    s1.run1();
    s1.join();
    EXPECT_TRUE(s1.success);

    S s2;
    s2.run1_int();
    s2.join();
    EXPECT_TRUE(s2.success);

    S s3;
    s3.run2();
    s3.join();
    EXPECT_TRUE(s3.success);

    S s4;
    s4.run2_int();
    s4.join();
    EXPECT_TRUE(s4.success);

    S s5;
    s5.run1_static();
    s5.join();
    EXPECT_TRUE(s5.success);

    S s6;
    s6.run2_static();
    s6.join();
    EXPECT_TRUE(s6.success);

    S s7;
    s7.run2();
    // And we're not joining the last one, it should join automatically
}
