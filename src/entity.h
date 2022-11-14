//
// Created by ethanthoma on 2022-01-02.
//

#ifndef ECS_ENTITY_H
#define ECS_ENTITY_H

#include <cstdint>
#include <memory>
#include <functional>

namespace ecs {
    struct entity {
        uint32_t id_ = 0;

        entity() = default;
        explicit entity(uint32_t t_id) : id_(t_id) {}

        bool operator==(const entity & e) const {
            return id_ == e.id_;
        }
    };

    struct entity_hash {
        std::size_t operator()(entity const &e) const noexcept {
            return std::hash<uint32_t>{}(e.id_) + 0x9e3779b9;
        }
    };

    struct entity_batch {
        uint32_t size_ = 0;
        std::unique_ptr<entity> arr_ = nullptr;

        explicit entity_batch(uint32_t t_size) : size_(t_size), arr_(std::make_unique<entity>(t_size)) {}

        entity_batch(entity_batch const & t_eb) : size_(t_eb.size_), arr_(std::make_unique<entity>(size_)) {
            copy(t_eb);
        }

        void foreach(const std::function<void(entity)> & func) const {
            for (uint32_t i = 0; i != size_; ++i) {
                func(arr_.get()[i]);
            }
        }

        entity_batch & operator=(const entity_batch & t_eb) {
            copy(t_eb);

            return *this;
        }

        ecs::entity & operator[](uint32_t index) const {
            return arr_.get()[index];
        }

        // order preserving
        bool operator==(const entity_batch & eb) const noexcept {
            bool is_equal = true;
            for (uint32_t i = 0; i != size_ && is_equal; ++i) {
                is_equal &= arr_.get()[i] == eb.arr_.get()[i];
            }
            return is_equal;
        }

    private:
        void copy(const entity_batch & t_eb) {
            size_ = t_eb.size_;
            arr_ = std::make_unique<entity>(t_eb.size_);

            for (uint32_t i = 0; i != size_; ++i) {
                arr_.get()[i] = t_eb.arr_.get()[i];
            }
        }
    };
} // namespace ecs

#endif //ECS_ENTITY_H
