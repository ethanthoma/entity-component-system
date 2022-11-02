//
// Created by User on 2022-01-02.
//

#include "entity_manager.h"

ecs::entity ecs::entity_manager::make() {
    entity e;

    if (!free_id_.empty()) {
        e.id_ = free_id_.front();
        free_id_.pop();
    } else {
        e.id_ = entity_count_++;
    }

    loe_.push_back(e);
    return e;
}

void ecs::entity_manager::free(entity e) {
    to_free_.push_back(e);
}

void ecs::entity_manager::update() {
    for (const auto & e : to_free_) {
        free_id_.push(e.id_);
    }
    to_free_.clear();
}