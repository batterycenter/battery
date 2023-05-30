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
            auto str = is_binary ? b::fs::read_binary_file_nothrow(resource_path) : b::fs::read_text_file_nothrow(resource_path);
            if (!str.has_value()) {
                b::log::error("Failed to hot-reload resource '{}': {}", resource_path, strerror(errno));
                return;
            }

            b::log::info("Hot-reloaded resource '{}'", resource_path);
            callback(b::resource::from_byte_string(str.value(), resource_path.raw_extension()));
        }

        b::fs::path resource_path;
        bool is_binary {};
        std::function<void(b::resource)> callback;
        std::unique_ptr<b::file_watcher> file_watcher;
#endif
    };

}