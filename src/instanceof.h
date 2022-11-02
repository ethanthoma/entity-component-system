//
// Created by User on 2022-01-05.
//

#ifndef ECS_INSTANCEOF_H
#define ECS_INSTANCEOF_H

#include <cstdint>

#include "component.h"

namespace ecs {
    struct instanceof : public ecs::component<instanceof> {
        uint32_t i;

        instanceof() = default;
        explicit instanceof(uint32_t t_i) : i(t_i) {}
    };
}
#endif //ECS_INSTANCEOF_H
