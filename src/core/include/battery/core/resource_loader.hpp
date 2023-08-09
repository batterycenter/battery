#pragma once

#include "battery/core/file_watcher.hpp"
#include "battery/core/resource.hpp"

namespace b {

    class ResourceLoader {
    public:
        ResourceLoader() = default;

#ifndef B_PRODUCTION_MODE     // Non-production mode
        template<typename T>
        ResourceLoader(const T& resource, const std::function<void(b::Resource)>& callback, double poll_interval = 0.1)
            : m_resourcePath(resource.filepath()),
              m_isBinary(resource.isBinary()),
              m_fileWatcher(std::make_unique<b::FileWatcher>(resource.filepath(), [this]() { this->onChangeCallback(); }, poll_interval)),
              m_callback(callback) {}
#else                               // Production mode
        template<typename T>
        resource_loader(const T& resource, const std::function<void(b::resource)>& callback, double poll_interval = 0.1) {
            callback(static_cast<b::resource>(resource));
        }
#endif

        ResourceLoader(const ResourceLoader&) = delete;
        ResourceLoader& operator=(const ResourceLoader&) = delete;
        ResourceLoader(ResourceLoader&&) = delete;
        ResourceLoader& operator=(ResourceLoader&&) = delete;

    private:
#ifndef B_PRODUCTION_MODE     // Non-production mode
        void onChangeCallback() {      // On every change, reload the resource
            auto str = m_isBinary ? b::fs::read_binary_file_nothrow(m_resourcePath) : b::fs::read_text_file_nothrow(m_resourcePath);
            if (!str.has_value()) {
                b::log::error("Failed to hot-reload resource '{}': {}", m_resourcePath, strerror(errno));
                return;
            }

            b::log::info("Hot-reloaded resource '{}'", m_resourcePath);
            m_callback(b::Resource::FromBytes(str.value().encode_utf8(), m_resourcePath.raw_extension().string()));
        }

        b::fs::path m_resourcePath;
        bool m_isBinary {};
        std::function<void(b::Resource)> m_callback;
        std::unique_ptr<b::FileWatcher> m_fileWatcher;
#endif
    };

}