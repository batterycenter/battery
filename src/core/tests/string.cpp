
#include "battery/core.hpp"
#include <gtest/gtest.h>

#include "utf8.h"

TEST(BatteryCore_String, utf8ValidStd) {
    EXPECT_TRUE(b::string::is_valid_utf8("年 Hello World! 本"));
    EXPECT_FALSE(b::string::is_valid_utf8("年 Hello \xC0\xAFWorld! 本"));   // Invalid utf-8
}




TEST(BatteryCore_String, StringFromCharPtr) {
    const char* str = "年 Hello World! 本";
    b::string bstr = str;
    EXPECT_EQ(str, bstr.str());
}

TEST(BatteryCore_String, StringFromU8Ptr) {
    const char8_t* str = u8"年 Hello World! 本";
    b::string bstr = str;
    EXPECT_EQ(str, bstr.u8());
}

TEST(BatteryCore_String, StringFromU16Ptr) {
    const char16_t* str = u"年 Hello World! 本";
    b::string bstr = str;
    EXPECT_EQ(str, bstr.u16());
}

TEST(BatteryCore_String, StringFromU32Ptr) {
    const char32_t* str = U"年 Hello World! 本";
    b::string bstr = str;
    EXPECT_EQ(str, bstr.u32());
}




TEST(BatteryCore_String, StringFromCharString) {
    std::string str = "年 Hello World! 本";
    b::string bstr = str;
    EXPECT_EQ(str, bstr.str());
}

TEST(BatteryCore_String, StringFromU8String) {
    std::u8string str = u8"年 Hello World! 本";
    b::string bstr = str;
    EXPECT_EQ(str, bstr.u8());
}

TEST(BatteryCore_String, StringFromU16String) {
    std::u16string str = u"年 Hello World! 本";
    b::string bstr = str;
    EXPECT_EQ(str, bstr.u16());
}

TEST(BatteryCore_String, StringFromU32String) {
    std::u32string str = U"年 Hello World! 本";
    b::string bstr = str;
    EXPECT_EQ(str, bstr.u32());
}



TEST(BatteryCore_String, StringSetFromCharPtr) {
    const char* str = "年 Hello World! 本";
    b::string bstr;
    bstr = str;
    EXPECT_EQ(str, bstr.str());
}

TEST(BatteryCore_String, StringSetFromU8Ptr) {
    const char8_t* str = u8"年 Hello World! 本";
    b::string bstr;
    bstr = str;
    EXPECT_EQ(str, bstr.u8());
}

TEST(BatteryCore_String, StringSetFromU16Ptr) {
    const char16_t* str = u"年 Hello World! 本";
    b::string bstr;
    bstr = str;
    EXPECT_EQ(str, bstr.u16());
}

TEST(BatteryCore_String, StringSetFromU32Ptr) {
    const char32_t* str = U"年 Hello World! 本";
    b::string bstr;
    bstr = str;
    EXPECT_EQ(str, bstr.u32());
}




TEST(BatteryCore_String, StringSetFromCharString) {
    std::string str = "年 Hello World! 本";
    b::string bstr;
    bstr = str;
    EXPECT_EQ(str, bstr.str());
}

TEST(BatteryCore_String, StringSetFromU8String) {
    std::u8string str = u8"年 Hello World! 本";
    b::string bstr;
    bstr = str;
    EXPECT_EQ(str, bstr.u8());
}

TEST(BatteryCore_String, StringSetFromU16String) {
    std::u16string str = u"年 Hello World! 本";
    b::string bstr;
    bstr = str;
    EXPECT_EQ(str, bstr.u16());
}

TEST(BatteryCore_String, StringSetFromU32String) {
    std::u32string str = U"年 Hello World! 本";
    b::string bstr;
    bstr = str;
    EXPECT_EQ(str, bstr.u32());
}



TEST(BatteryCore_String, StrForEachChar32) {
    b::string str = "年 Hello World! 本";
    auto result = b::string::foreach(str, [&](char32_t c) {
        if (c == U'l') {
            return b::string(u8"");         // effectively remove 'l'
        }
        else {
            return b::string(c);
        }
    });
    EXPECT_EQ(result, b::string("年 Heo Word! 本"));
}

TEST(BatteryCore_String, StrForEachChar32_raw) {
    std::u32string str = U"年 Hello World! 本";
    auto result = b::string::foreach(str, [&](char32_t c) {
        if (c == U'l') {
            return U'x';         // effectively replace 'l' with 'x'
        }
        else {
            return c;
        }
    });
    EXPECT_EQ(result, b::string("年 Hexxo Worxd! 本"));
}




TEST(BatteryCore_String, StringSplit) {
    std::string str = "This is a string for testing";
    std::vector<b::string> target = { "This", "is", "a", "string", "for", "testing" };
    auto v = b::string::split(str, " ");
    EXPECT_EQ(v, target);
}




TEST(BatteryCore_String, StringJoin) {
    std::vector<b::string> src = { "This", "is", "a", "string", "for", "testing" };
    std::string target = "This is a string for testing";
    auto v = b::string::join(src, " ");
    EXPECT_EQ(v, target);
}




TEST(BatteryCore_String, StringReplace) {
    auto src = "This unit test is now a unit test string 本";
    auto v = b::string::replace(src, "unit test", "banana");
    EXPECT_EQ(v, "This banana is now a banana string 本");
}

TEST(BatteryCore_String, StringReplaceOne) {
    std::string str = "aa bb cc bb aa bb";
    EXPECT_EQ(b::string::replace_one(str, "aa", "cc", 0), "cc bb cc bb aa bb");
    EXPECT_EQ(b::string::replace_one(str, "bb", "aa", 0), "aa aa cc bb aa bb");
    EXPECT_EQ(b::string::replace_one(str, "bb", "cc", 2), "aa bb cc bb aa cc");
    EXPECT_EQ(b::string::replace_one(str, "aa", "cc", 1), "aa bb cc bb cc bb");
    EXPECT_EQ(b::string::replace_one(str, "aa", "cc", -1), "aa bb cc bb cc bb");
    EXPECT_EQ(b::string::replace_one(str, "bb", "aa", -2), "aa bb cc aa aa bb");
    EXPECT_EQ(b::string::replace_one(str, "cc", "dd", 2), "aa bb cc bb aa bb");
    EXPECT_EQ(b::string::replace_one(str, "aa", "dd", -3), "aa bb cc bb aa bb");
}

TEST(BatteryCore_String, StringReplaceFirstLast) {
    std::string str = "aa bb cc bb aa bb";
    EXPECT_EQ(b::string::replace_first(str, "aa", "cc"), "cc bb cc bb aa bb");
    EXPECT_EQ(b::string::replace_last(str, "aa", "cc"), "aa bb cc bb cc bb");
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

TEST(BatteryCore_String, U8StringToString) {
    std::string str = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
    auto parsed = b::string("年 本 Süßölgefäß 国 分 高");
    EXPECT_EQ(str, parsed);
}



TEST(BatteryCore_String, ToLowercase) {
    std::string str = "Süßölgefäß ÄÖÜ";
    EXPECT_EQ(b::string::to_lower(str), b::string("süßölgefäß äöü"));
}

TEST(BatteryCore_String, ToUppercase) {
    std::string str = "Süßölgefäß ÄÖÜ";
    EXPECT_EQ(b::string::to_upper(str), b::string("SÜSSÖLGEFÄSS ÄÖÜ"));
}



TEST(BatteryCore_String, Base64AndBack) {
    std::string str = "Lorem ipsum dolor sit amet consectetur adipisicing elit. Minima voluptatum velit mollitia dolorem facilis suscipit cumque, molestias ut ex magni natus laudantium totam quisquam odit consectetur reprehenderit non quae vitae?";
    std::string encoded = "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQgY29uc2VjdGV0dXIgYWRpcGlzaWNpbmcgZWxpdC4gTWluaW1hIHZvbHVwdGF0dW0gdmVsaXQgbW9sbGl0aWEgZG9sb3JlbSBmYWNpbGlzIHN1c2NpcGl0IGN1bXF1ZSwgbW9sZXN0aWFzIHV0IGV4IG1hZ25pIG5hdHVzIGxhdWRhbnRpdW0gdG90YW0gcXVpc3F1YW0gb2RpdCBjb25zZWN0ZXR1ciByZXByZWhlbmRlcml0IG5vbiBxdWFlIHZpdGFlPw==";
    EXPECT_EQ(b::encode_base64(str), encoded);
    EXPECT_EQ(b::decode_base64(b::encode_base64(str)), str);
}
