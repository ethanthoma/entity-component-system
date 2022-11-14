//
// Created by User on 11/13/2022.
//

#ifndef ECS_COMMAND_H
#define ECS_COMMAND_H

#include <utility>

#include "archetype.h"

namespace ecs {
    class command {
        virtual void execute();
    };

    template<class Component>
    class add_component : public command {
    private:
        uint32_t index_;
        std::shared_ptr<archetype> from_;
        std::shared_ptr<archetype> to_;
        Component c_;
    public:
        add_component(uint32_t index,
                      std::shared_ptr<archetype> from,
                      std::shared_ptr<archetype> to,
                      Component c) : c_(c) {}

        void execute() {
            auto list_o_c = from_->get(index_);

        }
    };
}

#endif //ECS_COMMAND_H
