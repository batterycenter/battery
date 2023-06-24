
#include "battery/core.hpp"
#include <gtest/gtest.h>

class App : public b::console_application {
public:
    App() = default;

    double test_framerate { 100 };
    double sum_framerate { 0 };
    size_t max_frames { 0 };

    void console_setup() override {
        setup_called = true;
        set_requested_framerate(test_framerate);
        max_frames = static_cast<size_t>(test_framerate * 2);
    }

    void console_update() override {
        EXPECT_EQ(this->framerate, this->frametime > 0.0 ? 1.0 / this->frametime : 0.0);

        std::vector<b::string> expected_args = {u8"ABC", u8"DEF", u8"GHI"};
        EXPECT_EQ(this->args, expected_args);

        auto elapsed = b::time() - last_time;
        last_time = b::time();
        if (framecount > 2) {
            EXPECT_GT(frametime, elapsed * 0.1);
            EXPECT_LT(frametime, elapsed * 10.0);

            EXPECT_GT(framerate, test_framerate * 0.1);
            EXPECT_LT(framerate, test_framerate * 10.0);

            EXPECT_GT(elapsed, 1.0 / test_framerate * 0.1);
            EXPECT_LT(elapsed, 1.0 / test_framerate * 10.0);
        }

        EXPECT_EQ(updated, framecount);
        updated++;

        sum_framerate += framerate;
        if (updated >= max_frames) {     // Run for 2 seconds
            this->stop_application();
        }
    }

    void console_cleanup() override {
        cleaned_called = true;
    }

    double last_time = 0.0;
    bool setup_called = false;
    bool cleaned_called = false;
    int updated = 0;
};

TEST(BatteryCore_Application, Application) {
    App app;
    app.run(u8"BatteryCore_UnitTest", { u8"ABC", u8"DEF", u8"GHI" });
    EXPECT_TRUE(app.setup_called);
    EXPECT_TRUE(app.cleaned_called);
    EXPECT_EQ(app.updated, app.max_frames);
    EXPECT_EQ(app.framecount, app.max_frames);
    EXPECT_GT(app.sum_framerate / app.max_frames, app.test_framerate * 0.95);
    EXPECT_LT(app.sum_framerate / app.max_frames, app.test_framerate * 1.05);
}