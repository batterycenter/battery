
#include "battery/battery.hpp"
#include <gtest/gtest.h>

class App : public b::ConsoleApplication {
public:
    App() = default;

    double testFramerate { 100 };
    double sumFramerate { 0 };
    size_t maxFrames { 0 };
    bool setupCalled = false;
    bool cleanedCalled = false;
    int updated = 0;

    void onConsoleSetup() override {
        setupCalled = true;
        setRequestedFramerate(testFramerate);
        maxFrames = static_cast<size_t>(testFramerate * 2);
    }

    void onConsoleUpdate() override {
        EXPECT_EQ(this->framerate, this->frametime > 0.0 ? 1.0 / this->frametime : 0.0);

        std::vector<std::string> expected_args = { "ABC", "DEF", "GHI" };
        EXPECT_EQ(this->args, expected_args);

        auto elapsed = b::time() - m_lastTime;
        m_lastTime = b::time();
        if (framecount > 2) {
            EXPECT_GT(frametime, elapsed * 0.1);
            EXPECT_LT(frametime, elapsed * 10.0);

            EXPECT_GT(framerate, testFramerate * 0.1);
            EXPECT_LT(framerate, testFramerate * 10.0);

            EXPECT_GT(elapsed, 1.0 / testFramerate * 0.1);
            EXPECT_LT(elapsed, 1.0 / testFramerate * 10.0);
        }

        EXPECT_EQ(updated, framecount);
        updated++;

        sumFramerate += framerate;
        if (updated >= maxFrames) {     // Run for 2 seconds
            stopApplication();
        }
    }

    void onConsoleExit() override {
        cleanedCalled = true;
    }

private:
    double m_lastTime = 0.0;
};

TEST(BatteryCore_Application, Application) {
    App app;
    (void)app.run("BatteryCore_UnitTest", { "ABC", "DEF", "GHI" });
    EXPECT_TRUE(app.setupCalled);
    EXPECT_TRUE(app.cleanedCalled);
    EXPECT_EQ(app.updated, app.maxFrames);
    EXPECT_EQ(app.framecount, app.maxFrames);
    EXPECT_GT(app.sumFramerate / app.maxFrames, app.testFramerate * 0.95);
    EXPECT_LT(app.sumFramerate / app.maxFrames, app.testFramerate * 1.05);
}