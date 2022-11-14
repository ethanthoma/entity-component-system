//
// Created by ethanthoma on 2022-01-02.
//

#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include <cstdint>
#include <memory>

namespace ecs {
    struct entity {
        uint32_t id_ = 0;

        entity() = default;
        explicit entity(uint32_t t_id) : id_(t_id) {}

        bool operator==(const entity & e) const {
            return id_ == e.id_;
        }
    };

    struct entity_hash {
        std::size_t operator()(entity const &e) const noexcept {
            return std::hash<uint32_t>{}(e.id_) + 0x9e3779b9;
        }
    };

    struct entity_batch {
        uint32_t size_ = 0;
        std::unique_ptr<entity> arr_;

        explicit entity_batch(uint32_t t_size) : size_(t_size), arr_(std::make_unique<entity>(t_size)) {}

        // order preserving
        bool operator==(const entity_batch & eb) const noexcept {
            bool is_equal = true;
            for (uint32_t i = 0; i != size_ && is_equal; ++i) {
                is_equal &= arr_.get()[i] == eb.arr_.get()[i];
            }
            return is_equal;
        }
    };
} // namespace ecs

#endif //ECS_ENTITY_H
