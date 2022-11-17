//
// Created by ethanthoma on 11/14/2022.
//

#ifndef ECS_BITMASK_GRAPH_H
#define ECS_BITMASK_GRAPH_H

#include <memory>
#include <list>

namespace ecs {
    class bitmask_graph {
    private:
        struct node {
            bitmask b_;
            std::list<std::weak_ptr<node>> parents_;
            std::list<std::shared_ptr<node>> children_;

            [[nodiscard]] bool subset_of(const node & t_n) const { return b_.subset_of(t_n.b_); }
            bool operator!=(const node & t_n) const { return b_ != t_n.b_; }
        };
        // largest archetypes
        std::list<std::shared_ptr<node>> largest_bitmasks_{};

        static void add_child(const std::shared_ptr<node> & t_child, const std::shared_ptr<node> & t_parent) {
            t_child->parents_.push_back(t_child);
            t_parent->children_.push_back(t_parent);
        }

        bool add_helper(const std::shared_ptr<node>& t_small, const std::shared_ptr<node>& t_large) {
            bool added = false;
            for (auto node_ptr = t_large->children_.begin(); node_ptr != t_large->children_.end(); ++node_ptr) {
                auto child = *node_ptr;
                // t_small is grandchild of t_large
                if (t_small->subset_of(*child)) {
                    added = add_helper(t_small, child);
                }
                // t_small is a parent of child but child of t_large
                else if (child->subset_of(*t_small)) {
                    node_ptr = --t_large->children_.erase(node_ptr);
                    add_child(child, t_small);
                }
            }

            if (!added) {
                add_child(t_small, t_large);
            }

            return true;
        }

        void get_path_helper(const bitmask & t_b) {
        }
    public:
        bitmask_graph() = default;
        explicit bitmask_graph(const bitmask & t_b) {
            largest_bitmasks_.push_back(std::make_shared<node>(node{t_b, {}, {}}));
        }

        void add(const bitmask & t_b) {
            auto n = std::make_shared<node>(node{t_b, {}, {}});
            bool added = false;
            for (auto node_ptr = largest_bitmasks_.begin(); node_ptr != largest_bitmasks_.end(); ++node_ptr) {
                auto & node = *node_ptr;

                // node is child of top nodes
                if (node->subset_of(*n)) {
                    added = add_helper(node, n);
                }

                // node is parent of top node
                else if (n->subset_of(*node)) {
                    add_child(n, node);
                    // remove top node from list
                    // don't update iterator if erased—minus, so it cancels out
                    node_ptr = --largest_bitmasks_.erase(node_ptr);
                }
            }

            // if not child of top node, it is a top node
            if (!added) {
                largest_bitmasks_.push_back(n);
            }
        }

        auto get_path(const bitmask & t_b) {
        }
    };
};

#endif //ECS_BITMASK_GRAPH_H
