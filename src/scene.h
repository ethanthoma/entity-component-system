//
// Created by User on 2022-01-03.
//

#ifndef ECS_SCENE_H
#define ECS_SCENE_H

#include <cstdint>
#include <unordered_map>
#include <tuple>
#include <typeindex>

#include "entity.h"
#include "entity_manager.h"
#include "component_manager.h"
#include "bitmask.h"
#include "instanceof.h"

namespace ecs {
    class scene {
    private:
        entity_manager em;
        component_manager cm;

        std::unordered_map<entity, const bitmask *, entity_hash> e_to_a_;
        std::unordered_map<entity, uint32_t, entity_hash> e_to_a_index_;
    public:
        auto makeEntity() {
            auto e = em.make();

            e_to_a_.insert({e, cm.get_base_bitmask()});
            auto index = cm.template reserve<instanceof>();
            e_to_a_index_[e] = index;

            return e;
        }

        template<typename first, typename  ...rest> auto setArchetype(entity e) {
            auto ap = cm.template update_archetype<first, rest...>(e_to_a_.at(e));

            e_to_a_.insert_or_assign(e, ap);
            e_to_a_index_[e] = cm.get_archetype_count(ap) - 1;

            return ap;
        }

        void setArchetype(entity e, bitmask * t_a) {
            auto ap = cm.update_archetype(e_to_a_.at(e), t_a);

            e_to_a_.insert_or_assign(e, ap);
            e_to_a_index_[e] = cm.get_archetype_count(ap) - 1;
        }

        template<typename T, typename ...Args> void addComponent(entity e, Args && ... args) {
            auto ap = cm.template update_archetype<T>(e_to_a_.at(e));

            e_to_a_.insert_or_assign(e, ap);
            e_to_a_index_[e] = cm.get_archetype_count(ap) - 1;

            setComponent<T>(e, std::forward<Args>(args) ...);
        }

        template<typename T, typename ...Args> void setComponent(entity e, Args && ... args) {

        }

        template<typename T> void removeComponent(entity e) {
        }

        void freeEntity(entity e) {
        }
    };
} // namespace ecs

#endif //ECS_SCENE_H
