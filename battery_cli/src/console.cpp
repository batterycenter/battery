
#include "battery/core/all.hpp"
#include "console.h"

static std::unique_ptr<indicators::ProgressBar> make_option(const std::string& label) {
    namespace i = ::indicators;
    return std::unique_ptr<i::ProgressBar>(new i::ProgressBar{
            i::option::BarWidth{2},
            i::option::Start{"  "},
            i::option::Fill{">"},
            i::option::End{""},
            i::option::ForegroundColor{i::Color::white},
            i::option::PostfixText{label},
            i::option::FontStyles{std::vector<i::FontStyle>{i::FontStyle::bold}}
    });
}

int ask_user_options(const std::vector<std::string>& options) {
    namespace i = ::indicators;
    std::vector<std::unique_ptr<i::ProgressBar>> progressBars;
    i::DynamicProgress<i::ProgressBar> bars;

    for (const auto& opt : options) {
        auto obj = make_option(opt);
        progressBars.push_back(make_option(opt));
        bars.push_back(*(progressBars.back()));
    }

    std::atomic<int> item = 0;
    std::atomic<bool> done = false;
    b::console::terminal terminal;
    b::thread thread([&done,&bars,&options,&item]{
        while (!done) {
            for (int i = 0; i < options.size(); i++) {
                bars[i].set_progress(100);
                if (i == item) {
                    bars[i].set_option(i::option::ForegroundColor {i::Color::cyan});
                    bars[i].set_option(i::option::Fill {">"});
                }
                else {
                    bars[i].set_option(i::option::ForegroundColor {i::Color::white});
                    bars[i].set_option(i::option::Fill {" "});
                }
            }
            b::sleep_ms(50);
        }
    });

    while (!done) {
        auto action = terminal.get_control_key();
        if (action == b::console::keycode::ENTER) {
            done = true;
        }
        if (action == b::console::keycode::UP) {
            item--;
            if (item < 0) item = static_cast<int>(options.size()) - 1;
        }
        if (action == b::console::keycode::DOWN) {
            item++;
            if (item >= options.size()) item = 0;
        }
    }

    thread.join();

    return item;
}
