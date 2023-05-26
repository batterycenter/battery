#pragma once

#include <string>
#include <utility>
#include "battery/core/fs.hpp"
#include "battery/core/log.hpp"
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
            entry(cachefile& parent, b::string key) : parent(parent), key(std::move(key)) {}

            operator nlohmann::json() { return parent.get<nlohmann::json>(key); }
            operator b::string() { return parent.get<std::string>(key); }

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
            b::string key;
        };

        entry operator[](const b::string& key) {
            return entry(*this, key);
        }

        template<typename T>
        T get(const b::string& key) {
            b::fs::ifstream file(filename);
            if (file.fail()) {
                throw std::runtime_error(b::format("b::cachefile: Failed to open file '{}' for reading: ", filename, strerror(errno)));
            }
            json = nlohmann::json::parse(file.read_string().value());
            return json[key.str()];
        }

        template<typename T>
        void set(const b::string& key, T value) {
            b::fs::ifstream in(filename);
            if (in.fail()) {
                throw std::runtime_error(b::format("b::cachefile: Failed to open file '{}' for reading: ", filename, strerror(errno)));
            }
            json = nlohmann::json::parse(in.read_string().value());
            json[key] = value;
            b::fs::ofstream file(filename);
            if (file.fail()) {
                throw std::runtime_error(b::format("b::cachefile: Failed to open file '{}' for reading: ", filename, strerror(errno)));
            }
            file << json.dump(4);
        }

    private:
        b::fs::path filename;
        nlohmann::json json;
    };

}