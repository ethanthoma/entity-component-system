//
// Created by User on 11/13/2022.
//

#ifndef ECS_ARCHETYPE_H
#define ECS_ARCHETYPE_H

#include <vector>
#include <memory>
#include <queue>
#include <map>

#include "bitmask.h"
#include "component_container.h"
#include "command.h"
#include "component.h"
#include "either.h"

namespace ecs {
    class archetype {
    private:
        typedef uint32_t component_index;

        ecs::bitmask b_;
        std::map<component_index, std::shared_ptr<ecs::i_component_container>> containers_;
        std::queue<command> command_queue_;
        uint32_t init_ = 0;
        uint32_t count_ = 0;
    public:
        explicit archetype(const ecs::bitmask &b) : b_(b) {}

        auto get(uint32_t index) {
            struct key_value { component_index id{}; std::unique_ptr<char[]> element; };
            struct key_value_arr {std::unique_ptr<key_value> arr; uint32_t size; };

            auto retval = new key_value[b_.size()];
            uint32_t i;
            b_.foreach([this, index, retval, i](component_index ci) mutable {
                retval[i++] = key_value{i++,
                                        (*this->containers_.find(ci)->second)[index]};
            });
            return key_value_arr{std::unique_ptr<key_value>(retval), b_.size()};
        }

        void command_add() {
            count_++;
        }

        template<class ...Args>
        void command_set(uint32_t index, component_index ci, Args... args) {
            auto iter = containers_.find(ci);
            if (iter != containers_.end()) {
                auto container = iter->second;
            }
        }

        either<std::shared_ptr<ecs::i_component_container>, nullptr_t> query_containers(component_index ci) {
            typedef either<std::shared_ptr<ecs::i_component_container>, nullptr_t> ptr_or_null;

            // does not have that type
            if (!b_.has_type(ci)) return ptr_or_null(nullptr);

            // find if vector exists
            auto iter = containers_.find(ci);

            if (iter == containers_.end()) return ptr_or_null(nullptr);

            return either<std::shared_ptr<ecs::i_component_container>, nullptr_t>(iter->second);
        }
    };
}

#endif //ECS_ARCHETYPE_H
