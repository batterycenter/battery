
#include "battery/core/all.h"
#include <gtest/gtest.h>
#include <fstream>
#include <filesystem>

// TODO: Test string::replace

///
/// \brief Split a string into an array of string pieces by a delimeter character. When no delimeter is found,
/// 	   only the input string is returned. Several delimeters in a row are treated as a single one
/// \param[in] str The string to be split
/// \param[in] delimeter The character to split the string at
/// \return An array of string tokens
/// \see battery::string::join()
///
std::vector<std::string> split(std::string str, char delimeter);

///
/// \brief Takes an array of string tokens and joins them into a single string. The spacer string
///        is inserted between any two string tokens being joined (empty by default)
/// \param[in] strings Array of string tokens to be joined
/// \param[in] spacer Spacer string
/// \return The combined string
/// \see battery::string::split()
///
std::string join(std::vector<std::string> strings, std::string spacer = "");

///
/// \brief Replace parts of a string with another string. The parameter `string` is iterated over
///        and any occurrences of the string `from` are replaced with the string `to`.
/// \param[in] string The string to be modified
/// \param[in] from A token to be replaced
/// \param[in] to What to replace the token with
/// \return The modified string
/// \see battery::string::replace_one()
///
std::string replace(std::string string, const std::string& from, const std::string& to);

///
/// \brief Replace one part of a string with another string. The parameter `string` is iterated over
///        and only one occurrence of the string `from` is replaced with the string `to`.
/// \param[in] string The string to be modified
/// \param[in] from A token to be replaced
/// \param[in] to What to replace the token with
/// \param[in] occurrence The index of the occurrence. 0 would mean the first occurrence, 2 the third.
///                       Additionally, -1 would be the last occurrence and -2 the second-to-last.
/// \todo Implement this function properly. At the moment, it does the same as Battery::String::Replace()
/// 	  and replaces all occurrences
/// \return The modified string
/// \see battery::string::replace()
///
std::string replace_one(std::string string, const std::string& from, const std::string& to, int occurrence = 0);

TEST(BatteryCore_String, UTF8ToUTF16AndBack) {
    // "年 本 Süßölgefäß 国 分 高"
    std::string u8 = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";
    std::u16string u16 = utf8::utf8to16(u8);
    EXPECT_EQ(u8, utf8::utf16to8(u16));
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
