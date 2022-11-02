//
// Created by User on 2022-01-02.
//

#ifndef ECS_POSITION_COMPONENT_H
#define ECS_POSITION_COMPONENT_H

#include <cstdint>

#include "component.h"

struct position : public ecs::component<position> {
    uint32_t x, y;

    position() = default;
    position(uint32_t t_x, uint32_t t_y) : x(t_x), y(t_y) {}
};

#endif //ECS_POSITION_COMPONENT_H
