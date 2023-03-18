
#include "battery/core/all.h"
#include <gtest/gtest.h>

TEST(BatteryCore_String, StringSplit) {
    std::string str = "This is a string for testing";
    std::vector<std::string> target = { "This", "is", "a", "string", "for", "testing" };
    auto v = b::split(str, " ");
    EXPECT_EQ(v, target);
}

TEST(BatteryCore_String, StringSplitU8) {
    std::string str = "This is another Süßölgefäß";
    std::vector<std::string> target = { "This is another ", "ölgefäß" };
    auto v = b::split(str, "Süß");
    EXPECT_EQ(v, target);
}

TEST(BatteryCore_String, StringJoin) {
    std::vector<std::string> src = { "This", "is", "a", "string", "for", "testing" };
    std::string target = "This is a string for testing";
    auto v = b::join(src, " ");
    EXPECT_EQ(v, target);
}

TEST(BatteryCore_String, StringReplace) {
    auto src = "This unit test is now a unit test string";
    auto v = b::replace(src, "unit test", "banana");
    EXPECT_EQ(v, "This banana is now a banana string");
}

TEST(BatteryCore_String, StringReplaceOne) {
    std::string str = "aa bb cc bb aa bb";
    EXPECT_EQ(b::replace_one(str, "aa", "cc", 0), "cc bb cc bb aa bb");
    EXPECT_EQ(b::replace_one(str, "bb", "aa", 0), "aa aa cc bb aa bb");
    EXPECT_EQ(b::replace_one(str, "bb", "cc", 2), "aa bb cc bb aa cc");
    EXPECT_EQ(b::replace_one(str, "aa", "cc", 1), "aa bb cc bb cc bb");
    EXPECT_EQ(b::replace_one(str, "aa", "cc", -1), "aa bb cc bb cc bb");
    EXPECT_EQ(b::replace_one(str, "bb", "aa", -2), "aa bb cc aa aa bb");
    EXPECT_EQ(b::replace_one(str, "cc", "dd", 2), "aa bb cc bb aa bb");
    EXPECT_EQ(b::replace_one(str, "aa", "dd", -3), "aa bb cc bb aa bb");
}

TEST(BatteryCore_String, StringReplaceFirstLast) {
    std::string str = "aa bb cc bb aa bb";
    EXPECT_EQ(b::replace_first(str, "aa", "cc"), "cc bb cc bb aa bb");
    EXPECT_EQ(b::replace_last(str, "aa", "cc"), "aa bb cc bb cc bb");
}

TEST(BatteryCore_String, UTF8ToWCharAndBack) {
    // "年 本 Süßölgefäß 国 分 高"
    std::string u8 = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
    b::osstring os = b::to_osstring(u8);
    EXPECT_EQ(u8, b::from_osstring(os));
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
    auto u8str = b::to_u8string(_str);
    ASSERT_EQ(u8.size(), u8str.size());
    EXPECT_EQ(0, std::memcmp(u8.data(), u8str.data(),u8.size()));
}

TEST(BatteryCore_String, U8StringToString) {
    std::string str = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
    auto parsed = b::from_u8string(u8"年 本 Süßölgefäß 国 分 高");
    EXPECT_EQ(str, parsed);
}

TEST(BatteryCore_String, Utf8To32AndBack) {
    std::string str = "年 本 Süßölgefäß 国 分 高";
    std::u32string u32 = b::to_u32(str);
    EXPECT_EQ(str, b::to_u8(u32));
}

TEST(BatteryCore_String, ToLowercase) {
    std::string str = "Süßölgefäß ÄÖÜ";
    EXPECT_EQ(b::to_lower(str), "süßölgefäß äöü");
}

TEST(BatteryCore_String, ToUppercase) {
    std::string str = "Süßölgefäß ÄÖÜ";
    EXPECT_EQ(b::to_upper(str), "SÜSSÖLGEFÄSS ÄÖÜ");
}

TEST(BatteryCore_String, Base64AndBack) {
    std::string str = "Lorem ipsum dolor sit amet consectetur adipisicing elit. Minima voluptatum velit mollitia dolorem facilis suscipit cumque, molestias ut ex magni natus laudantium totam quisquam odit consectetur reprehenderit non quae vitae?";
    std::string encoded = "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQgY29uc2VjdGV0dXIgYWRpcGlzaWNpbmcgZWxpdC4gTWluaW1hIHZvbHVwdGF0dW0gdmVsaXQgbW9sbGl0aWEgZG9sb3JlbSBmYWNpbGlzIHN1c2NpcGl0IGN1bXF1ZSwgbW9sZXN0aWFzIHV0IGV4IG1hZ25pIG5hdHVzIGxhdWRhbnRpdW0gdG90YW0gcXVpc3F1YW0gb2RpdCBjb25zZWN0ZXR1ciByZXByZWhlbmRlcml0IG5vbiBxdWFlIHZpdGFlPw==";
    EXPECT_EQ(b::encode_base64(str), encoded);
    EXPECT_EQ(b::decode_base64(b::encode_base64(str)), str);
}
