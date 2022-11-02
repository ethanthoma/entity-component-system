//
// Created by User on 2022-01-02.
//

#ifndef ECS_ENTITY_MANAGER_H
#define ECS_ENTITY_MANAGER_H

#include <vector>
#include <queue>

#include "entity.h"

namespace ecs {
    class entity_manager {
    private:
        uint32_t entity_count_ = 0;
        std::vector<entity> loe_;
        std::vector<entity> to_free_;
        std::queue<uint32_t> free_id_;
    public:
        entity_manager() = default;
        entity make();
        void free(entity e);
        void update();
    };
} // namespace ecs

#endif //ECS_ENTITY_MANAGER_H
