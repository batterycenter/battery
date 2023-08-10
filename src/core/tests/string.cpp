
#include "battery/core.hpp"
#include <gtest/gtest.h>

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
    EXPECT_THROW(""_b.front(), b::out_of_range_error);
    EXPECT_THROW(""_b.back(), b::out_of_range_error);
    EXPECT_THROW(str.at(11), b::out_of_range_error);
}

TEST(BatteryCore_String, PushPop) {
    b::string str = "Hello World"_b;
    str.push_back(U'年');
    EXPECT_EQ(str, "Hello World年"_b);
    EXPECT_EQ(b::string(str.pop_back()), "年"_b);
    EXPECT_EQ(str, "Hello World"_b);
    EXPECT_THROW(""_b.pop_back(), b::out_of_range_error);
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
    EXPECT_THROW((void)str.substr(12), b::out_of_range_error);
    EXPECT_THROW((void)str.substr(12, 1), b::out_of_range_error);
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