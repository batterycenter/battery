
#include "battery/core.hpp"
#include <gtest/gtest.h>

#include "utf8.h"

TEST(BatteryCore_String, DefaultConstructors) {
    b::string str;                  // Default constructor
    b::string str2 = b::string();   // Move constructor
    b::string str3 = str2;          // Copy constructor
    str = str2;                     // Copy assignment operator
    str = b::string();              // Move assignment operator
}

TEST(BatteryCore_String, ConversionConstructors) {
    b::string str1 = u8"Hello Süßölgefäß 年本";                 // From const char8_t*
    b::string str2 = std::u8string(u8"Hello Süßölgefäß 年本");  // From std::u8string
    b::string str3 = u"Hello Süßölgefäß 年本";                  // From const char16_t*
    b::string str4 = std::u16string(u"Hello Süßölgefäß 年本");  // From std::u16string
    b::string str5 = U"Hello Süßölgefäß 年本";                  // From const char32_t*
    b::string str6 = std::u32string(U"Hello Süßölgefäß 年本");  // From std::u32string
    EXPECT_EQ(str1, str2);
    EXPECT_EQ(str1, str3);
    EXPECT_EQ(str1, str4);
    EXPECT_EQ(str1, str5);
    EXPECT_EQ(str1, str6);
    b::string str7 = U'年';  // From char32_t
    EXPECT_EQ(str7, u8"年"); // Comparison with const char8_t*
}

TEST(BatteryCore_String, ConversionOperators) {
    std::u8string compare = u8"Hello Süßölgefäß 年本";

    b::string str;
    str = u8"Hello Süßölgefäß 年本";                 // From const char8_t*
    EXPECT_EQ(str, compare);

    str = std::u8string(u8"Hello Süßölgefäß 年本");  // From std::u8string
    EXPECT_EQ(str, compare);

    str = u"Hello Süßölgefäß 年本";                  // From const char16_t*
    EXPECT_EQ(str, compare);

    str = std::u16string(u"Hello Süßölgefäß 年本");  // From std::u16string
    EXPECT_EQ(str, compare);

    str = U"Hello Süßölgefäß 年本";                  // From const char32_t*
    EXPECT_EQ(str, compare);

    str = std::u32string(U"Hello Süßölgefäß 年本");  // From std::u32string
    EXPECT_EQ(str, compare);

    str = U'年';  // From char32_t
    EXPECT_EQ(str, u8"年");
}

TEST(BatteryCore_String, AppendOperator) {
    b::string str = "Hello"_b;
    str += " World"_b;
    EXPECT_EQ(str, "Hello World"_b);
}

TEST(BatteryCore_String, AssignAppend) {
    b::string str;
    str.assign("Hello"_b);
    str.append(" World"_b);
    EXPECT_EQ(str, "Hello World"_b);
}

TEST(BatteryCore_String, SizeLengthEmpty) {
    b::string str;
    EXPECT_EQ(str.size(), 0);
    EXPECT_EQ(str.length(), 0);
    EXPECT_TRUE(str.empty());
    str = "Hello World"_b;
    EXPECT_EQ(str.size(), 11);
    EXPECT_EQ(str.length(), 11);
    EXPECT_FALSE(str.empty());
}

TEST(BatteryCore_String, BackFrontAt) {
    b::string str = "Hello World"_b;
    EXPECT_EQ(str.front(), 'H');
    EXPECT_EQ(str.back(), 'd');
    EXPECT_EQ(str.at(1), 'e');
    EXPECT_EQ(str.at(9), 'l');
    EXPECT_THROW(""_b.front(), std::out_of_range);
    EXPECT_THROW(""_b.back(), std::out_of_range);
    EXPECT_THROW(str.at(11), std::out_of_range);
}

TEST(BatteryCore_String, PushPop) {
    b::string str = "Hello World"_b;
    str.push_back(U'年');
    EXPECT_EQ(str, "Hello World年"_b);
    EXPECT_EQ(b::string(str.pop_back()), "年"_b);
    EXPECT_EQ(str, "Hello World"_b);
    EXPECT_THROW(""_b.pop_back(), std::out_of_range);
}

TEST(BatteryCore_String, ResizeReserveCapacity) {
    b::string str = "Hello World"_b;
    str.resize(5);
    EXPECT_EQ(str, "Hello"_b);
    str.resize(10, '!');
    EXPECT_EQ(str, "Hello!!!!!"_b);
    str.reserve(40);
    EXPECT_GT(str.capacity(), 40);
}

TEST(BatteryCore_String, Reverse) {
    b::string str = "Hello World年本"_b;
    str.reverse();
    EXPECT_EQ(str, "本年dlroW olleH"_b);
}

TEST(BatteryCore_String, Substr) {
    b::string str = "Hello World"_b;
    EXPECT_EQ(str.substr(6), "World"_b);
    EXPECT_EQ(str.substr(6, 3), "Wor"_b);
    EXPECT_EQ(str.substr(6, 20), "World"_b);
    EXPECT_THROW((void)str.substr(12), std::out_of_range);
    EXPECT_THROW((void)str.substr(12, 1), std::out_of_range);
}

TEST(BatteryCore_String, IsValidUtf8) {
    // Valid
    std::vector<uint8_t> vec = { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72,
                                 0x6c, 0x64, 0x20, 0x53, 0xc3, 0xbc, 0xc3, 0x9f, 0xc3,
                                 0xb6, 0x6c, 0x67, 0x65, 0x66, 0xc3, 0xa4, 0xc3, 0x9f,
                                 0x20, 0xe5, 0xb9, 0xb4, 0x20, 0xe6, 0x9c, 0xac, 0x20 };
    std::string str = std::string(vec.begin(), vec.end());
    EXPECT_TRUE(b::string::is_valid_utf8(str));
    // Invalid
    std::vector<uint8_t> vec2 = { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72,
                                  0x6c, 0x64, 0x20, 0x53, 0xc3, 0xbc, 0xc3, 0x9f, 0xc3,
                                  0xb6, 0x6c, 0x67, 0x65, 0xFF, 0xc3, 0xa4, 0xc3, 0x9f,
                                  0x20, 0xe5, 0xb9, 0xb4, 0x20, 0xe6, 0x9c, 0xac, 0x20 };
    std::string str2 = std::string(vec2.begin(), vec2.end());
    EXPECT_FALSE(b::string::is_valid_utf8(str2));
}

TEST(BatteryCore_String, Decode) {
    b::string testString = "Süßöl年"_b;

    // UTF-8
    std::vector<uint8_t> utf8 = { 0x53u, 0xc3u, 0xbcu, 0xc3u, 0x9fu, 0xc3u, 0xb6u, 0x6cu, 0xe5u, 0xb9u, 0xb4u };
    EXPECT_EQ(b::string::decode_utf8(std::string(utf8.begin(), utf8.end())), testString);
    EXPECT_EQ(b::string::decode_utf8(std::u8string(utf8.begin(), utf8.end())), testString);
    EXPECT_EQ(b::string::decode_u8(std::u8string(utf8.begin(), utf8.end())), testString);
    std::vector<uint8_t> invalid = { 0xFF };
    EXPECT_THROW(b::string::decode_utf8(std::string(invalid.begin(), invalid.end())), b::unicode_error);
    EXPECT_THROW(b::string::decode_utf8(std::u8string(invalid.begin(), invalid.end())), b::unicode_error);
    EXPECT_THROW(b::string::decode_u8(std::u8string(invalid.begin(), invalid.end())), b::unicode_error);

    // UTF-16
    std::u16string utf16 = u"\x0053\x00fc\x00df\x00f6\x006c\x5e74";
    EXPECT_EQ(b::string::decode_utf16(std::u16string(utf16.begin(), utf16.end())), testString);
    EXPECT_EQ(b::string::decode_utf16(std::wstring(utf16.begin(), utf16.end())), testString);
    EXPECT_EQ(b::string::decode_widestring(std::wstring(utf16.begin(), utf16.end())), testString);
    EXPECT_THROW(b::string::decode_utf16(std::u16string(u"\xD800\xD800")), b::unicode_error);
    EXPECT_THROW(b::string::decode_utf16(std::wstring(L"\xD800\xD800")), b::unicode_error);
    EXPECT_THROW(b::string::decode_widestring(std::wstring(L"\xD800\xD800")), b::unicode_error);

    // UTF-32
    std::u32string utf32 = U"\x00000053\x000000fc\x000000df\x000000f6\x0000006c\x00005e74";
    EXPECT_EQ(b::string::decode_utf32(std::u32string(utf32.begin(), utf32.end())), testString);
    EXPECT_EQ(b::string::decode_utf32(U'\x00005e74'), "年"_b);
    EXPECT_THROW(b::string::decode_utf32(std::u32string(U"\xD800\xD800")), b::unicode_error);
    EXPECT_THROW(b::string::decode_utf32(U'\xD800'), b::unicode_error);

    // Native (Windows = UTF-16 wide, Others = UTF-8)
#ifdef B_OS_WINDOWS
    EXPECT_EQ(b::string::decode_native(std::wstring(utf16.begin(), utf16.end())), testString);
    EXPECT_THROW(b::string::decode_native(std::wstring(L"\xD800\xD800")), b::unicode_error);
#else
    EXPECT_EQ(b::string::decode_native(std::string(utf8.begin(), utf8.end())), testString);
    EXPECT_THROW(b::string::decode_native(std::string(invalid.begin(), invalid.end())), b::unicode_error);
#endif

    // ASCII
    b::string asciiTest = "Hello World!"_b;
    std::vector<uint8_t> ascii = { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21 };
    EXPECT_EQ(b::string::decode_ascii(std::string(ascii.begin(), ascii.end())), asciiTest);
    EXPECT_EQ(b::string::decode_ascii('A'), "A"_b);
    EXPECT_THROW(b::string::decode_ascii("\xFF\xFF"), b::unicode_error);
    EXPECT_THROW(b::string::decode_ascii('\xFF'), b::unicode_error);

    // Latin-1 (equivalent to ISO-8859-1)
    b::string latin1Test = "ÁÂÃÄÅÆÇÈÉÊË!"_b;
    std::vector<uint8_t> latin1 = { 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0x21 };
    EXPECT_EQ(b::string::decode_latin1(std::string(latin1.begin(), latin1.end())), latin1Test);
    EXPECT_EQ(b::string::decode_latin1('A'), "A"_b);

    // ISO-8859-1 (equivalent to Latin-1)
    b::string iso88591Test = "ÁÂÃÄÅÆÇÈÉÊË!"_b;
    std::vector<uint8_t> iso88591 = { 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0x21 };
    EXPECT_EQ(b::string::decode_iso8859_1(std::string(iso88591.begin(), iso88591.end())), iso88591Test);
    EXPECT_EQ(b::string::decode_iso8859_1('A'), "A"_b);

    // Windows-1252
#ifdef B_OS_WINDOWS
    b::string windows1252Test = "Süßöl!"_b;
    std::vector<uint8_t> windows1252 = { 0x53, 0xfc, 0xdf, 0xf6, 0x6c, 0x21 };
    EXPECT_EQ(b::string::decode_windows1252(std::string(windows1252.begin(), windows1252.end())), windows1252Test);
    EXPECT_EQ(b::string::decode_windows1252('A'), "A"_b);
#endif
}

TEST(BatteryCore_String, Encode) {
    b::string testString = "Süßöl年"_b;

    // UTF-8
    std::vector<uint8_t> utf8 = {0x53, 0xc3, 0xbc, 0xc3, 0x9f, 0xc3, 0xb6, 0x6c, 0xe5, 0xb9, 0xb4};
    EXPECT_EQ(testString.encode_utf8(), std::string(utf8.begin(), utf8.end()));
    EXPECT_EQ(testString.encode_u8(), std::u8string(utf8.begin(), utf8.end()));

    // UTF-16
    std::vector<uint16_t> utf16 = {0x0053, 0x00fc, 0x00df, 0x00f6, 0x006c, 0x5e74};
    EXPECT_EQ(testString.encode_utf16(), std::u16string(utf16.begin(), utf16.end()));
    EXPECT_EQ(testString.encode_widestring(), std::wstring(utf16.begin(), utf16.end()));

    // UTF-32
    std::vector<uint32_t> utf32 = {0x00000053, 0x000000fc, 0x000000df, 0x000000f6, 0x0000006c, 0x00005e74};
    EXPECT_EQ(testString.encode_utf32(), std::u32string(utf32.begin(), utf32.end()));

    // Native (Windows = UTF-16 wide, Others = UTF-8)
#ifdef B_OS_WINDOWS
    EXPECT_EQ(testString.encode_native(), std::wstring(utf16.begin(), utf16.end()));
#else
    EXPECT_EQ(testString.encode_native(), std::string(utf8.begin(), utf8.end()));
#endif
}

TEST(BatteryCore_String, CastingOperators) {
    b::string testString = "Süßöl年"_b;
    EXPECT_EQ(testString, "Süßöl年"_b);
    EXPECT_EQ((std::u8string)testString, u8"Süßöl年");
    EXPECT_EQ((std::u16string)testString, u"Süßöl年");
    EXPECT_EQ((std::u32string)testString, U"Süßöl年");
}

TEST(BatteryCore_String, UnderlyingU32) {
    b::string testString = "Süßöl年"_b;
    EXPECT_EQ(testString.str(), U"Süßöl年");
}

TEST(BatteryCore_String, SplitJoin) {
    b::string testString = "Süßölgefäß年"_b;

    // Static
    auto split = b::string::split(testString, "ß"_b);
    EXPECT_EQ(split.size(), 3);
    EXPECT_EQ(split[0], "Sü"_b);
    EXPECT_EQ(split[1], "ölgefä"_b);
    EXPECT_EQ(split[2], "年"_b);
    EXPECT_EQ(b::string::join(split, "ù"_b), "Süùölgefäù年"_b);

    // Non-static
    split = testString.split("ß"_b);
    EXPECT_EQ(split.size(), 3);
    EXPECT_EQ(split[0], "Sü"_b);
    EXPECT_EQ(split[1], "ölgefä"_b);
    EXPECT_EQ(split[2], "年"_b);
    EXPECT_EQ("ù"_b.join(split), "Süùölgefäù年"_b);
}

TEST(BatteryCore_String, RegexReplace) {
    // This is an example regex for removing comments in source code
    auto pattern = R"(\/\*[\s\S]*?\*\/|\/\/.*)"_b;
    auto testString = R"(
var sample    = 0;
var path      = "my/path/to/something";
var my_string = "Hello World!";
// This is a comment!
function do_stuff() {
    alert(my_string);//another comment
}
/* This is a
 * multiline comment!
 */
var something;
/* programs/applications 16/*(4*2)=2 */
if(sample > new){
  do_stuff(/* arguments here */);
}
//
/**/)"_b;
    auto expectedResult = R"(
var sample    = 0;
var path      = "my/path/to/something";
var my_string = "Hello World!";

function do_stuff() {
    alert(my_string);
}

var something;

if(sample > new){
  do_stuff();
}

)"_b;

    // Static
    EXPECT_EQ(b::string::regex_replace(testString, pattern, ""_b), expectedResult);

    // Non-static
    auto other = testString;
    other.regex_replace(pattern, ""_b);
    EXPECT_EQ(other, expectedResult);
}

TEST(BatteryCore_String, RegexMatch) {
    auto pattern = R"(\d+)"_b;
    auto testString = "Hello 123 World! 456"_b;

    // Static
    auto matches = b::string::regex_match(testString, pattern);
    EXPECT_EQ(matches.size(), 2);
    EXPECT_EQ(matches[0], "123"_b);
    EXPECT_EQ(matches[1], "456"_b);

    // Non-static
    matches = testString.regex_match(pattern);
    EXPECT_EQ(matches.size(), 2);
    EXPECT_EQ(matches[0], "123"_b);
    EXPECT_EQ(matches[1], "456"_b);
}

TEST(BatteryCore_String, SimpleReplace) {
    auto testString = "Hello World!"_b;

    // Custom static replacer
    EXPECT_EQ(b::string::replace(testString, "World"_b, [](auto match) {
        EXPECT_EQ(match, 0);
        return "Battery"_b;
    }), "Hello Battery!"_b);

    // Custom non-static replacer
    auto other = testString;
    other.replace("World"_b, [](auto match) {
        EXPECT_EQ(match, 0);
        return "Battery"_b;
    });
    EXPECT_EQ(other, "Hello Battery!"_b);

    // Static
    EXPECT_EQ(b::string::replace(testString, "World"_b, "Battery"_b), "Hello Battery!"_b);

    // Non-static
    other = testString;
    other.replace("World"_b, "Battery"_b);
    EXPECT_EQ(other, "Hello Battery!"_b);
}

TEST(BatteryCore_String, SimpleReplaceFirstLast) {
    auto testString = "This test is a very good test!"_b;

    // Static
    EXPECT_EQ(b::string::replace_first(testString, "test"_b, "Apple"_b), "This Apple is a very good test!"_b);
    EXPECT_EQ(b::string::replace_last(testString, "test"_b, "Apple"_b), "This test is a very good Apple!"_b);

    // Non-static
    auto other = testString;
    other.replace_first("test"_b, "Apple"_b);
    EXPECT_EQ(other, "This Apple is a very good test!"_b);
    other = testString;
    other.replace_last("test"_b, "Apple"_b);
    EXPECT_EQ(other, "This test is a very good Apple!"_b);
}

TEST(BatteryCore_String, ToLower) {
    auto testString = "Hello World Süßölgefäß!"_b;

    // Static
    EXPECT_EQ(b::string::to_lower(testString), "hello world süßölgefäß!"_b);

    // Non-static
    auto other = testString;
    other.to_lower();
    EXPECT_EQ(other, "hello world süßölgefäß!"_b);
}

TEST(BatteryCore_String, ToUpper) {
    auto testString = "Hello World Süßölgefäß!"_b;

    // Static
    EXPECT_EQ(b::string::to_upper(testString), "HELLO WORLD SÜSSÖLGEFÄSS!"_b);

    // Non-static
    auto other = testString;
    other.to_upper();
    EXPECT_EQ(other, "HELLO WORLD SÜSSÖLGEFÄSS!"_b);
}

TEST(BatteryCore_String, IndexOperator) {
    // Const
    const b::string testString1 = "Hello World!"_b;
    EXPECT_EQ(testString1[0], 'H');
    EXPECT_EQ(testString1[1], 'e');
    EXPECT_EQ(testString1[2], 'l');
    EXPECT_EQ(testString1[3], 'l');
    EXPECT_EQ(testString1[4], 'o');
    EXPECT_EQ(testString1[5], ' ');
    EXPECT_EQ(testString1[6], 'W');
    EXPECT_EQ(testString1[7], 'o');
    EXPECT_EQ(testString1[8], 'r');
    EXPECT_EQ(testString1[9], 'l');
    EXPECT_EQ(testString1[10], 'd');
    EXPECT_EQ(testString1[11], '!');

    // Non-const
    b::string testString2 = "Hello World!"_b;
    EXPECT_EQ(testString2[0], 'H');
    EXPECT_EQ(testString2[1], 'e');
    EXPECT_EQ(testString2[2], 'l');
    EXPECT_EQ(testString2[3], 'l');
    EXPECT_EQ(testString2[4], 'o');
    EXPECT_EQ(testString2[5], ' ');
    EXPECT_EQ(testString2[6], 'W');
    EXPECT_EQ(testString2[7], 'o');
    EXPECT_EQ(testString2[8], 'r');
    EXPECT_EQ(testString2[9], 'l');
    EXPECT_EQ(testString2[10], 'd');
    EXPECT_EQ(testString2[11], '!');
}

TEST(BatteryCore_String, OtherOperators) {
    EXPECT_EQ("Hello"_b + " World!"_b, "Hello World!"_b);
    EXPECT_TRUE("Hello World!"_b == "Hello World!"_b);
    EXPECT_TRUE("Hello Battery"_b != "Hello World!"_b);
}

TEST(BatteryCore_String, Iterators) {

    // Implicit Const
    const b::string testString1 = "Hello World!"_b;
    auto it1 = testString1.begin();
    EXPECT_EQ(*it1, 'H');
    ++it1;
    EXPECT_EQ(*it1, 'e');
    ++it1;
    EXPECT_EQ(*it1, 'l');
    ++it1;
    EXPECT_EQ(*it1, 'l');
    ++it1;
    EXPECT_EQ(*it1, 'o');
    ++it1;
    EXPECT_EQ(*it1, ' ');
    ++it1;
    EXPECT_EQ(*it1, 'W');
    ++it1;
    EXPECT_EQ(*it1, 'o');
    ++it1;
    EXPECT_EQ(*it1, 'r');
    ++it1;
    EXPECT_EQ(*it1, 'l');
    ++it1;
    EXPECT_EQ(*it1, 'd');
    ++it1;
    EXPECT_EQ(*it1, '!');
    ++it1;
    EXPECT_EQ(it1, testString1.end());

    // Explicit Const
    b::string testString2 = "Hello World!"_b;
    auto it2 = testString2.cbegin();
    EXPECT_EQ(*it2, 'H');
    ++it2;
    EXPECT_EQ(*it2, 'e');
    ++it2;
    EXPECT_EQ(*it2, 'l');
    ++it2;
    EXPECT_EQ(*it2, 'l');
    ++it2;
    EXPECT_EQ(*it2, 'o');
    ++it2;
    EXPECT_EQ(*it2, ' ');
    ++it2;
    EXPECT_EQ(*it2, 'W');
    ++it2;
    EXPECT_EQ(*it2, 'o');
    ++it2;
    EXPECT_EQ(*it2, 'r');
    ++it2;
    EXPECT_EQ(*it2, 'l');
    ++it2;
    EXPECT_EQ(*it2, 'd');
    ++it2;
    EXPECT_EQ(*it2, '!');
    ++it2;
    EXPECT_EQ(it2, testString2.cend());

    // Non-const
    b::string testString3 = "Hello World!"_b;
    auto it3 = testString3.begin();
    *it3 = 'h';
    ++it3;
    *it3 = 'E';
    ++it3;
    *it3 = 'L';
    ++it3;
    *it3 = 'L';
    ++it3;
    *it3 = 'O';
    ++it3;
    *it3 = ' ';
    ++it3;
    *it3 = 'w';
    ++it3;
    *it3 = 'O';
    ++it3;
    *it3 = 'R';
    ++it3;
    *it3 = 'L';
    ++it3;
    *it3 = 'D';
    ++it3;
    *it3 = '!';
    ++it3;
    EXPECT_EQ(it3, testString3.end());
    EXPECT_EQ(testString3, "hELLO wORLD!"_b);
}

TEST(BatteryCore_String, IteratorsReverse) {
    // Implicit Const
    const b::string testString1 = "Hello World!"_b;
    auto it1 = testString1.rbegin();
    EXPECT_EQ(*it1, '!');
    ++it1;
    EXPECT_EQ(*it1, 'd');
    ++it1;
    EXPECT_EQ(*it1, 'l');
    ++it1;
    EXPECT_EQ(*it1, 'r');
    ++it1;
    EXPECT_EQ(*it1, 'o');
    ++it1;
    EXPECT_EQ(*it1, 'W');
    ++it1;
    EXPECT_EQ(*it1, ' ');
    ++it1;
    EXPECT_EQ(*it1, 'o');
    ++it1;
    EXPECT_EQ(*it1, 'l');
    ++it1;
    EXPECT_EQ(*it1, 'l');
    ++it1;
    EXPECT_EQ(*it1, 'e');
    ++it1;
    EXPECT_EQ(*it1, 'H');
    ++it1;
    EXPECT_EQ(it1, testString1.rend());

    // Explicit Const
    b::string testString2 = "Hello World!"_b;
    auto it2 = testString2.crbegin();
    EXPECT_EQ(*it2, '!');
    ++it2;
    EXPECT_EQ(*it2, 'd');
    ++it2;
    EXPECT_EQ(*it2, 'l');
    ++it2;
    EXPECT_EQ(*it2, 'r');
    ++it2;
    EXPECT_EQ(*it2, 'o');
    ++it2;
    EXPECT_EQ(*it2, 'W');
    ++it2;
    EXPECT_EQ(*it2, ' ');
    ++it2;
    EXPECT_EQ(*it2, 'o');
    ++it2;
    EXPECT_EQ(*it2, 'l');
    ++it2;
    EXPECT_EQ(*it2, 'l');
    ++it2;
    EXPECT_EQ(*it2, 'e');
    ++it2;
    EXPECT_EQ(*it2, 'H');
    ++it2;
    EXPECT_EQ(it2, testString2.crend());

    // Non-const
    b::string testString3 = "Hello World!"_b;
    auto it3 = testString3.rbegin();
    *it3 = '!';
    ++it3;
    *it3 = 'D';
    ++it3;
    *it3 = 'L';
    ++it3;
    *it3 = 'R';
    ++it3;
    *it3 = 'O';
    ++it3;
    *it3 = 'w';
    ++it3;
    *it3 = ' ';
    ++it3;
    *it3 = 'O';
    ++it3;
    *it3 = 'L';
    ++it3;
    *it3 = 'L';
    ++it3;
    *it3 = 'E';
    ++it3;
    *it3 = 'h';
    ++it3;
    EXPECT_EQ(it3, testString3.rend());
    EXPECT_EQ(testString3, "hELLO wORLD!"_b);
}

TEST(BatteryCore_String, Hash) {
    std::unordered_map<b::string, int> testMap;
    testMap["Hello"_b] = 1;
    testMap["World"_b] = 2;
    testMap["Süßölgefäß"_b] = 2;
    EXPECT_EQ(testMap["Hello"_b], 1);
    EXPECT_EQ(testMap["World"_b], 2);
    EXPECT_EQ(testMap["Süßölgefäß"_b], 2);
}

TEST(BatteryCore_String, Format) {
    b::string test = "Süßölgefäß"_b;
    EXPECT_EQ(b::format("This is a so-called {}", test),
              "This is a so-called Süßölgefäß"_b);
}

TEST(BatteryCore_String, StringLiterals) {
    b::string reference = b::string::decode_u8(u8"Süßölgefäß");
    EXPECT_EQ(reference, "Süßölgefäß"_b);
    EXPECT_EQ(reference, u8"Süßölgefäß"_b);
    EXPECT_EQ(reference, u"Süßölgefäß");
    EXPECT_EQ(reference, U"Süßölgefäß"_b);
}

// ========================================================
// =============== Begin base-64 encoding =================
// ========================================================

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

// ========================================================
// ================ End base-64 encoding ==================
// ========================================================
