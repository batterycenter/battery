
#include "battery/core/all.h"
#include <gtest/gtest.h>

TEST(BatteryCore_String, StringSplit) {
    std::string str = "This is a string for testing";
    std::vector<std::string> target = { "This", "is", "a", "string", "for", "testing" };
    auto v = battery::string::split(str, ' ');
    EXPECT_EQ(v, target);
}

TEST(BatteryCore_String, StringJoin) {
    std::vector<std::string> src = { "This", "is", "a", "string", "for", "testing" };
    std::string target = "This is a string for testing";
    auto v = battery::string::join(src, " ");
    EXPECT_EQ(v, target);
}

TEST(BatteryCore_String, StringReplace) {
    auto src = "This unit test is now a unit test string";
    auto v = battery::string::replace(src, "unit test", "banana");
    EXPECT_EQ(v, "This banana is now a banana string");
}

TEST(BatteryCore_String, StringReplaceOne) {
    GTEST_SKIP();   // TODO: Implement function properly and test it
}

TEST(BatteryCore_String, UTF8ToWCharAndBack) {
    // "年 本 Süßölgefäß 国 分 高"
    std::string u8 = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
    battery::string::osstring os = battery::string::utf8_to_osstring(u8);
    EXPECT_EQ(u8, battery::string::osstring_to_utf8(os));
}

TEST(BatteryCore_String, CodePointsC) {
    std::string codepoints = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
    std::string str = "年 本 Süßölgefäß 国 分 高";
    EXPECT_EQ(codepoints, str);
}

TEST(BatteryCore_String, CodePointsU8) {
    std::u8string codepoints = u8"\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
    std::u8string str = u8"年 本 Süßölgefäß 国 分 高";
    ASSERT_EQ(codepoints.size(), str.size());
    EXPECT_EQ(0, std::memcmp(codepoints.data(), str.data(), str.size()));
}

TEST(BatteryCore_String, StringLiteralsC) {
    std::string str = "年 本";
    std::vector<uint8_t> bytearray = {0xe5, 0xb9, 0xb4, 0x20, 0xe6, 0x9c, 0xac};
    ASSERT_EQ(str.size(), bytearray.size());
    EXPECT_EQ(0, std::memcmp(str.data(), bytearray.data(), str.size()));
}

TEST(BatteryCore_String, StringLiteralsU8) {
    std::u8string str = u8"年 本";
    std::vector<uint8_t> bytearray = {0xe5, 0xb9, 0xb4, 0x20, 0xe6, 0x9c, 0xac};
    ASSERT_EQ(str.size(), bytearray.size());
    EXPECT_EQ(0, std::memcmp(str.data(), bytearray.data(),str.size()));
}

TEST(BatteryCore_String, StringLiteralsC_Codepoint) {
    std::string str = "\u5e74 \u672c";
    std::vector<uint8_t> bytearray = {0xe5, 0xb9, 0xb4, 0x20, 0xe6, 0x9c, 0xac};
    ASSERT_EQ(str.size(), bytearray.size());
    EXPECT_EQ(0, std::memcmp(str.data(), bytearray.data(),str.size()));
}

TEST(BatteryCore_String, StringLiteralsU8_Codepoint) {
    std::u8string str = u8"\u5e74 \u672c";
    std::vector<uint8_t> bytearray = {0xe5, 0xb9, 0xb4, 0x20, 0xe6, 0x9c, 0xac};
    ASSERT_EQ(str.size(), bytearray.size());
    EXPECT_EQ(0, std::memcmp(str.data(), bytearray.data(),str.size()));
}

TEST(BatteryCore_String, StringToU8String) {
    std::string _str = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
    std::u8string u8 = u8"年 本 Süßölgefäß 国 分 高";
    auto u8str = battery::string::string_to_u8string(_str);
    ASSERT_EQ(u8.size(), u8str.size());
    EXPECT_EQ(0, std::memcmp(u8.data(), u8str.data(),u8.size()));
}

TEST(BatteryCore_String, U8StringToString) {
    std::string str = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
    auto parsed = battery::string::u8string_to_string(u8"年 本 Süßölgefäß 国 分 高");
    EXPECT_EQ(str, parsed);
}
