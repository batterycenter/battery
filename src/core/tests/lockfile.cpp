
#include "battery/core/main.hpp"
#include "battery/core/folders.hpp"
#include "battery/core/fs.hpp"
#include "battery/core/log.hpp"
#include "battery/core/lockfile.hpp"
#include <gtest/gtest.h>

TEST(BatteryCore_Lockfile, lockfile) {
    auto parent = b::folders::get_cache() + "unit_test_folder";
    auto path = parent + "unit_test.lock";
    if (b::fs::exists(parent)) {
        b::fs::remove_all(parent);
    }

    {
        b::lockfile lock1(path);
        EXPECT_FALSE(lock1.is_locked());
        EXPECT_FALSE(lock1);

        lock1.lock();
        EXPECT_TRUE(lock1.is_locked());
        EXPECT_TRUE(lock1);

        lock1.unlock();
        EXPECT_FALSE(lock1.is_locked());
        EXPECT_FALSE(lock1);

        lock1.try_lock();
        EXPECT_TRUE(lock1.is_locked());
        EXPECT_TRUE(lock1);

        lock1.timeout = 1.0;
        EXPECT_ANY_THROW(lock1.lock());
        EXPECT_FALSE(lock1.lock(true));
        lock1.timeout = {};

        lock1.unlock();
        EXPECT_FALSE(lock1.is_locked());
        EXPECT_FALSE(lock1);

        EXPECT_NO_THROW(lock1.try_lock());
        EXPECT_ANY_THROW(lock1.try_lock());
        lock1.unlock();

        EXPECT_TRUE(lock1.try_lock(true));
        EXPECT_FALSE(lock1.try_lock(true));
        lock1.unlock();
        EXPECT_TRUE(lock1.try_lock(true));

        EXPECT_TRUE(lock1.is_locked());
        EXPECT_TRUE(lock1);
    }

    b::fs::remove_all(parent);
}

TEST(BatteryCore_Lockfile, ScopedLockfile) {
    auto parent = b::folders::get_cache() + "unit_test_folder";
    auto path = parent + "unit_test.lock";
    if (b::fs::exists(parent)) {
        b::fs::remove_all(parent);
    }

    EXPECT_NO_THROW({ b::scoped_lockfile lock1(path); });         // Lock once and unlock
    EXPECT_NO_THROW({ b::scoped_lockfile lock2(path); });         // And lock a second time without deleting file

    EXPECT_ANY_THROW({
        b::scoped_lockfile lock3(path);
        b::scoped_lockfile lock4(path);        // Now it should throw
    });

    EXPECT_ANY_THROW({
         b::scoped_lockfile lock5(path);
         b::scoped_lockfile lock6(path);        // Now it should throw
    });

    EXPECT_NO_THROW({ b::scoped_lockfile lock7(path); });

    b::fs::remove_all(parent);
}

TEST(BatteryCore_Lockfile, ScopedNoThrowLockfile) {
    auto parent = b::folders::get_cache() + "unit_test_folder";
    auto path = parent + "unit_test.lock";
    if (b::fs::exists(parent)) {
        b::fs::remove_all(parent);
    }

    {
        b::scoped_lockfile_nothrow lock1(path);
        EXPECT_TRUE(static_cast<bool>(lock1));
    }

    {
        b::scoped_lockfile_nothrow lock2(path);
        EXPECT_TRUE(static_cast<bool>(lock2));
    }

    {
         b::scoped_lockfile_nothrow lock3(path);
         b::scoped_lockfile_nothrow lock4(path);
         EXPECT_TRUE(static_cast<bool>(lock3));
         EXPECT_FALSE(static_cast<bool>(lock4));
    }

    {
         b::scoped_lockfile_nothrow lock5(path);
         b::scoped_lockfile_nothrow lock6(path);
         EXPECT_TRUE(static_cast<bool>(lock5));
         EXPECT_FALSE(static_cast<bool>(lock6));
    }

    {
        b::scoped_lockfile_nothrow lock7(path);
        EXPECT_TRUE(static_cast<bool>(lock7));
    }

    b::fs::remove_all(parent);
}
