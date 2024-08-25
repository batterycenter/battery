#pragma once

#include "battery/extern/json.hpp"
#include "fs.hpp"
#include "log.hpp"
#include <string>
#include <utility>

namespace b {

// This class will read a file, or create it if it does not exist, but never delete it.
// It stores key-value pairs in the form of a json file.
class cachefile {
public:
    cachefile();
    cachefile(const b::fs::path& filename);

    class entry {
    public:
        entry(cachefile& parent, std::string key)
            : parent(parent)
            , key(std::move(key))
        {
        }

        operator nlohmann::json()
        {
            return parent.get<nlohmann::json>(key);
        }

        template <typename T> operator T()
        {
            return parent.get<T>(key);
        }

        template <typename T> entry& operator=(T j)
        {
            parent.set<T>(key, j);
            return *this;
        }

    private:
        cachefile& parent;
        std::string key;
    };

    entry operator[](const std::string& key)
    {
        return entry(*this, key);
    }

    template <typename T> T get(const std::string& key)
    {
        auto json = load();
        return json[key];
    }

    template <typename T> void set(const std::string& key, T value)
    {
        auto json = load();
        json[key] = value;
        b::fs::write(filename, json.dump(4));
    }

    bool contains(const std::string& key)
    {
        auto json = load();
        return json.contains(key);
    }

private:
    nlohmann::json load()
    {
        auto file = b::fs::read(filename);
        return nlohmann::json::parse(file);
    }

private:
    b::fs::path filename;
};

} // namespace b
