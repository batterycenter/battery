
#include "battery/core/all.hpp"
#include <gtest/gtest.h>

TEST(BatteryCore_Uuid, is_valid) {
    auto uuid = b::uuid::v4();
    EXPECT_TRUE(b::uuid::is_valid(uuid));
}

TEST(BatteryCore_Uuid, is_not_valid) {
    EXPECT_FALSE(b::uuid::is_valid("not-a-valid-uuid"));
}

TEST(BatteryCore_Uuid, different) {
    std::vector<std::string> uuids;
    for (int i = 0; i < 100; ++i) {
        uuids.push_back(b::uuid::v4());
    }
    // Check if all uuids are unique
    std::sort(uuids.begin(), uuids.end());
    EXPECT_EQ(std::unique(uuids.begin(), uuids.end()), uuids.end());
}
