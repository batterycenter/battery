
#include "battery/core.hpp"
#include <gtest/gtest.h>

#ifndef TESTDATA_FOLDER
#error TESTDATA_FOLDER is not defined! Something is wrong with the unit-test build system!
#endif

static const b::fs::path TESTDATA_PATH = TESTDATA_FOLDER;

TEST(BatteryCore_Filesystem, Path_ConstructorsAndOperators) {
    b::fs::path path1 = "unit/test";
    EXPECT_EQ(path1.string(), "unit/test");

    b::fs::path path2 = path1;
    EXPECT_EQ(path2.string(), "unit/test");

    b::fs::path path3 = std::move(path1);
    EXPECT_EQ(path3.string(), "unit/test");

    b::fs::path path4 = "unit/test";
    EXPECT_EQ(path4.string(), "unit/test");

    b::fs::path path5;
    path5 = path4;
    EXPECT_EQ(path5.string(), "unit/test");

    b::fs::path path6;
    path6 = std::move(path4);
    EXPECT_EQ(path6.string(), "unit/test");

    b::fs::path path7;
    EXPECT_EQ(path7.string(), "");
}

TEST(BatteryCore_Filesystem, Path_AssignAppendConcatenate) {
    b::fs::path path1;
    path1 = "unit/Süß";
    EXPECT_EQ(path1.string(), "unit/Süß");

    b::fs::path path2 = "wrong-Süß";
    path2.assign("unit/Süß");
    EXPECT_EQ(path2.string(), "unit/Süß");

    b::fs::path path3 = "wrong-Süß";
    path3.assign(path2);
    EXPECT_EQ(path3.string(), "unit/Süß");

    b::fs::path path4 = "right-Süß";
    path4.append("unit/Süß");
    EXPECT_EQ(path4.make_preferred(), b::fs::path("right-Süß/unit/Süß").make_preferred());

    b::fs::path path5 = "right-Süß";
    path5.append(path4);
    EXPECT_EQ(path5.make_preferred(), b::fs::path("right-Süß/right-Süß/unit/Süß").make_preferred());

    b::fs::path path6 = "right-Süß";
    path6 /= "unit/Süß";
    EXPECT_EQ(path6.make_preferred(), b::fs::path("right-Süß/unit/Süß").make_preferred());

    b::fs::path path7 = "right-Süß";
    path7 /= path6;
    EXPECT_EQ(path7.make_preferred(), b::fs::path("right-Süß/right-Süß/unit/Süß").make_preferred());

    b::fs::path path8 = "right-Süß";
    path8.concat("unit/Süß");
    EXPECT_EQ(path8.make_preferred(), b::fs::path("right-Süßunit/Süß").make_preferred());

    b::fs::path path9 = "right-Süß";
    path9.concat(path8);
    EXPECT_EQ(path9.make_preferred(), b::fs::path("right-Süßright-Süßunit/Süß").make_preferred());

    b::fs::path path10 = "right-Süß";
    path10 += "unit/Süß";
    EXPECT_EQ(path10.make_preferred(), b::fs::path("right-Süßunit/Süß").make_preferred());

    b::fs::path path11 = "right-Süß";
    path11 += path10;
    EXPECT_EQ(path11.make_preferred(), b::fs::path("right-Süßright-Süßunit/Süß").make_preferred());
}

TEST(BatteryCore_Filesystem, Path_Clear) {
    b::fs::path path1 = "unit/Süß時";
    path1.clear();
    EXPECT_EQ(path1.string(), "");
}

TEST(BatteryCore_Filesystem, Path_MakePreferred) {
    b::fs::path path1 = "a/very\\comprehensive/Süßöl時\\test.file";
#ifdef B_OS_WINDOWS
    EXPECT_EQ(path1.make_preferred().native_string(), "a\\very\\comprehensive\\Süßöl時\\test.file");
#else
    EXPECT_EQ(path1.make_preferred().native_string(), "a/very\\comprehensive/Süßöl\\test.file");
#endif
}

TEST(BatteryCore_Filesystem, Path_MakeAbsolute) {
    b::fs::path path1 = "unit時/test.file";
    path1.make_absolute();
    EXPECT_TRUE(path1.is_absolute());
}

TEST(BatteryCore_Filesystem, Path_Filenames) {
    b::fs::path path1 = "a/very/long/unit時/test.file";
    path1.remove_filename();
    EXPECT_EQ(path1.string(), "a/very/long/unit時/");

    b::fs::path path2 = "a/very/long/unit/test.file";
    path2.replace_filename("test2時.txt");
    EXPECT_EQ(path2.string(), "a/very/long/unit/test2時.txt");

    b::fs::path path3 = "a/very/long/unit/test.file";
    path3.replace_filename(b::fs::path("cool_filename時.json"));
    EXPECT_EQ(path3.string(), "a/very/long/unit/cool_filename時.json");

    b::fs::path path4 = "a/very/long/unit/test時.file";
    path4.replace_extension("txt");
    EXPECT_EQ(path4.string(), "a/very/long/unit/test時.txt");

    b::fs::path path5 = "a/very/long/unit/test時.file";
    path5.replace_extension(b::fs::path("json"));
    EXPECT_EQ(path5.string(), "a/very/long/unit/test時.json");
}

TEST(BatteryCore_Filesystem, Path_Swap) {
    b::fs::path path1 = "unit/test.file時";
    b::fs::path path2 = "unit/test2.txt";
    path1.swap(path2);
    EXPECT_EQ(path1.string(), "unit/test2.txt");
    EXPECT_EQ(path2.string(), "unit/test.file時");
}

TEST(BatteryCore_Filesystem, Path_String_Native) {
    b::fs::path path1 = "a/very\\comprehensive/Süßöl時\\test.file";
#ifdef B_OS_WINDOWS
    EXPECT_EQ(path1.string(), "a/very/comprehensive/Süßöl時/test.file");
    EXPECT_EQ(path1.native_string(), "a/very\\comprehensive/Süßöl時\\test.file");
#else
    EXPECT_EQ(path1.string(), "a/very\\comprehensive/Süßöl\\test.file");
    EXPECT_EQ(path1.native_string(), "a/very\\comprehensive/Süßöl\\test.file");
#endif
}

TEST(BatteryCore_Filesystem, Path_Comparison) {
    b::fs::path path = "unit/Süßölgefäß時/test.config";
    b::fs::path shorter = "unit/Süßölgefäß時";
    b::fs::path longer = "unit/Süßölgefäß時/test.config/path";
    EXPECT_EQ(path.compare(path), 0);
    EXPECT_GT(path.compare(shorter), 0);
    EXPECT_LT(path.compare(longer), 0);
}

TEST(BatteryCore_Filesystem, Path_lexical) {
    b::fs::path path = "/an/absolute/../unit/Süßöl時/path/../file.config";
    EXPECT_EQ(path.lexically_normal().string(), "/an/unit/Süßöl時/file.config");

    b::fs::path base = "/an/absolute/cool/unit";
    EXPECT_EQ(path.lexically_relative(base).string(), "../../../unit/Süßöl時/path/../file.config");
    EXPECT_EQ(path.lexically_proximate(base).string(), "../../../unit/Süßöl時/path/../file.config");

    b::fs::path path2 = "unit/Süßölgefäß時/test.config";
    EXPECT_EQ(path2.lexically_relative("/unit-test").string(), "");
    EXPECT_EQ(path2.lexically_proximate("/unit-test").string(), "unit/Süßölgefäß時/test.config");
}

TEST(BatteryCore_Filesystem, Path_Root) {
    b::fs::path path = "C:/an/absolute/../unit/Süßöl時/path/../file.config";
    EXPECT_EQ(path.root_name().string(), "C:");
    EXPECT_EQ(path.root_directory().string(), "/");
    EXPECT_EQ(path.root_path().string(), "C:/");
    EXPECT_EQ(path.relative_root_path().string(), "an/absolute/../unit/Süßöl時/path/../file.config");
    EXPECT_EQ(path.parent_path().string(), "C:/an/absolute/../unit/Süßöl時/path/..");
    EXPECT_EQ(path.filename().string(), "file.config");
    EXPECT_EQ(path.stem().string(), "file");
    EXPECT_EQ(path.extension().string(), ".config");
    EXPECT_EQ(path.raw_extension().string(), "config");
}

TEST(BatteryCore_Filesystem, Path_Empty) {
    b::fs::path path = "";
    EXPECT_TRUE(path.empty());
    path = "unit/test.file時";
    EXPECT_FALSE(path.empty());
}

TEST(BatteryCore_Filesystem, Path_Has) {
    EXPECT_TRUE(b::fs::path("C:/unit/test.file時").has_root_path());
    EXPECT_FALSE(b::fs::path("unit/test.file時").has_root_path());

    EXPECT_TRUE(b::fs::path("C:/unit/test.file時").has_root_name());
    EXPECT_FALSE(b::fs::path("unit/test.file時").has_root_name());

    EXPECT_TRUE(b::fs::path("C:/unit/test.file時").has_root_directory());
    EXPECT_FALSE(b::fs::path("unit/test.file時").has_root_directory());

    EXPECT_TRUE(b::fs::path("C:/unit/test.file時").has_relative_root_path());
    EXPECT_FALSE(b::fs::path("C:/").has_relative_root_path());

    EXPECT_TRUE(b::fs::path("C:/unit/test.file時").has_parent_path());
    EXPECT_FALSE(b::fs::path("test.file時").has_parent_path());

    EXPECT_TRUE(b::fs::path("C:/unit/test.file時").has_filename());
    EXPECT_FALSE(b::fs::path("C:/unit時/").has_filename());

    EXPECT_TRUE(b::fs::path("C:/unit/test.file時").has_stem());
    EXPECT_FALSE(b::fs::path("C:/unit時/").has_stem());

    EXPECT_TRUE(b::fs::path("C:/unit/test.file時").has_extension());
    EXPECT_FALSE(b::fs::path("C:/unit/file時").has_extension());
}

TEST(BatteryCore_Filesystem, Path_AbsRel) {
    b::fs::path path = "C:/an/absolute/../unit/Süßöl時/path/../file.config";
    EXPECT_TRUE(path.is_absolute());
    EXPECT_FALSE(path.is_relative());

    path = "an/absolute/../unit/Süßöl時/path/../file.config";
    EXPECT_FALSE(path.is_absolute());
    EXPECT_TRUE(path.is_relative());
}

TEST(BatteryCore_Filesystem, Path_StdPath) {
    b::fs::path path = "C:/an/absolute/../unit/Süßöl時/path/../file.config";
    std::filesystem::path stdPath = path.std_path();
    EXPECT_EQ(stdPath.u8string(), u8"C:/an/absolute/../unit/Süßöl時/path/../file.config");
}

TEST(BatteryCore_Filesystem, Path_Iterators) {
    b::fs::path path = "C:/an/absolute/../unit/Süßöl時/path/../file.config";
    auto it = path.begin();
    EXPECT_EQ(*it, L"C:");
    ++it;
    EXPECT_EQ(*it, L"/");
    ++it;
    EXPECT_EQ(*it, L"an");
    ++it;
    EXPECT_EQ(*it, L"absolute");
    ++it;
    EXPECT_EQ(*it, L"..");
    ++it;
    EXPECT_EQ(*it, L"unit");
    ++it;
    EXPECT_EQ(*it, L"Süßöl時");
    ++it;
    EXPECT_EQ(*it, L"path");
    ++it;
    EXPECT_EQ(*it, L"..");
    ++it;
    EXPECT_EQ(*it, L"file.config");
    ++it;
    EXPECT_EQ(it, path.end());

    // Implicit const
    const b::fs::path path2 = "C:/an/absolute/../unit/Süßöl時/path/../file.config";
    auto it2 = path2.begin();
    EXPECT_EQ(*it2, L"C:");
    ++it2;
    EXPECT_EQ(*it2, L"/");
    ++it2;
    EXPECT_EQ(*it2, L"an");
    ++it2;
    EXPECT_EQ(*it2, L"absolute");
    ++it2;
    EXPECT_EQ(*it2, L"..");
    ++it2;
    EXPECT_EQ(*it2, L"unit");
    ++it2;
    EXPECT_EQ(*it2, L"Süßöl時");
    ++it2;
    EXPECT_EQ(*it2, L"path");
    ++it2;
    EXPECT_EQ(*it2, L"..");
    ++it2;
    EXPECT_EQ(*it2, L"file.config");
    ++it2;
    EXPECT_EQ(it2, path2.end());
}

TEST(BatteryCore_Filesystem, Path_ComparisonOperators) {
    b::fs::path path1 = "unit/test時/file.config時";
    b::fs::path path2 = "unit/test時/file.txt時";

    EXPECT_TRUE(path1 == path1);
    EXPECT_FALSE(path1 == path2);

    EXPECT_FALSE(path1 != path1);
    EXPECT_TRUE(path1 != path2);

    b::fs::path more = "unit/test時/file.config時/more";
    b::fs::path less = "unit/test時/";

    EXPECT_TRUE(path1 > less);
    EXPECT_FALSE(path1 > more);

    EXPECT_TRUE(path1 >= less);
    EXPECT_FALSE(path1 >= more);

    EXPECT_FALSE(path1 < less);
    EXPECT_TRUE(path1 < more);

    EXPECT_FALSE(path1 <= less);
    EXPECT_TRUE(path1 <= more);

    EXPECT_EQ(path1 <=> path1, std::strong_ordering::equal);
    EXPECT_EQ(path1 <=> path2, std::strong_ordering::less);
    EXPECT_EQ(path2 <=> path1, std::strong_ordering::greater);
}

TEST(BatteryCore_Filesystem, GlobalPath_Swap) {
    b::fs::path path1 = "unit/test/file時.config時";
    b::fs::path path2 = "unit/test/file時.txt時";

    b::fs::swap(path1, path2);

    EXPECT_EQ(path1, "unit/test/file時.txt時");
    EXPECT_EQ(path2, "unit/test/file時.config時");
}

TEST(BatteryCore_Filesystem, GlobalPath_Operators) {
    b::fs::path path1 = "unit/test/file年.config時";
    b::fs::path path2 = "unit/test/file年.txt";

    EXPECT_EQ(path1 / path2, "unit/test/file年.config時/unit/test/file年.txt");
    EXPECT_EQ(path1 + path2, "unit/test/file年.config時unit/test/file年.txt");
}

// ========================================================
// =================== End path class =====================
// ========================================================


TEST(BatteryCore_Filesystem, AbsoluteCanonical) {
    b::fs::path path = "unit時/test学/../Süßöl年.config";
    auto abs = b::fs::absolute(path);
    EXPECT_EQ(abs, b::fs::current_path() / "unit時/Süßöl年.config");

    b::fs::create_directory(abs);
    EXPECT_EQ(b::fs::canonical(path), b::fs::current_path() / "unit時/Süßöl年.config");
    b::fs::remove("unit時");
}

TEST(BatteryCore_Filesystem, TryAbsoluteCanonical) {
    b::fs::path path = "unit時/test学/../Süßöl年.config";
    auto abs = b::fs::try_absolute(path).value();
    EXPECT_EQ(abs, b::fs::current_path() / "unit時/Süßöl年.config");

    b::fs::create_directory(abs);
    EXPECT_EQ(b::fs::try_canonical(path).value(), b::fs::current_path() / "unit時/Süßöl年.config");
    b::fs::remove("unit時");
}

TEST(BatteryCore_Filesystem, RelativeProximate) {
    b::fs::path path1 = "/unit学/test時/../Süßöl年.config";
    EXPECT_EQ(b::fs::relative(path1, "/unit学/base").string(), "../Süßöl年.config");
    EXPECT_EQ(b::fs::proximate(path1, "/unit学/base").string(), "../Süßöl年.config");
    EXPECT_EQ(b::fs::relative("C:/時/b", "年").string(), "");
    EXPECT_EQ(b::fs::proximate("C:/時/b", "年").string(), "C:/時/b");
}

TEST(BatteryCore_Filesystem, TryRelativeProximate) {
    b::fs::path path1 = "/unit学/test時/../Süßöl年.config";
    EXPECT_EQ(b::fs::try_relative(path1, "/unit学/base").value().string(), "../Süßöl年.config");
    EXPECT_EQ(b::fs::try_proximate(path1, "/unit学/base").value().string(), "../Süßöl年.config");
    EXPECT_EQ(b::fs::try_relative("C:/時/b", "年").value().string(), "");
    EXPECT_EQ(b::fs::try_proximate("C:/時/b", "年").value().string(), "C:/時/b");
}

TEST(BatteryCore_Filesystem, CopyRemove) {
    b::fs::remove(TESTDATA_PATH / "国 分 高");
    b::fs::copy(TESTDATA_PATH / "年 本 Süßölgefäß 国 分 高",
                TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高");
    EXPECT_TRUE(b::fs::is_regular_file(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高"));
    b::fs::remove(TESTDATA_PATH / "国 分 高");
    EXPECT_FALSE(b::fs::exists(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高"));
}

TEST(BatteryCore_Filesystem, TryCopyRemove) {
    auto result = b::fs::try_remove(TESTDATA_PATH / "国 分 高");
    EXPECT_TRUE(result);
    // We do not check the result itself

    EXPECT_TRUE(b::fs::try_copy(TESTDATA_PATH / "年 本 Süßölgefäß 国 分 高",
                            TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高"));

    result = b::fs::try_is_regular_file(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高");
    EXPECT_TRUE(result);
    EXPECT_TRUE(result.value());

    result = b::fs::try_remove(TESTDATA_PATH / "国 分 高");
    EXPECT_TRUE(result);
    EXPECT_EQ(result.value(), 2);

    result = b::fs::try_exists(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高");
    EXPECT_TRUE(result);
    EXPECT_FALSE(result.value());
}

TEST(BatteryCore_Filesystem, CopyFileRemove) {
    b::fs::remove(TESTDATA_PATH / "国 分 高");
    b::fs::copy_file(TESTDATA_PATH / "年 本 Süßölgefäß 国 分 高",
                TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高");
    EXPECT_TRUE(b::fs::is_regular_file(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高"));
    b::fs::remove(TESTDATA_PATH / "国 分 高");
    EXPECT_FALSE(b::fs::exists(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高"));
}

TEST(BatteryCore_Filesystem, TryCopyFileRemove) {
    auto result = b::fs::try_remove(TESTDATA_PATH / "国 分 高");
    EXPECT_TRUE(result);
    // We do not check the result itself

    EXPECT_TRUE(b::fs::try_copy_file(TESTDATA_PATH / "年 本 Süßölgefäß 国 分 高",
                                TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高"));

    result = b::fs::try_is_regular_file(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高");
    EXPECT_TRUE(result);
    EXPECT_TRUE(result.value());

    result = b::fs::try_remove(TESTDATA_PATH / "国 分 高");
    EXPECT_TRUE(result);
    EXPECT_EQ(result.value(), 2);

    result = b::fs::try_exists(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高");
    EXPECT_TRUE(result);
    EXPECT_FALSE(result.value());
}

// It seems like we cannot unit-test symlinks since they require administrative privileges

//TEST(BatteryCore_Filesystem, CreateCopyRemoveSymlink) {
//    b::fs::remove(TESTDATA_PATH / "国 分 高");
//
//    // Create a symlink
//    b::fs::create_symlink(TESTDATA_PATH / "年 本 Süßölgefäß 国 分 高",
//                          TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高");
//
//    // Check if it exists
//    EXPECT_TRUE(b::fs::is_symlink(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高"));
//
//    // Copy the symlink
//    b::fs::copy(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高",
//                TESTDATA_PATH / "国 分 高" / "年 本" / "年 本 Süßölgefäß 国 分 高");
//
//    // Check if it exists
//    EXPECT_TRUE(b::fs::is_symlink(TESTDATA_PATH / "国 分 高" / "年 本" / "年 本 Süßölgefäß 国 分 高"));
//
//    // Read the symlink
//    EXPECT_EQ(b::fs::read_symlink(TESTDATA_PATH / "国 分 高" / "年 本" / "年 本 Süßölgefäß 国 分 高").string(),
//              "年 本 Süßölgefäß 国 分 高");
//
//    // Read symlink status
//    auto status = b::fs::symlink_status(TESTDATA_PATH / "国 分 高" / "年 本" / "年 本 Süßölgefäß 国 分 高");
//    EXPECT_TRUE(b::fs::is_symlink(status));
//
//    // Create a directory symlink
//    b::fs::create_directory_symlink(TESTDATA_PATH,
//                                    TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高" / "年 本");
//
//    // Read the symlink
//    EXPECT_EQ(b::fs::read_symlink(TESTDATA_PATH / "国 分 高" / "年 本" / "年 本"), TESTDATA_PATH);
//
//    // Remove the symlink
//    b::fs::remove(TESTDATA_PATH / "国 分 高" / "年 本" / "年 本");
//    EXPECT_FALSE(b::fs::is_symlink(TESTDATA_PATH / "国 分 高" / "年 本" / "年 本"));
//
//    // Remove the directory symlink
//    b::fs::remove(TESTDATA_PATH / "国 分 高" / "年 本");
//    EXPECT_FALSE(b::fs::is_symlink(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高"));
//
//    // Remove the entire folder
//    b::fs::remove(TESTDATA_PATH / "国 分 高");
//}
//
//TEST(BatteryCore_Filesystem, TryCreateCopyRemoveSymlink) {
//    auto result = b::fs::try_remove(TESTDATA_PATH / "国 分 高");
//    EXPECT_TRUE(result);
//    // We do not check the result itself
//
//    // Create a symlink
//    EXPECT_TRUE(b::fs::try_create_symlink(TESTDATA_PATH / "年 本 Süßölgefäß 国 分 高",
//                                          TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高"));
//
//    // Check if it exists
//    EXPECT_TRUE(b::fs::try_is_symlink(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高"));
//
//    // Copy the symlink
//    EXPECT_TRUE(b::fs::try_copy(TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高",
//                                TESTDATA_PATH / "国 分 高" / "年 本" / "年 本 Süßölgefäß 国 分 高"));
//
//    // Check if it exists
//    EXPECT_TRUE(b::fs::try_is_symlink(TESTDATA_PATH / "国 分 高" / "年 本" / "年 本 Süßölgefäß 国 分 高"));
//
//    // Read the symlink
//    EXPECT_EQ(b::fs::try_read_symlink(TESTDATA_PATH / "国 分 高" / "年 本" / "年 本 Süßölgefäß 国 分 高").value(),
//              "年 本 Süßölgefäß 国 分 高");
//
//    // Read symlink status
//    auto status = b::fs::try_symlink_status(TESTDATA_PATH / "国 分 高" / "年 本" / "年 本 Süßölgefäß 国 分 高");
//    EXPECT_TRUE(b::fs::is_symlink(status.value()));
//
//    // Create a directory symlink
//    EXPECT_TRUE(b::fs::try_create_directory_symlink(TESTDATA_PATH,
//                                                    TESTDATA_PATH / "国 分 高" / "年 本 Süßölgefäß 国 分 高" / "年 本"));
//}
