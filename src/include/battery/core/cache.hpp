#pragma once

#include <string>
#include <utility>
#include "battery/core/fs.hpp"
#include "battery/core/extern/json.hpp"

namespace b {

    // This class will read a file, or create it if it does not exist, but never delete it.
    // It stores key-value pairs in the form of a json file.
    class cachefile {
    public:
        cachefile();
        cachefile(const b::fs::path& filename);

        class entry {
        public:
            entry(cachefile& parent, std::string key) : parent(parent), key(std::move(key)) {}

            operator nlohmann::json() { return parent.get<nlohmann::json>(key); }
            operator std::string() { return parent.get<std::string>(key); }

            template<typename T>
            operator T() {
                return parent.get<T>(key);
            }

            template<typename T>
            entry& operator=(T j) {
                parent.set<T>(key, j);
                return *this;
            }

        private:
            cachefile& parent;
            std::string key;
        };

        entry operator[](const std::string& key) {
            return entry(*this, key);
        }

        template<typename T>
        T get(const std::string& key) {
            b::fs::ifstream file(filename);
            if (file.fail()) {
                throw std::runtime_error("b::cachefile: Failed to open file '" + b::u8_as_str(filename.u8string()) + "' for reading: " + strerror(errno));
            }
            json = nlohmann::json::parse(file.read_string().value());
            return json[key];
        }

        template<typename T>
        void set(const std::string& key, T value) {
            b::fs::ifstream in(filename);
            if (in.fail()) {
                throw std::runtime_error("b::cachefile: Failed to open file '" + b::u8_as_str(filename.u8string()) + "' for reading: " + strerror(errno));
            }
            json = nlohmann::json::parse(in.read_string().value());
            json[key] = value;
            b::fs::ofstream file(filename);
            if (file.fail()) {
                throw std::runtime_error("b::cachefile: Failed to open file '" + b::u8_as_str(filename.u8string()) + "' for writing: " + strerror(errno));
            }
            file << json.dump(4);
        }

    private:
        b::fs::path filename;
        nlohmann::json json;
    };

}