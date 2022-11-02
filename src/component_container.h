//
// Created by User on 2022-01-02.
//

#ifndef ECS_COMPONENT_CONTAINER_H
#define ECS_COMPONENT_CONTAINER_H

#include <iostream>
#include <cstdint>
#include <cstdlib>

#include "component.h"

namespace ecs {
    class i_component_container {
    protected:
        static const uint32_t MIN_CAPACITY_ = 64;

        uint32_t size_ = 0;
        uint32_t elem_size_ = 0;
        uint32_t capacity_ = 0;
        uint32_t capacity_size_ = MIN_CAPACITY_;
        void * begin_;
        void * end_;

        void reserve(uint32_t t_new_capacity) {
            capacity_ = t_new_capacity;
            capacity_size_ = capacity_ * elem_size_;

            begin_ = realloc(begin_, capacity_size_);
            end_ = reinterpret_cast<char *>(begin_) + capacity_size_;
        }
    public:
        explicit i_component_container(uint32_t t_capacity_size_) {
            begin_ = malloc(t_capacity_size_);
            end_ = reinterpret_cast<char *>(begin_) + capacity_size_;
        }

        ~i_component_container() {
            free(begin_);
        }

        template <typename T>
        void insert(T t) {
            if (size_ == capacity_) {
                reserve(2 * capacity_);
            }

            memmove(reinterpret_cast<char *>(begin_) + (size_ * elem_size_), &t, elem_size_);

            size_++;
        }

        template <typename T>
        void replace(T t, uint32_t i) {
            // grow to fit index
            if (i >= capacity_) {
                uint32_t min_cap = MIN_CAPACITY_ / elem_size_;
                reserve(((i / min_cap) + 1) * min_cap);
            }

            memmove(reinterpret_cast<char *>(begin_) + (i * elem_size_), &t, elem_size_);

            // if it's not within the original size_, set size_ to it
            size_ = i+1 > size_ ? size_ = i+1 : size_;
        }

        void resize(uint32_t t_size) {
            if (t_size > capacity_) {
                uint32_t min_cap = MIN_CAPACITY_ / elem_size_;
                reserve(((t_size / min_cap) + 1) * min_cap);
            }

            size_ = t_size;
        }

        template<typename T, typename ... Args>
        void emplace(Args && ... args) {
            insert(T(args ...));
        }

        void swap(uint32_t t_first_index, uint32_t t_second_index) {
            uint8_t * a = reinterpret_cast<uint8_t *>(begin_) + (t_first_index * elem_size_);
            uint8_t * b = reinterpret_cast<uint8_t *>(begin_) + (t_second_index * elem_size_);

            for (uint32_t i = 0; i != elem_size_; ++i, ++a, ++b) {
                (((*a) ^ (*b)) && ((*b) ^= (*a) ^= (*b), (*a) ^= (*b)));
            }
        }

        void remove(uint32_t t_index) {
            if (size_ == 0) {std::cout<<"remove: size_ alrdy 0\n"; return;}

            if (t_index != size_ - 1) {
                memcpy(reinterpret_cast<char *>(begin_) + (t_index * elem_size_), reinterpret_cast<char *>(end_) - elem_size_, elem_size_);
            }

            size_--;
        }

        [[nodiscard]] uint32_t size() const {
            return size_;
        }
    };

    template <typename T>
    class component_container : public i_component_container {
    public:
        component_container() : i_component_container(capacity_size_ < sizeof(T) ? ((sizeof(T) / capacity_size_) + 1) : MIN_CAPACITY_) {
            if ((elem_size_ = sizeof(T)) == 0) { std::cout<<"component_container: template size_ is 0\n"; throw; }

            capacity_ = capacity_size_ / elem_size_;
        }
    };
} // namespace ecs

#endif //ECS_COMPONENT_CONTAINER_H