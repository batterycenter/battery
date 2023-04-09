
#include "battery/core/all.hpp"
#include <gtest/gtest.h>

class S {
public:
    S() {
        success = false;
    }

    void lambda_function() {
        thread = b::thread([this]() {
            async();
            bool_data = !bool_data;
        });
    }

    void lambda_function_params() {
        thread = b::thread([this](int c) {
            async_int(c);
            bool_data = !bool_data;
        }, 187);
    }

    void lambda_function_static() {
        thread = b::thread([]() {
            S::static_async_int(187);
        });
    }

    void member_function() {
        thread = b::thread(&S::async, this);
    }

    void member_function_params() {
        thread = b::thread(&S::async_int, this, 187);
    }

    void member_function_static() {
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

TEST(BatteryCore_Thread, Lambda) {
    S s;
    s.lambda_function();
    s.join();
    EXPECT_TRUE(s.success);
}

TEST(BatteryCore_Thread, LambdaParams) {
    S s;
    s.lambda_function_params();
    s.join();
    EXPECT_TRUE(s.success);
}

TEST(BatteryCore_Thread, LambdaStatic) {
    S s;
    s.lambda_function_static();
    s.join();
    EXPECT_TRUE(s.success);
}

TEST(BatteryCore_Thread, MemberFunction) {
    S s;
    s.member_function();
    s.join();
    EXPECT_TRUE(s.success);
}

TEST(BatteryCore_Thread, MemberFunctionParams) {
    S s;
    s.member_function_params();
    s.join();
    EXPECT_TRUE(s.success);
}

TEST(BatteryCore_Thread, MemberFunctionStatic) {
    S s;
    s.member_function_static();
    s.join();
    EXPECT_TRUE(s.success);
}

TEST(BatteryCore_Thread, NoJoin) {
    S s;
    s.member_function();    // And we're not joining this one, it must join automatically on destruction
}

TEST(BatteryCore_Thread, StopToken) {
    std::atomic<bool> stopped = false;

    b::thread thread([&stopped](std::stop_token token) {
        while (!token.stop_requested()) {
            b::sleep(0.1);
        }
        stopped = true;
    });

    thread.request_stop();
    thread.join();
    EXPECT_TRUE(stopped);
}

TEST(BatteryCore_Thread, StopTokenParams) {
    std::atomic<bool> stopped = false;
    std::atomic<bool> values_correct = false;

    b::thread thread([&stopped,&values_correct](std::stop_token token, int i, float x) {
        while (!token.stop_requested()) {
            b::sleep(0.1);
        }
        stopped = true;
        values_correct = (i == 187 && x == 3.14f);
    }, 187, 3.14f);

    thread.request_stop();
    thread.join();
    EXPECT_TRUE(stopped);
}

class Stop {
public:
    Stop() = default;

    void member() {
        thread = b::thread(&Stop::member_thread, this, 187, 3.14f);
        thread.request_stop();
        thread.join();
    }

    void member_thread(int i, float x) {
        success = true;
        values_correct = (i == 187 && x == 3.14f);
    }

    void member_stop() {
        thread = b::thread(&Stop::member_stop_thread, this, 187, 3.14f);
        thread.request_stop();
        thread.join();
    }

    void member_stop_thread(int i, float x) {
        while(!thread.get_stop_token().stop_requested()) {
            b::sleep(0.1);
        }
        success = true;
        values_correct = (i == 187 && x == 3.14f);
    }

    std::atomic<bool> success = false;
    std::atomic<bool> values_correct = false;
    b::thread thread;
};

TEST(BatteryCore_Thread, MemberNoStop) {
    Stop stop;
    stop.member();
    EXPECT_TRUE(stop.success);
    EXPECT_TRUE(stop.values_correct);
}

TEST(BatteryCore_Thread, MemberStopTokenParams) {
    Stop stop;
    stop.member_stop();
    EXPECT_TRUE(stop.success);
    EXPECT_TRUE(stop.values_correct);
}
