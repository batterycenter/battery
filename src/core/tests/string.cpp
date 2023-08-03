
#include "battery/core.hpp"
#include <gtest/gtest.h>

#include "utf8.h"

TEST(BatteryCore_String, utf8ValidStd) {
    EXPECT_TRUE(b::string::is_valid_utf8("年 Hello World! 本"));
    EXPECT_FALSE(b::string::is_valid_utf8("年 Hello \xC0\xAFWorld! 本"));   // Invalid utf-8
}



//
//TEST(BatteryCore_String, StringFromCharPtr) {
//    const char* str = "年 Hello World! 本";
//    b::string bstr = str;
//    EXPECT_EQ(str, bstr.str());
//}
//
//TEST(BatteryCore_String, StringFromU8Ptr) {
//    const char8_t* str = u8"年 Hello World! 本";
//    b::string bstr = str;
//    EXPECT_EQ(str, bstr.u8());
//}
//
//TEST(BatteryCore_String, StringFromU16Ptr) {
//    const char16_t* str = u"年 Hello World! 本";
//    b::string bstr = str;
//    EXPECT_EQ(str, bstr.u16());
//}
//
//TEST(BatteryCore_String, StringFromU32Ptr) {
//    const char32_t* str = U"年 Hello World! 本";
//    b::string bstr = str;
//    EXPECT_EQ(str, bstr.u32());
//}
//
//
//
//
//TEST(BatteryCore_String, StringFromCharString) {
//    std::string str = "年 Hello World! 本";
//    b::string bstr = str;
//    EXPECT_EQ(str, bstr.str());
//}
//
//TEST(BatteryCore_String, StringFromU8String) {
//    std::u8string str = u8"年 Hello World! 本";
//    b::string bstr = str;
//    EXPECT_EQ(str, bstr.u8());
//}
//
//TEST(BatteryCore_String, StringFromU16String) {
//    std::u16string str = u"年 Hello World! 本";
//    b::string bstr = str;
//    EXPECT_EQ(str, bstr.u16());
//}
//
//TEST(BatteryCore_String, StringFromU32String) {
//    std::u32string str = U"年 Hello World! 本";
//    b::string bstr = str;
//    EXPECT_EQ(str, bstr.u32());
//}
//
//
//
//TEST(BatteryCore_String, StringSetFromCharPtr) {
//    const char* str = "年 Hello World! 本";
//    b::string bstr;
//    bstr = str;
//    EXPECT_EQ(str, bstr.str());
//}
//
//TEST(BatteryCore_String, StringSetFromU8Ptr) {
//    const char8_t* str = u8"年 Hello World! 本";
//    b::string bstr;
//    bstr = str;
//    EXPECT_EQ(str, bstr.u8());
//}
//
//TEST(BatteryCore_String, StringSetFromU16Ptr) {
//    const char16_t* str = u"年 Hello World! 本";
//    b::string bstr;
//    bstr = str;
//    EXPECT_EQ(str, bstr.u16());
//}
//
//TEST(BatteryCore_String, StringSetFromU32Ptr) {
//    const char32_t* str = U"年 Hello World! 本";
//    b::string bstr;
//    bstr = str;
//    EXPECT_EQ(str, bstr.u32());
//}
//
//
//
//
//TEST(BatteryCore_String, StringSetFromCharString) {
//    std::string str = "年 Hello World! 本";
//    b::string bstr;
//    bstr = str;
//    EXPECT_EQ(str, bstr.str());
//}
//
//TEST(BatteryCore_String, StringSetFromU8String) {
//    std::u8string str = u8"年 Hello World! 本";
//    b::string bstr;
//    bstr = str;
//    EXPECT_EQ(str, bstr.u8());
//}
//
//TEST(BatteryCore_String, StringSetFromU16String) {
//    std::u16string str = u"年 Hello World! 本";
//    b::string bstr;
//    bstr = str;
//    EXPECT_EQ(str, bstr.u16());
//}
//
//TEST(BatteryCore_String, StringSetFromU32String) {
//    std::u32string str = U"年 Hello World! 本";
//    b::string bstr;
//    bstr = str;
//    EXPECT_EQ(str, bstr.u32());
//}
//
//
//
//TEST(BatteryCore_String, StrForEachChar32) {
//    b::string str = "年 Hello World! 本";
//    auto result = b::string::foreach(str, [&](char32_t c) {
//        if (c == U'l') {
//            return b::string(u8"");         // effectively remove 'l'
//        }
//        else {
//            return b::string(c);
//        }
//    });
//    EXPECT_EQ(result, b::string("年 Heo Word! 本"));
//}
//
//TEST(BatteryCore_String, StrForEachChar32_raw) {
//    std::u32string str = U"年 Hello World! 本";
//    auto result = b::string::foreach(str, [&](char32_t c) {
//        if (c == U'l') {
//            return U'x';         // effectively replace 'l' with 'x'
//        }
//        else {
//            return c;
//        }
//    });
//    EXPECT_EQ(result, b::string("年 Hexxo Worxd! 本"));
//}
//
//
//
//
//TEST(BatteryCore_String, StringSplit) {
//    std::string str = "This is a string for testing";
//    std::vector<b::string> target = { "This", "is", "a", "string", "for", "testing" };
//    auto v = b::string::split(str, " ");
//    EXPECT_EQ(v, target);
//}
//
//
//
//
//TEST(BatteryCore_String, StringJoin) {
//    std::vector<b::string> src = { "This", "is", "a", "string", "for", "testing" };
//    std::string target = "This is a string for testing";
//    auto v = b::string::join(src, " ");
//    EXPECT_EQ(v, target);
//}
//
//
//
//
//TEST(BatteryCore_String, StringReplace) {
//    auto src = "This unit test is now a unit test string 本";
//    auto v = b::string::replace(src, "unit test", "banana");
//    EXPECT_EQ(v, "This banana is now a banana string 本");
//}
//
//TEST(BatteryCore_String, StringReplaceOne) {
//    std::string str = "aa bb cc bb aa bb";
//    EXPECT_EQ(b::string::replace_one(str, "aa", "cc", 0), "cc bb cc bb aa bb");
//    EXPECT_EQ(b::string::replace_one(str, "bb", "aa", 0), "aa aa cc bb aa bb");
//    EXPECT_EQ(b::string::replace_one(str, "bb", "cc", 2), "aa bb cc bb aa cc");
//    EXPECT_EQ(b::string::replace_one(str, "aa", "cc", 1), "aa bb cc bb cc bb");
//    EXPECT_EQ(b::string::replace_one(str, "aa", "cc", -1), "aa bb cc bb cc bb");
//    EXPECT_EQ(b::string::replace_one(str, "bb", "aa", -2), "aa bb cc aa aa bb");
//    EXPECT_EQ(b::string::replace_one(str, "cc", "dd", 2), "aa bb cc bb aa bb");
//    EXPECT_EQ(b::string::replace_one(str, "aa", "dd", -3), "aa bb cc bb aa bb");
//}
//
//TEST(BatteryCore_String, StringReplaceFirstLast) {
//    std::string str = "aa bb cc bb aa bb";
//    EXPECT_EQ(b::string::replace_first(str, "aa", "cc"), "cc bb cc bb aa bb");
//    EXPECT_EQ(b::string::replace_last(str, "aa", "cc"), "aa bb cc bb cc bb");
//}
//
//
//
//TEST(BatteryCore_String, CodePointsC) {
//    std::string codepoints = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
//    std::string str = "年 本 Süßölgefäß 国 分 高";
//    EXPECT_EQ(codepoints, str);
//}
//
//TEST(BatteryCore_String, CodePointsU8) {
//    std::u8string codepoints = u8"\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
//    std::u8string str = u8"年 本 Süßölgefäß 国 分 高";
//    ASSERT_EQ(codepoints.size(), str.size());
//    EXPECT_EQ(0, std::memcmp(codepoints.data(), str.data(), str.size()));
//}
//
//TEST(BatteryCore_String, StringLiteralsC) {
//    std::string str = "年 本";
//    std::vector<uint8_t> bytearray = {0xe5, 0xb9, 0xb4, 0x20, 0xe6, 0x9c, 0xac};
//    ASSERT_EQ(str.size(), bytearray.size());
//    EXPECT_EQ(0, std::memcmp(str.data(), bytearray.data(), str.size()));
//}
//
//TEST(BatteryCore_String, StringLiteralsU8) {
//    std::u8string str = u8"年 本";
//    std::vector<uint8_t> bytearray = {0xe5, 0xb9, 0xb4, 0x20, 0xe6, 0x9c, 0xac};
//    ASSERT_EQ(str.size(), bytearray.size());
//    EXPECT_EQ(0, std::memcmp(str.data(), bytearray.data(),str.size()));
//}
//
//TEST(BatteryCore_String, StringLiteralsC_Codepoint) {
//    std::string str = "\u5e74 \u672c";
//    std::vector<uint8_t> bytearray = {0xe5, 0xb9, 0xb4, 0x20, 0xe6, 0x9c, 0xac};
//    ASSERT_EQ(str.size(), bytearray.size());
//    EXPECT_EQ(0, std::memcmp(str.data(), bytearray.data(),str.size()));
//}
//
//TEST(BatteryCore_String, StringLiteralsU8_Codepoint) {
//    std::u8string str = u8"\u5e74 \u672c";
//    std::vector<uint8_t> bytearray = {0xe5, 0xb9, 0xb4, 0x20, 0xe6, 0x9c, 0xac};
//    ASSERT_EQ(str.size(), bytearray.size());
//    EXPECT_EQ(0, std::memcmp(str.data(), bytearray.data(),str.size()));
//}
//
//TEST(BatteryCore_String, U8StringToString) {
//    std::string str = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
//    auto parsed = b::string("年 本 Süßölgefäß 国 分 高");
//    EXPECT_EQ(str, parsed);
//}
//
//
//
//TEST(BatteryCore_String, ToLowercase) {
//    std::string str = "Süßölgefäß ÄÖÜ";
//    EXPECT_EQ(b::string::to_lower(str), b::string("süßölgefäß äöü"));
//}
//
//TEST(BatteryCore_String, ToUppercase) {
//    std::string str = "Süßölgefäß ÄÖÜ";
//    EXPECT_EQ(b::string::to_upper(str), b::string("SÜSSÖLGEFÄSS ÄÖÜ"));
//}



const auto BASE64_UTF8_TEST = u8"Lorem ipsum dolor sit amet イロハニホヘト チリヌシ ヱヒモセスン Heizölrückstoßabdämpfung";
const auto BASE64_UTF8_TEST_ENCODED = "TG9yZW0gaXBzdW0gZG9sb3Igc2l0IGFtZXQg44Kk44Ot44OP44OL44Ob44OY44OIIOODgeO"
                                      "DquODjOOCtyDjg7Hjg5Ljg6Ljgrvjgrnjg7MgSGVpesO2bHLDvGNrc3Rvw59hYmTDpG1wZnVuZw=="_b;

const std::vector<uint8_t> BASE64_BINARY_TEST = {
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
        0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
        0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
        0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F,
        0x50, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A, 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
        0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E, 0x6F,
        0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
        0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
        0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
        0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
        0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
        0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
        0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
        0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF,
};
const auto BASE64_BINARY_TEST_ENCODED = u8"AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1Njc"
                                          "4OTo7PD0+P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3"
                                          "BxcnN0dXZ3eHl6e3x9fn+AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanq"
                                          "Kmqq6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/g"
                                          "4eLj5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/w==";

TEST(BatteryCore_String, Base64TextEncode) {
    EXPECT_EQ(b::encode_base64(BASE64_UTF8_TEST), BASE64_UTF8_TEST_ENCODED);
}

TEST(BatteryCore_String, Base64TextDecode) {
    EXPECT_EQ(b::decode_base64_text(BASE64_UTF8_TEST_ENCODED), BASE64_UTF8_TEST);
}

TEST(BatteryCore_String, Base64BinaryEncode) {
    EXPECT_EQ(b::encode_base64(BASE64_BINARY_TEST), BASE64_BINARY_TEST_ENCODED);
}

TEST(BatteryCore_String, Base64BinaryDecode) {
    EXPECT_EQ(b::decode_base64_binary(BASE64_BINARY_TEST_ENCODED), BASE64_BINARY_TEST);
}

TEST(BatteryCore_String, Base64TextEncodePadding) {
    EXPECT_EQ(b::encode_base64("M"_b), "TQ=="_b);
    EXPECT_EQ(b::encode_base64("Ma"_b), "TWE="_b);
    EXPECT_EQ(b::encode_base64("Man"_b), "TWFu"_b);
    EXPECT_EQ(b::encode_base64("light w"_b), "bGlnaHQgdw=="_b);
    EXPECT_EQ(b::encode_base64("light wo"_b), "bGlnaHQgd28="_b);
    EXPECT_EQ(b::encode_base64("light wor"_b), "bGlnaHQgd29y"_b);
    EXPECT_EQ(b::encode_base64("light work"_b), "bGlnaHQgd29yaw=="_b);
    EXPECT_EQ(b::encode_base64("light work."_b), "bGlnaHQgd29yay4="_b);
}

TEST(BatteryCore_String, Base64DecodeFail) {
    ASSERT_TRUE(b::decode_base64_text("T(Fu"_b).empty());
    ASSERT_TRUE(b::decode_base64_text("TWF."_b).empty());
    ASSERT_TRUE(b::decode_base64_text("T\"F."_b).empty());
    ASSERT_TRUE(b::decode_base64_text("TW#u"_b).empty());
    ASSERT_TRUE(b::decode_base64_binary("T(Fu"_b).empty());
    ASSERT_TRUE(b::decode_base64_binary("TWF."_b).empty());
    ASSERT_TRUE(b::decode_base64_binary("T\"F."_b).empty());
    ASSERT_TRUE(b::decode_base64_binary("TW#u"_b).empty());
}

TEST(BatteryCore_String, Base64TextDecodePadding) {
    EXPECT_EQ(b::decode_base64_text("TWFu"_b), "Man"_b);
    EXPECT_EQ(b::decode_base64_text("TWE="_b), "Ma"_b);
    EXPECT_EQ(b::decode_base64_text("TWE"_b), "Ma"_b);
    EXPECT_EQ(b::decode_base64_text("TQ=="_b), "M"_b);
    EXPECT_EQ(b::decode_base64_text("TQ="_b), "M"_b);
    EXPECT_EQ(b::decode_base64_text("TQ"_b), "M"_b);
    EXPECT_EQ(b::decode_base64_text("bGlnaHQgdw=="_b), "light w"_b);
    EXPECT_EQ(b::decode_base64_text("bGlnaHQgd28="_b), "light wo"_b);
    EXPECT_EQ(b::decode_base64_text("bGlnaHQgd29y"_b), "light wor"_b);
    EXPECT_EQ(b::decode_base64_text("bGlnaHQgdw"_b), "light w"_b);
    EXPECT_EQ(b::decode_base64_text("bGlnaHQgd28"_b), "light wo"_b);
    EXPECT_EQ(b::decode_base64_text("bGlnaHQgd29y"_b), "light wor"_b);
}
