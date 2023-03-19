
#include "battery/core/all.hpp"
#include <gtest/gtest.h>

#ifndef TESTDATA_FOLDER
#error TESTDATA_FOLDER is not defined! Something is wrong with the unit-testing build system!
#endif

// TODO: Check current_path for UTF-8

TEST(BatteryCore_Filesystem, ReadFileWithSpacesAndKanji) {

    // In the testdata folder, there should be a file named "年 本 Süßölgefäß 国 分 高", with the content "utf-8 filenames work".
    // We just try to open it to see if unicode filenames work

    // The filename from above written in unicode codepoints
    std::string filename = TESTDATA_FOLDER "/\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";

    b::fs::ifstream file(filename);
    ASSERT_FALSE(file.fail());

    auto content = (std::string)file;
    EXPECT_EQ(content, "utf-8 filenames work");

    file.return_to_beginning();
    content = file.to_string();
    EXPECT_EQ(content, "utf-8 filenames work");

    content.clear();
    file.return_to_beginning();
    file.read_in_chunks(2, [&content](std::string_view str) {
        content += str;
    });
    EXPECT_EQ(content, "utf-8 filenames work");
}

TEST(BatteryCore_Filesystem, ReadWriteFileContent_UTF8) {

    // Here we write UTF-8 encoded binary data to a file and then read it as a regular file and check the content

    std::string content = "\u5e74 \u672c S\u00fc\u00df\u00f6lgef\u00e4\u00df \u56fd \u5206 \u9ad8";

    auto test = [&content](b::fs::Mode mode) {
        b::fs::ofstream outfile("filesystem-test.txt", mode);
        ASSERT_FALSE(outfile.fail());
        outfile << content;
        outfile.close();

        b::fs::ifstream infile("filesystem-test.txt", mode);
        ASSERT_EQ(infile.to_string(), content);
    };

    test(b::fs::Mode::TEXT);
    test(b::fs::Mode::BINARY);
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

    if (b::fs::exists("test")) {
        b::fs::remove_all("test");
    }
}

TEST(BatteryCore_Filesystem, CreateDirectoryOnFileWrite) {

    if (b::fs::exists("test")) {
        b::fs::remove_all("test");
    }

    CreateDirectoryOnFileWrite_RunTest("filesystem-test.txt", true, true);
    CreateDirectoryOnFileWrite_RunTest("../filesystem-test.txt", true, true);
    CreateDirectoryOnFileWrite_RunTest("test/more/filesystem-test.txt", true, true);
    CreateDirectoryOnFileWrite_RunTest("filesystem-test.txt", false, true);
    CreateDirectoryOnFileWrite_RunTest("../filesystem-test.txt", false, true);
    CreateDirectoryOnFileWrite_RunTest("test/more/filesystem-test.txt", false, false);
}

TEST(BatteryCore_Filesystem, FS_PathOperators) {
    b::fs::path p1("../test/");
    p1.append("more/folders");
    b::fs::path p2(p1);
    p2 += "file.txt";
    auto p3 = p1 + "test";

    p1.make_preferred();
    p2.make_preferred();
    p3.make_preferred();

    EXPECT_EQ(b::replace(p1.to_string(), "\\", "/"), "../test/more/folders");
    EXPECT_EQ(b::replace(p2.to_string(), "\\", "/"), "../test/more/folders/file.txt");
    EXPECT_EQ(b::replace(p3.to_string(), "\\", "/"), "../test/more/folders/test");

    b::fs::path p4("C:/some/directory");
    p4 += "more/directories";
    auto p5 = p4 + "one more";
    EXPECT_EQ(b::replace(p5.to_string(), "\\", "/"), "C:/some/directory/more/directories/one more");
}

TEST(BatteryCore_Filesystem, FS_Path_UTF8) {
    b::fs::path p1("files/年 本/");
    EXPECT_EQ(p1.to_string(), "files/年 本/");

    b::fs::path p2(u8"files/年 本/");
    EXPECT_EQ(p2.to_string(), "files/年 本/");

    b::fs::path p3("ölfile.süß年");
    EXPECT_EQ(p3.extension().to_string(), ".süß年");
}
