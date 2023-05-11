
#include "battery/core/all.hpp"
#include <gtest/gtest.h>

TEST(BatteryCore_Time, Time_S) {
    auto start = b::time();
    b::sleep(0.05);
    auto elapsed = b::time() - start;
    EXPECT_GT(elapsed, 0.04);
    EXPECT_LT(elapsed, 0.06);
}

TEST(BatteryCore_Time, Time_Precise) {
    auto start = b::time();
    b::sleep_precise(0.005);
    auto elapsed = b::time() - start;
    EXPECT_GT(elapsed, 0.004);
    EXPECT_LT(elapsed, 0.006);
}

TEST(BatteryCore_Time, Time_Epoch) {
    auto start = b::epoch_time();
    b::sleep(1.5);
    auto elapsed = b::epoch_time() - start;
    EXPECT_GE(elapsed, 1);
    EXPECT_LE(elapsed, 2);
}

TEST(BatteryCore_Time, Time_EpochMS) {
    auto start = b::epoch_time_ms();
    b::sleep_ms(300);
    auto elapsed = b::epoch_time_ms() - start;
    EXPECT_GT(elapsed, 200);
    EXPECT_LT(elapsed, 400);
}

TEST(BatteryCore_Time, Time_EpochUS) {
    auto start = b::epoch_time_us();
    b::sleep_ms(300);
    auto elapsed = b::epoch_time_us() - start;
    EXPECT_GT(elapsed, 200000);
    EXPECT_LT(elapsed, 400000);
}
