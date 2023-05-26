
#include "battery/core/all.hpp"
#include <gtest/gtest.h>

#include "utf8.h"

TEST(BatteryCore_String, OsStringAndBackStr) {
    std::string str = "年 Hello World! 本";
    auto osstr = b::to_osstring(str);
    EXPECT_EQ(str, b::osstring_to_str(osstr));
}

TEST(BatteryCore_String, OsStringAndBackU8) {
    std::u8string str = u8"年 Hello World! 本";
    auto osstr = b::to_osstring(str);
    EXPECT_EQ(str, b::osstring_to_u8(osstr));
}

TEST(BatteryCore_String, OsStringAndBackU32) {
    std::u32string str = U"年 Hello World! 本";
    auto osstr = b::to_osstring(str);
    EXPECT_EQ(str, b::osstring_to_u32(osstr));
}

TEST(BatteryCore_String, u8StringToStdStringAndBack) {
    std::string str = "年 Hello World! 本";
    auto u8 = b::u8_from_std_string(str);
    EXPECT_EQ(u8, u8"年 Hello World! 本");
    EXPECT_EQ(str, b::u8_as_str(u8));
}




TEST(BatteryCore_String, u8ValidStd) {
    EXPECT_TRUE(b::is_valid_u8("年 Hello World! 本"));
    EXPECT_FALSE(b::is_valid_u8("年 Hello \xC0\xAFWorld! 本"));   // Invalid utf-8
}

TEST(BatteryCore_String, u8Valid) {
    EXPECT_TRUE(b::is_valid_u8(u8"年 Hello World! 本"));

    std::u8string invalid = { 0x40, 0xC0, 0xAF, 0x40 };   // Invalid utf-8. Very hard to achieve using u8 literals.
    EXPECT_FALSE(b::is_valid_u8(invalid));
}

TEST(BatteryCore_String, u32Valid) {
    EXPECT_TRUE(b::is_valid_u32(U"年 Hello World! 本"));

    std::u32string invalid = { 0x0048, 0x0065, 0x006C, 0x006C, 0x006F, 0xFFFFFFF0, 0x0021 };   // Invalid utf-32. Very hard to achieve using U literals.
    EXPECT_FALSE(b::is_valid_u32(invalid));
}




TEST(BatteryCore_String, ToU8) {
    EXPECT_EQ(b::to_u8(U"年 Hello World! 本"), u8"年 Hello World! 本");
}

TEST(BatteryCore_String, ToU8Char) {
    EXPECT_EQ(b::to_u8(U'年'), u8"年");
}

TEST(BatteryCore_String, ToU32) {
    EXPECT_EQ(b::to_u32(u8"年 Hello World! 本"), U"年 Hello World! 本");
}

TEST(BatteryCore_String, ToU32Char) {
    EXPECT_EQ(b::to_u32(U'年'), U"年");
}




TEST(BatteryCore_String, u8ForEach) {
    std::u8string str = u8"年 Hello World! 本";
    auto result = b::u8_foreach(str, [&](char32_t c) {
        if (c == U'l') {
            return std::u8string(u8"");         // effectively remove 'l'
        }
        else {
            return b::to_u8(c);
        }
    });
    EXPECT_EQ(result, u8"年 Heo Word! 本");
}

TEST(BatteryCore_String, u32ForEach) {
    std::u32string str = U"年 Hello World! 本";
    auto result = b::u32_foreach(str, [&](char32_t c) {
        if (c == U'l') {
            return std::u32string(U"");         // effectively remove 'l'
        }
        else {
            return b::to_u32(c);
        }
    });
    EXPECT_EQ(result, U"年 Heo Word! 本");
}




TEST(BatteryCore_String, StringSplit) {
    std::string str = "This is a string for testing";
    std::vector<std::string> target = { "This", "is", "a", "string", "for", "testing" };
    auto v = b::split(str, " ");
    EXPECT_EQ(v, target);
}

TEST(BatteryCore_String, StringSplitU8) {
    std::u8string str = u8"This is another Süßölgefäß 本";
    std::vector<std::u8string> target = { u8"This is another ", u8"ölgefäß 本" };
    auto v = b::split(str, u8"Süß");
    EXPECT_EQ(v, target);
}

TEST(BatteryCore_String, StringSplitU32) {
    std::u32string str = U"This is another Süßölgefäß 本";
    std::vector<std::u32string> target = { U"This is another ", U"ölgefäß 本" };
    auto v = b::split(str, U"Süß");
    EXPECT_EQ(v, target);
}




TEST(BatteryCore_String, StringJoin) {
    std::vector<std::string> src = { "This", "is", "a", "string", "for", "testing" };
    std::string target = "This is a string for testing";
    auto v = b::join(src, " ");
    EXPECT_EQ(v, target);
}

TEST(BatteryCore_String, StringJoinU8) {
    std::vector<std::u8string> src = { u8"This", u8"is", u8"a", u8"string", u8"for", u8"Süßölgefäß", u8"testing", u8"本" };
    std::u8string target = u8"This is a string for Süßölgefäß testing 本";
    auto v = b::join(src, u8" ");
    EXPECT_EQ(v, target);
}

TEST(BatteryCore_String, StringJoinU32) {
    std::vector<std::u32string> src = { U"This", U"is", U"a", U"string", U"for", U"Süßölgefäß", U"testing", U"本" };
    std::u32string target = U"This is a string for Süßölgefäß testing 本";
    auto v = b::join(src, U" ");
    EXPECT_EQ(v, target);
}




TEST(BatteryCore_String, StringReplace) {
    auto src = "This unit test is now a unit test string 本";
    auto v = b::replace(src, "unit test", "banana");
    EXPECT_EQ(v, "This banana is now a banana string 本");
}

TEST(BatteryCore_String, StringReplaceU8) {
    auto src = u8"This unit test is now a unit test string 本";
    auto v = b::replace(src, u8"unit test", u8"banana");
    EXPECT_EQ(v, u8"This banana is now a banana string 本");
}

TEST(BatteryCore_String, StringReplaceU32) {
    auto src = U"This unit test is now a unit test string 本";
    auto v = b::replace(src, U"unit test", U"banana");
    EXPECT_EQ(v, U"This banana is now a banana string 本");
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

TEST(BatteryCore_String, StringReplaceOneU8) {
    std::u8string str = u8"aa bb cc bb aa bb";
    EXPECT_EQ(b::replace_one(str, u8"aa", u8"cc", 0), u8"cc bb cc bb aa bb");
    EXPECT_EQ(b::replace_one(str, u8"bb", u8"aa", 0), u8"aa aa cc bb aa bb");
    EXPECT_EQ(b::replace_one(str, u8"bb", u8"cc", 2), u8"aa bb cc bb aa cc");
    EXPECT_EQ(b::replace_one(str, u8"aa", u8"cc", 1), u8"aa bb cc bb cc bb");
    EXPECT_EQ(b::replace_one(str, u8"aa", u8"cc", -1), u8"aa bb cc bb cc bb");
    EXPECT_EQ(b::replace_one(str, u8"bb", u8"aa", -2), u8"aa bb cc aa aa bb");
    EXPECT_EQ(b::replace_one(str, u8"cc", u8"dd", 2), u8"aa bb cc bb aa bb");
    EXPECT_EQ(b::replace_one(str, u8"aa", u8"dd", -3), u8"aa bb cc bb aa bb");
}

TEST(BatteryCore_String, StringReplaceOneU32) {
    std::u32string str = U"aa bb cc bb aa bb";
    EXPECT_EQ(b::replace_one(str, U"aa", U"cc", 0), U"cc bb cc bb aa bb");
    EXPECT_EQ(b::replace_one(str, U"bb", U"aa", 0), U"aa aa cc bb aa bb");
    EXPECT_EQ(b::replace_one(str, U"bb", U"cc", 2), U"aa bb cc bb aa cc");
    EXPECT_EQ(b::replace_one(str, U"aa", U"cc", 1), U"aa bb cc bb cc bb");
    EXPECT_EQ(b::replace_one(str, U"aa", U"cc", -1), U"aa bb cc bb cc bb");
    EXPECT_EQ(b::replace_one(str, U"bb", U"aa", -2), U"aa bb cc aa aa bb");
    EXPECT_EQ(b::replace_one(str, U"cc", U"dd", 2), U"aa bb cc bb aa bb");
    EXPECT_EQ(b::replace_one(str, U"aa", U"dd", -3), U"aa bb cc bb aa bb");
}




TEST(BatteryCore_String, StringReplaceFirstLast) {
    std::string str = "aa bb cc bb aa bb";
    EXPECT_EQ(b::replace_first(str, "aa", "cc"), "cc bb cc bb aa bb");
    EXPECT_EQ(b::replace_last(str, "aa", "cc"), "aa bb cc bb cc bb");
}

TEST(BatteryCore_String, StringReplaceFirstLastU8) {
    std::u8string str = u8"aa bb cc bb aa bb";
    EXPECT_EQ(b::replace_first(str, u8"aa", u8"cc"), u8"cc bb cc bb aa bb");
    EXPECT_EQ(b::replace_last(str, u8"aa", u8"cc"), u8"aa bb cc bb cc bb");
}

TEST(BatteryCore_String, StringReplaceFirstLastU32) {
    std::u32string str = U"aa bb cc bb aa bb";
    EXPECT_EQ(b::replace_first(str, U"aa", U"cc"), U"cc bb cc bb aa bb");
    EXPECT_EQ(b::replace_last(str, U"aa", U"cc"), U"aa bb cc bb cc bb");
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
    auto u8str = b::u8_from_std_string(_str);
    ASSERT_EQ(u8.size(), u8str.size());
    EXPECT_EQ(0, std::memcmp(u8.data(), u8str.data(),u8.size()));
}

TEST(BatteryCore_String, U8StringToString) {
    std::string str = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
    auto parsed = b::u8_as_str(u8"年 本 Süßölgefäß 国 分 高");
    EXPECT_EQ(str, parsed);
}



TEST(BatteryCore_String, ToLowercase) {
    std::string str = "Süßölgefäß ÄÖÜ";
    EXPECT_EQ(b::to_lower(str), "süßölgefäß äöü");
}

TEST(BatteryCore_String, ToLowercaseU8) {
    std::u8string str = u8"Süßölgefäß ÄÖÜ";
    EXPECT_EQ(b::to_lower(str), u8"süßölgefäß äöü");
}

TEST(BatteryCore_String, ToLowercaseU32) {
    std::u32string str = U"Süßölgefäß ÄÖÜ";
    EXPECT_EQ(b::to_lower(str), U"süßölgefäß äöü");
}



TEST(BatteryCore_String, ToUppercase) {
    std::string str = "Süßölgefäß ÄÖÜ";
    EXPECT_EQ(b::to_upper(str), "SÜSSÖLGEFÄSS ÄÖÜ");
}

TEST(BatteryCore_String, ToUppercaseU8) {
    std::u8string str = u8"Süßölgefäß ÄÖÜ";
    EXPECT_EQ(b::to_upper(str), u8"SÜSSÖLGEFÄSS ÄÖÜ");
}

TEST(BatteryCore_String, ToUppercaseU32) {
    std::u32string str = U"Süßölgefäß ÄÖÜ";
    EXPECT_EQ(b::to_upper(str), U"SÜSSÖLGEFÄSS ÄÖÜ");
}



TEST(BatteryCore_String, Base64AndBack) {
    std::string str = "Lorem ipsum dolor sit amet consectetur adipisicing elit. Minima voluptatum velit mollitia dolorem facilis suscipit cumque, molestias ut ex magni natus laudantium totam quisquam odit consectetur reprehenderit non quae vitae?";
    std::string encoded = "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQgY29uc2VjdGV0dXIgYWRpcGlzaWNpbmcgZWxpdC4gTWluaW1hIHZvbHVwdGF0dW0gdmVsaXQgbW9sbGl0aWEgZG9sb3JlbSBmYWNpbGlzIHN1c2NpcGl0IGN1bXF1ZSwgbW9sZXN0aWFzIHV0IGV4IG1hZ25pIG5hdHVzIGxhdWRhbnRpdW0gdG90YW0gcXVpc3F1YW0gb2RpdCBjb25zZWN0ZXR1ciByZXByZWhlbmRlcml0IG5vbiBxdWFlIHZpdGFlPw==";
    EXPECT_EQ(b::encode_base64(str), encoded);
    EXPECT_EQ(b::decode_base64(b::encode_base64(str)), str);
}
