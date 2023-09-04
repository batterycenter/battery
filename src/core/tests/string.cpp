
#include "battery/core.hpp"
#include <gtest/gtest.h>

TEST(BatteryCore_String, IsValidUtf8) {
    // Valid
    std::vector<uint8_t> vec = { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72,
                                 0x6c, 0x64, 0x20, 0x53, 0xc3, 0xbc, 0xc3, 0x9f, 0xc3,
                                 0xb6, 0x6c, 0x67, 0x65, 0x66, 0xc3, 0xa4, 0xc3, 0x9f,
                                 0x20, 0xe5, 0xb9, 0xb4, 0x20, 0xe6, 0x9c, 0xac, 0x20 };
    std::string str = std::string(vec.begin(), vec.end());
    EXPECT_TRUE(b::is_utf8(str));
    // Invalid
    std::vector<uint8_t> vec2 = { 0x48, 0x65, 0x6c, 0x6c, 0x6f, 0x20, 0x57, 0x6f, 0x72,
                                  0x6c, 0x64, 0x20, 0x53, 0xc3, 0xbc, 0xc3, 0x9f, 0xc3,
                                  0xb6, 0x6c, 0x67, 0x65, 0xFF, 0xc3, 0xa4, 0xc3, 0x9f,
                                  0x20, 0xe5, 0xb9, 0xb4, 0x20, 0xe6, 0x9c, 0xac, 0x20 };
    std::string str2 = std::string(vec2.begin(), vec2.end());
    EXPECT_FALSE(b::is_utf8(str2));
}

TEST(BatteryCore_String, SplitJoin) {
    std::string testString = "Süßölgefäß年";

    auto split = b::split(testString, "ß");
    EXPECT_EQ(split.size(), 3);
    EXPECT_EQ(split[0], "Sü");
    EXPECT_EQ(split[1], "ölgefä");
    EXPECT_EQ(split[2], "年");
    EXPECT_EQ(b::join(split, "ù"), "Süùölgefäù年");

    // Reversed
    EXPECT_EQ(split.size(), 3);
    EXPECT_EQ(split[0], "Sü");
    EXPECT_EQ(split[1], "ölgefä");
    EXPECT_EQ(split[2], "年");
    EXPECT_EQ(b::rjoin(split, "ù"), "年ùölgefäùSü");
}

TEST(BatteryCore_String, RegexReplace) {
    // This is an example regex for removing comments in source code
    auto pattern = R"(\/\*[\s\S]*?\*\/|\/\/.*)";
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
/**/)";
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

)";

    // Static
    EXPECT_EQ(b::regex_replace(testString, pattern, ""), expectedResult);
}

TEST(BatteryCore_String, RegexMatch) {
    auto pattern = R"(\d+)";
    auto testString = "Hello 123 World! 456";

    auto matches = b::regex_match(testString, pattern);
    EXPECT_EQ(matches.size(), 2);
    EXPECT_EQ(matches[0], "123");
    EXPECT_EQ(matches[1], "456");
}

TEST(BatteryCore_String, SimpleReplace) {
    auto testString = "Hello World!";

    // Custom static replacer
    EXPECT_EQ(b::replace(testString, "World", [](auto match) {
        EXPECT_EQ(match, 0);
        return "Battery";
    }), "Hello Battery!");

    // Static
    EXPECT_EQ(b::replace(testString, "World", "Battery"), "Hello Battery!");
}

TEST(BatteryCore_String, SimpleReplaceFirstLast) {
    auto testString = "This test is a very good test!";

    EXPECT_EQ(b::replace_first(testString, "test", "Apple"), "This Apple is a very good test!");
    EXPECT_EQ(b::replace_last(testString, "test", "Apple"), "This test is a very good Apple!");
}

TEST(BatteryCore_String, ToLower) {
    auto testString = "Hello World Süßölgefäß!";

    EXPECT_EQ(b::to_lower(testString), "hello world süßölgefäß!");
}

TEST(BatteryCore_String, ToUpper) {
    auto testString = "Hello World Süßölgefäß!";

    EXPECT_EQ(b::to_upper(testString), "HELLO WORLD SÜSSÖLGEFÄSS!");
}
