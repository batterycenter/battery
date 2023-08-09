
#include "battery/core.hpp"
#include <gtest/gtest.h>

#ifndef TESTDATA_FOLDER
#error TESTDATA_FOLDER is not defined! Something is wrong with the unit-testing build system!
#endif

// TODO: Check current_path for UTF-8

TEST(BatteryCore_Filesystem, ReadFileWithSpacesAndKanji) {

    // In the testdata folder, there should be a file named "年 本 Süßölgefäß 国 分 高", with the content "utf-8 filenames work".
    // We just try to open it to see if unicode filenames work

    // The filename from above written in unicode codepoints
    b::fs::path filename = TESTDATA_FOLDER "/\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8"_b;

    b::fs::ifstream file(filename);
    ASSERT_FALSE(file.fail());
    file.close();

    auto content = b::fs::read_text_file(filename);
    EXPECT_EQ(content, u8"utf-8 filenames work");

    content = b::fs::read_text_file_nothrow(filename).value();
    EXPECT_EQ(content, u8"utf-8 filenames work");
//
//    content.clear();
//    b::fs::read_text_file_in_chunks(filename, 2, [&content](auto str) {
//        content += str;
//    });
//    EXPECT_EQ(content, u8"utf-8 filenames work");
}

TEST(BatteryCore_Filesystem, ReadWriteFileContent_UTF8) {

    // Here we write UTF-8 encoded binary data to a file and then read it as a regular file and check the content

    b::string content = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8"_b;

//    ASSERT_TRUE(b::fs::write_text_file_nothrow("filesystem-test.txt"_b, content));
//    ASSERT_TRUE(b::fs::write_binary_file_nothrow("filesystem-test.txt"_b, content));
//
//    ASSERT_TRUE(b::fs::read_text_file_nothrow("filesystem-test.txt"_b).has_value());
//    ASSERT_TRUE(b::fs::read_binary_file_nothrow("filesystem-test.txt"_b).has_value());
}

void CreateDirectoryOnFileWrite_RunTest(const b::fs::path& path, bool create_directories, bool expected_to_exist) {
    if (b::fs::exists(path)) {
        b::fs::remove(path);
        EXPECT_FALSE(b::fs::exists(path));
    }

    b::fs::ofstream outfile(path, b::fs::Mode::TEXT, create_directories);
    if (create_directories) {
        ASSERT_FALSE(outfile.fail());
    }
    else if (!expected_to_exist) {
        ASSERT_TRUE(outfile.fail());
        if (outfile.fail()) return;
    }
    outfile << "Test";
    outfile.close();

    EXPECT_EQ(b::fs::exists(path), expected_to_exist);
    b::fs::remove(path);
    EXPECT_FALSE(b::fs::exists(path));

    if (b::fs::exists("test"_b)) {
        b::fs::remove_all("test"_b);
    }
}

TEST(BatteryCore_Filesystem, CreateDirectoryOnFileWrite) {

    if (b::fs::exists("test"_b)) {
        b::fs::remove_all("test"_b);
    }

    CreateDirectoryOnFileWrite_RunTest("filesystem-test.txt"_b, true, true);
    CreateDirectoryOnFileWrite_RunTest("../filesystem-test.txt"_b, true, true);
    CreateDirectoryOnFileWrite_RunTest("test/more/filesystem-test.txt"_b, true, true);
    CreateDirectoryOnFileWrite_RunTest("filesystem-test.txt"_b, false, true);
    CreateDirectoryOnFileWrite_RunTest("../filesystem-test.txt"_b, false, true);
    CreateDirectoryOnFileWrite_RunTest("test/more/filesystem-test.txt"_b, false, false);
}

TEST(BatteryCore_Filesystem, FS_PathOperators) {
    b::fs::path p1("../test/"_b);
    p1.append("more/folders"_b);
    b::fs::path p2(p1);
    p2 += "file.txt"_b;
    auto p3 = p1 + "test"_b;

    p1.make_preferred();
    p2.make_preferred();
    p3.make_preferred();

    EXPECT_EQ(b::string::replace(p1.string(), "\\"_b, "/"_b), u8"../test/more/folders");
    EXPECT_EQ(b::string::replace(p2.string(), "\\"_b, "/"_b), u8"../test/more/folders/file.txt");
    EXPECT_EQ(b::string::replace(p3.string(), "\\"_b, "/"_b), u8"../test/more/folders/test");

    b::fs::path p4("C:/some/directory"_b);
    p4 /= "more/directories"_b;
    auto p5 = p4 / "one more"_b;
    EXPECT_EQ(b::string::replace(p5.string(), "\\"_b, "/"_b), "C:/some/directory/more/directories/one more"_b);
}

TEST(BatteryCore_Filesystem, FS_Path_UTF8) {
    b::fs::path p1("files/年 本/"_b);
    EXPECT_EQ(p1.string(), u8"files/年 本/");

    b::fs::path p2(u8"files/年 本/");
    EXPECT_EQ(p2.string(), u8"files/年 本/");

    b::fs::path p3(u8"ölfile.süß年");
    EXPECT_EQ(p3.extension().string(), u8".süß年");

    b::fs::path p4(u8"Ölgefäß/ölfile.süß年");
    EXPECT_EQ(p4.filename().string(), u8"ölfile.süß年");
}
