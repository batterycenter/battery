
#include "battery/core.hpp"
#include <gtest/gtest.h>

TEST(BatteryCore_ByteArray, ConstructorAndOperators) {
    b::bytearray ba1;
    EXPECT_TRUE(ba1.empty());

    std::vector<uint8_t> vec = { 0x01, 0x02, 0x03 };
    b::bytearray ba2 = vec;
    EXPECT_EQ(ba2.size(), 3);
    EXPECT_EQ(ba2[0], 0x01);
    EXPECT_EQ(ba2[1], 0x02);
    EXPECT_EQ(ba2[2], 0x03);
    b::bytearray ba3 = std::move(vec);
    EXPECT_EQ(ba3.size(), 3);
    EXPECT_EQ(ba3[0], 0x01);
    EXPECT_EQ(ba3[1], 0x02);
    EXPECT_EQ(ba3[2], 0x03);

    b::bytearray ba6 = { 0x01, 0x02, 0x03 };

    std::vector<uint8_t> vec2 = { 0x01, 0x02, 0x03 };
    b::bytearray ba7;
    ba7 = vec2;
    EXPECT_EQ(ba7.size(), 3);
    EXPECT_EQ(ba7[0], 0x01);
    EXPECT_EQ(ba7[1], 0x02);
    EXPECT_EQ(ba7[2], 0x03);

    b::bytearray ba8;
    ba8 = std::move(vec2);
    EXPECT_EQ(ba8.size(), 3);
    EXPECT_EQ(ba8[0], 0x01);
    EXPECT_EQ(ba8[1], 0x02);
    EXPECT_EQ(ba8[2], 0x03);
}

TEST(BatteryCore_ByteArray, Format) {
    b::bytearray ba = { 0x01, 0x02, 0x03 };
    auto result = b::format("'{}'", ba);
    EXPECT_EQ(result, "'[1, 2, 3]'");
}