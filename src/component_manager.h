//
// Created by ethanthoma on 2022-01-02.
//

#ifndef ECS_COMPONENT_MANAGER_H
#define ECS_COMPONENT_MANAGER_H

#include <cstdint>
#include <utility>
#include <vector>
#include <queue>
#include <set>
#include <tuple>
#include <memory>
#include <unordered_map>
#include <typeindex>
#include <atomic>

#include "component.h"
#include "component_container.h"
#include "bitmask.h"
#include "archetype.h"
#include "bitmask_graph.h"

namespace ecs {
    typedef uint32_t component_index;
    typedef uint32_t component_container_index;

    class component_manager {
    private:
        // how many components are registered
        uint32_t component_index_count_ = 0;
        // maps component to index in component containers
        std::unordered_map<component_id, component_index> id_to_index_;
        // vector of all the component containers
        // std::vector<std::unique_ptr<i_component_container>> component_containers_;

        // maps an archetype bitmask to an archetype instance
        std::unordered_map<bitmask, archetype, bitmask_hash> bm_to_a_;
        // all archetypes created so far
        std::set<const archetype *> layout_;

        bitmask_graph ag_;

        template<typename T> auto get_component_container() {
           //return static_cast<component_container<T> *>(component_containers_[id_to_index_.at(component<T>::get_id())].get());
        }
    public:
        component_manager() = default;

//        template<class first, class ...rest>
//        auto get_archetype() {
//            // generate bit mask
//            bitmask b_;
//            b_.insert(component<first>::get_id());
//            b_.insert(component<rest...>::get_id());
//
//
//        }

        // registers a new component type
        template<typename T> component_index register_component() {
            // get type index of T
            const component_id & id = component<T>::get_id();

            return id;
        }

        template<typename first, typename ...rest>
        bitmask get_bitmask() {
            bitmask b(component<first>::get_id());
            b.insert(component<rest...>::get_id());
            return b;
        }

//        // registers an archetype from bitmask
//        // RETURNS: bitmask pointer
//        const bitmask * register_archetype(const bitmask * t_a) {
//            bm_to_a_[*t_a].query_count()++;
//            return &bm_to_a_.find(*t_a)->first;
//        }

//        void deregister_archetype(const bitmask * t_a, int entity_archetype_index) {
//            auto it = bm_to_a_.find(*t_a);
//            if (it != bm_to_a_.end()) {
//                // decreases archetype count and end
//                it->second.count--;
//                // deletes archetype if no more entities have it
//                if(it->second.query_count() == 0) {
//                    layout_.erase(layout_.find(&it->second));
//                    bm_to_a_.erase(it);
//                    return;
//                }
//
//                // shuffles every archetype after the current archetype down
//                shuffle(&it->second, entity_archetype_index);
//            }
//        }

//        // shuffles archetypes down
//        void shuffle(const archetype * start, uint32_t  offset) {
//            // start at the current archetype in the layout
//            auto set_it = layout_.find(start);
//            // the starting indices to shuffle each component by
//            auto curr_map = start->id_to_container_index;
//            // where the first and last element of the archetype is
//            auto curr_start = start->start;
//            auto curr_count = start->query_count();
//            // iterate all archetypes after the current one
//            while(set_it != layout_.end()) {
//                // move the last element in the container to a gap
//                for (auto & it: curr_map) {
//                    // get the i index for current i
//                    component_index i = id_to_index_.at(it.first);
//                    // subtract so it refers to the last index of the previous map
//                    component_container_index container_offset = it.second + offset;
//                    // move last element to front plus offset
//                    // component_containers_[i]->move_to(curr_start + container_offset + curr_count,
//                    //                                  curr_start + container_offset);
//                }
//                // get next archetype
//                set_it++;
//                // get archetype start index and size
//                // reduce start by 1 for all indices after the first
//                curr_start = --(*set_it)->start;
//                curr_count = (*set_it)->query_count();
//                offset = -1;
//            }
//        }

        //
//        template<typename T> bitmask update_archetype(const bitmask t_old, int entity_archetype_index) {
//            bitmask t_new(t_old);
//
//            deregister_archetype(t_old, entity_archetype_index);
//
//            t_new.insert(register_component<T>());
//
//            return register_archetype(t_new);
//        }

//        const bitmask * update_archetype(const bitmask * t_old, const bitmask * t_new, component_container_index t_instanceof_index) {
//            deregister_archetype(t_old, t_instanceof_index);
//            return register_archetype(t_new);
//        }

//        // reserves space on component container i
//        // does not init values (junk values)
//        template<class T> auto reserve() {
//            auto c_container = get_component_container<T>();
//            auto index = c_container->size();
//            c_container->resize(index + 1);
//            return index;
//        }

//        // replaces component at i,j with new component(args)
//        template<typename T, typename ...Args> void set(component_container_index t_j, Args && ... args) {
//            get_component_container<T>()->template replace(T(args ...), t_j);
//        }
//
//        // removes component i,j
//        template<typename T> void free(component_container_index t_j) {
//            register_component<T>();
//            get_component_container<T>()->remove(t_j);
//        }
//
//        // removes component i,j
//        void free(component_index t_i, component_container_index t_j) {
//            // static_cast<i_component_container *>(component_containers_[t_i].get())->remove(t_j);
//        }

        auto get_archetype_count (const std::shared_ptr<bitmask> & t_a) const {
            return bm_to_a_.at(*t_a).query_count();
        }

//        // returns i of component
//        template<typename T> component_index get_component_index() const {
//            return id_to_index_.at(component<T>::get_id());
//        }

//        // returns i of component
//        component_index get_component_index(component_id t_type_index) const {
//            return id_to_index_.at(t_type_index);
//        }
    };
} // namespace ecs

#endif //ECS_COMPONENT_MANAGER_H
