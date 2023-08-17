
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

    std::string str = "Foo";
    b::bytearray ba4 = b::bytearray::from_string(str);
    EXPECT_EQ(ba4.size(), 3);
    EXPECT_EQ(ba4[0], 'F');
    EXPECT_EQ(ba4[1], 'o');
    EXPECT_EQ(ba4[2], 'o');

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

    std::string str2 = "Bar";
    b::bytearray ba9;
    ba9 = b::bytearray::from_string(str2);
    EXPECT_EQ(ba9.size(), 3);
    EXPECT_EQ(ba9[0], 'B');
    EXPECT_EQ(ba9[1], 'a');
    EXPECT_EQ(ba9[2], 'r');

    b::bytearray ba11;
    ba11 = { 0x01, 0x02, 0x03 };

    b::bytearray ba12 = {0x01, 0x02, 0x03 };
    EXPECT_EQ(ba12.str(), "\x01\x02\x03");
}

TEST(BatteryCore_ByteArray, Decode) {   // Only UTF-8 is decoded here. All other encodings are assumed to work too
    b::bytearray ba1 = b::bytearray::from_string("S\xc3\xbc\xc3\x9f");     // "Süß" UTF-8 encoded plain string
    EXPECT_EQ(ba1.decode<b::enc::utf8>(), "Süß"_b);
}

TEST(BatteryCore_ByteArray, Format) {
    b::bytearray ba = { 0x01, 0x02, 0x03 };
    auto result = b::format("'{}'", ba);
    EXPECT_EQ(result, "'[1, 2, 3]'"_b);
}