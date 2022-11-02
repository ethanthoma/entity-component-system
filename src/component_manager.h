//
// Created by User on 2022-01-02.
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
#include "instanceof.h"

namespace ecs {
    typedef uint32_t component_index;
    typedef uint32_t component_container_index;

    struct archetype {
        std::unordered_map<component_id, component_container_index> id_to_container_index;
        mutable uint32_t start = 0;
        mutable uint32_t end = 0;
        mutable uint32_t count = 0;
    };

    class component_manager {
    private:
        bitmask base_bitmask_;

        uint32_t component_index_count_ = 0;
        std::unordered_map<component_id, component_index> id_to_index_;
        std::vector<std::unique_ptr<i_component_container>> component_containers_;

        struct bitmask_hash {
            std::size_t operator()(const bitmask & t_a) const noexcept {
                return t_a.hash();
            }
        };

        std::unordered_map<bitmask, archetype, bitmask_hash> bm_to_a_;
        std::set<const archetype *> layout_;

        template<typename T> auto get_component_container() {
            return static_cast<component_container<T> *>(component_containers_[id_to_index_.at(component<T>::get_id())].get());
        }
    public:
        component_manager() {
            component_id id = component<instanceof>::get_id();
            component_index index = register_component<instanceof>();

            archetype a;
            a.id_to_container_index[id] = index;
            a.count = 1;
            base_bitmask_.insert(id);
            bm_to_a_[base_bitmask_] = a;
            layout_.insert(&bm_to_a_.at(base_bitmask_));
        }

        const bitmask * get_base_bitmask() {
            return &base_bitmask_;
        }

        template<typename T> component_index register_component() {
            // get type index of T
            const component_id & id = component<T>::get_id();

            // see if component has been registered
            if (id_to_index_.find(id) == id_to_index_.end()) {
                // register type index to component i (id)
                id_to_index_[id] = component_index_count_++;
                // map component i to bm component container; indexed by (i, j)
                component_containers_.push_back(std::make_unique<component_container<T>>());
                return component_index_count_ - 1;
            } else {
                return id_to_index_.at(id);
            }
        }

        const bitmask * register_archetype(const bitmask * t_a) {
            bm_to_a_[*t_a].count++;
            return &bm_to_a_.find(*t_a)->first;
        }

        void deregister_archetype(const bitmask * t_a, component_container_index t_instanceof_index) {
            auto it = bm_to_a_.find(*t_a);
            if (it != bm_to_a_.end()) {
                // decreases archetype count and end
                it->second.count--;
                it->second.end--;
                // deletes archetype if no more entities have it
                if(it->second.count == 0) {
                    layout_.erase(layout_.find(&it->second));
                    bm_to_a_.erase(it);
                    return;
                }

                // shuffles every archetype after the current archetype down
                auto set_it = --layout_.rbegin();
                while(*set_it != &it->second) {
                    (*set_it)->start--;
                    (*set_it)->end--;
                }

                // swaps the contents of the last entity with the entity with instanceof index of t_instanceof_index
                for (auto & [id, con_index] : (*set_it)->id_to_container_index) {
                    static_cast<i_component_container *>(component_containers_[id].get())->swap(con_index + (*set_it)->end,
                                                                                                con_index + (t_instanceof_index - (*set_it)->start));
                }
            }
        }

        template<typename first, typename ...rest> const bitmask * update_archetype(const bitmask * t_old) {
            bitmask t_new(*t_old);

            deregister_archetype(t_old);

            t_new.insert(register_component<first>());
            t_new.insert(register_component<rest...>());

            return register_archetype(&t_new);
        }

        const bitmask * update_archetype(const bitmask * t_old, const bitmask * t_new) {
            deregister_archetype(t_old);
            return register_archetype(t_new);
        }

        // reserves space on component container i
        // does not init values (junk values)
        template<class T> auto reserve() {
            auto c_container = get_component_container<T>();
            auto index = c_container->size();
            c_container->resize(index + 1);
            return index;
        }

        // replaces component at i,j with new component(args)
        template<typename T, typename ...Args> void set(component_container_index t_j, Args && ... args) {
            get_component_container<T>()->template replace(T(args ...), t_j);
        }

        // removes component i,j
        template<typename T> void free(component_container_index t_j) {
            register_component<T>();
            get_component_container<T>()->remove(t_j);
        }

        // removes component i,j
        void free(component_index t_i, component_container_index t_j) {
            static_cast<i_component_container *>(component_containers_[t_i].get())->remove(t_j);
        }

        auto get_archetype_count (const bitmask * t_a) const {
            return bm_to_a_.at(*t_a).count;
        }

        // returns i of component
        template<typename T> component_index get_component_index() const {
            return id_to_index_.at(component<T>::get_id());
        }

        // returns i of component
        component_index get_component_index(component_id t_type_index) const {
            return id_to_index_.at(t_type_index);
        }
    };
} // namespace ecs

#endif //ECS_COMPONENT_MANAGER_H
