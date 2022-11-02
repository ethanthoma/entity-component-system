//
// Created by User on 2022-01-02.
//

#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include <cstdint>

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
} // namespace ecs

#endif //ECS_ENTITY_H
