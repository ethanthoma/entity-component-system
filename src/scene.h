//
// Created by ethanthoma on 2022-01-03.
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
#include "either.h"

namespace ecs {
    class scene {
    private:
        entity_manager em_;
        component_manager cm;

        std::unordered_map<entity, std::shared_ptr<bitmask>, entity_hash> e_to_b_;
        std::unordered_map<entity, uint32_t, entity_hash> e_to_a_index_;

        class mutator {
        private:
            either<entity, entity_batch> e_;
        public:
            explicit mutator(entity t_e) : e_(either<entity, entity_batch>(t_e)) {}
            explicit mutator(const entity_batch & t_eb) : e_(either<entity, entity_batch>(t_eb)) {}

            template <class C, class ...Args>
            auto add_component(Args ...args) {

            }

            template <class C>
            auto remove_component() {

            }

            template<class first, class ...rest, class ...Args>
            auto set_archetype(Args ...args) {

            }
        };
    public:
        mutator mutate(entity t_e) {
            return mutator(t_e);
        }

        mutator mutate(const entity_batch& t_eb) {
            return mutator(t_eb);
        }

        ecs::entity make_entity() {
            ecs::entity e = em_.make();
            return e;
        }

        ecs::entity_batch make_entity(uint32_t t_amt) {
            ecs::entity_batch eb = em_.make(t_amt);
            return eb;
        }

        void free_entity(entity t_e) {
            em_.free(t_e);
        }

        void free_entity(const entity_batch & t_eb) {
            em_.free(t_eb);
        }

//        auto makeEntity() {
//            // create a new entity
//            auto e = em.make();
//            // create an empty bitmask
//            auto b = std::make_shared<ecs::bitmask>();
//
//            // set entities map values
//            e_to_b_[e] = b;
//            e_to_a_index_[e] = cm.get_archetype_count(b) - 1;
//
//            return e;
//        }



//        template<typename first, typename  ...rest> auto setArchetype(entity e) {
//            auto ap = cm.template update_archetype<first, rest...>(e_to_b_.at(e));
//
//            e_to_b_.insert_or_assign(e, ap);
//            e_to_a_index_[e] = cm.get_archetype_count(ap) - 1;
//
//            return ap;
//        }

//        void setArchetype(entity e, bitmask * t_a) {
//            auto ap = cm.update_archetype(e_to_b_.at(e), t_a);
//
//            e_to_b_.insert_or_assign(e, ap);
//            e_to_a_index_[e] = cm.get_archetype_count(ap) - 1;
//        }

        // adds a component to an entity
        // this means that the archetype changes and increases in size
        // this means when pathing via queries, the new component lies upstream of the current one,
        // so we can delay updates till we actually return the query
//        template<class T, class ...Args> void addComponent(entity e, Args && ... args) {
//            // update the current archetype and get the new archetype
//            auto b = cm.template update_archetype<T>(e_to_b_.at(e));w
//
//            e_to_b_.insert_or_assign(e, b);
//            e_to_a_index_[e] = cm.get_archetype_count(b) - 1;
//
//            setComponent<T>(e, std::forward<Args>(args) ...);
//        }

//        template<typename T, typename ...Args> void setComponent(entity e, Args && ... args) {
//
//        }
//
//        template<typename T> void removeComponent(entity e) {
//        }
//
//        void freeEntity(entity e) {
//        }
    };
} // namespace ecs

#endif //ECS_SCENE_H
