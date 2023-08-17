
#include "battery/core/uuid.hpp"
#include "battery/core/extern/uuidv4.h"

#include <iostream>

namespace b::uuid {

    class uuid_data {
    public:

        static std::shared_ptr<uuids::uuid_random_generator> get() {
            static uuid_data instance;
            return instance.gen;
        }

        uuid_data(const uuid_data&) = delete;
        uuid_data(uuid_data&&) = delete;
        uuid_data& operator=(const uuid_data&) = delete;
        uuid_data& operator=(uuid_data&&) = delete;

    private:
        std::random_device rd;
        std::mt19937 generator;
        std::shared_ptr<uuids::uuid_random_generator> gen;

        uuid_data() {
            auto seed_data = std::array<int, std::mt19937::state_size> {};
            std::generate(std::begin(seed_data), std::end(seed_data), std::ref(rd));
            std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
            generator = std::mt19937(seq);
            gen = std::make_shared<uuids::uuid_random_generator>(generator);
        };
    };

    b::string v4() {
        auto gen = uuid_data::get();
        auto uuid = gen->operator()();
        return b::string::decode<b::enc::utf8>(to_string(uuid));
    }

    bool is_valid(const b::string& uuid) {
        auto opt = uuids::uuid::from_string(uuid.encode<b::enc::utf8>());
        return opt.has_value();
    }

} // namespace b::uuid
