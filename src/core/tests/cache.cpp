
#include "battery/battery.hpp"
#include <gtest/gtest.h>

//TEST(BatteryCore_Cache, Cache) {
//    auto path = b::Folders::AppCacheDir() / "unit_test.cache"_b;
//    if (b::fs::exists(path)) {
//        b::fs::remove(path);
//    }
//    {
//        b::cachefile cache(path);
//        cache["boolean"] = true;
//        cache["integer"] = 187;
//        cache["double"] = 0.5;
//        cache["string"] = "unit test";
//        nlohmann::json data;
//        data["test1"] = 5;
//        data["test2"] = "test";
//        cache["object"] = data;
//    }
//    {
//        b::cachefile cache(path);
//        EXPECT_EQ((bool)cache["boolean"], true);
//        EXPECT_EQ((int)cache["integer"], 187);
//        EXPECT_EQ((double)cache["double"], 0.5);
//        std::string str = cache["string"];
//        EXPECT_EQ(str, "unit test");
//        nlohmann::json data;
//        data["test1"] = 5;
//        data["test2"] = "test";
//        nlohmann::json result = cache["object"];
//        EXPECT_EQ(result.dump(4), data.dump(4));
//    }
//    b::fs::remove(path);
//}

// TODO Fix this test
