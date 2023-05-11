#pragma once

#include "battery/core/file_watcher.hpp"
#include "battery/core/resource.hpp"

namespace b {

    class resource_loader {
    public:
        resource_loader() = default;

#ifndef BATTERY_PRODUCTION_MODE     // Non-production mode
        template<typename T>
        resource_loader(const T& resource, const std::function<void(b::resource)>& callback, float poll_interval = 0.1f)
            : resource_path(resource.filepath()),
              is_binary(resource.is_binary()),
              file_watcher(std::make_unique<b::file_watcher>(resource.filepath(), [this]() { this->onchange_callback(); }, poll_interval)),
              callback(callback) {}
#else                               // Production mode
        template<typename T>
        resource_loader(const T& resource, const std::function<void(b::resource)>& callback, float poll_interval = 0.1f) {
            callback(static_cast<b::resource>(resource));
        }
#endif

        resource_loader(const resource_loader&) = delete;
        resource_loader& operator=(const resource_loader&) = delete;
        resource_loader(resource_loader&&) = delete;
        resource_loader& operator=(resource_loader&&) = delete;

    private:
#ifndef BATTERY_PRODUCTION_MODE     // Non-production mode
        void onchange_callback() {      // On every change, reload the resource
            auto file = b::fs::ifstream(resource_path, is_binary ? b::fs::Mode::BINARY : b::fs::Mode::TEXT);
            auto str = file.read_string();
            if (str.has_value()) {
                callback(b::resource::from_byte_string(str.value(), b::u8_as_str(resource_path.raw_extension().u8string())));
            }
            else {
                b::log::error("Failed to hot-reload resource '{}': {}", b::u8_as_str(resource_path.u8string()), strerror(errno));
            }
        }

        b::fs::path resource_path;
        bool is_binary {};
        std::function<void(b::resource)> callback;
        std::unique_ptr<b::file_watcher> file_watcher;
#endif
    };

}