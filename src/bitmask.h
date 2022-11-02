//
// Created by User on 2022-01-04.
//

#ifndef ECS_BITMASK_H
#define ECS_BITMASK_H

#include <cstdlib>
#include <cstdint>
#include <set>
#include <utility>

namespace ecs {
    class bitmask {
    private:
        uint32_t capacity_ = 1;
        uint32_t size_ = 0;
        uint64_t * begin_;
    public:
        bitmask() {
            begin_ = (uint64_t *) calloc(capacity_, sizeof(uint64_t));
        }

        bitmask(const bitmask & t_bm) {
            size_ = t_bm.size_;
            capacity_ = t_bm.capacity_;
            begin_ = (uint64_t *) calloc(capacity_, sizeof(uint64_t));

            for (uint32_t i = 0; i != capacity_; ++i) {
                begin_[i] = t_bm.begin_[i];
            }
        }

        bitmask(std::initializer_list<uint32_t> t_is) {
            begin_ = (uint64_t *) calloc(capacity_, sizeof(uint64_t));
            for (auto i : t_is) {
                insert(i);
            }
        }

        ~bitmask() {
            free(begin_);
        }

        void insert(uint32_t t_i) {
            auto index = t_i >> 6;
            auto val = 0x8000000000000000 >> t_i;

            if (index >= capacity_) {
                capacity_ = index + 1;
                begin_ = (uint64_t *) realloc(begin_, capacity_);
            }

            size_ += begin_[index] ^ val;
            begin_[index] |= val;
        }

        bool has_type(uint32_t t_i) {
            auto index = t_i >> 6;
            auto val = 0x8000000000000000 >> t_i;

            return index < capacity_ && begin_[index] & val;
        }

        [[nodiscard]] size_t hash() const {
            std::size_t seed = 0;
            uint32_t i = 0;
            while(i++ != capacity_) {
                seed ^= std::hash<uint64_t>{}(begin_[i]) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
            }
            return seed;
        }

        [[nodiscard]] auto size() const {
            return size_;
        }

        bool operator==(const bitmask & t_other) const {
            if (size_ != t_other.size_) return false;

            uint64_t i = 0, j = 0;
            while (i++ != capacity_) {
                j |= begin_[i] ^ t_other.begin_[i];
            }

            return !j;
        }

        bool operator<(const bitmask & t_other) const {
            if (size_ != t_other.size_) return size_ < t_other.size_;

            // capacity_ >= 1 so begin_[0] is always defined
            uint64_t i = 0;
            while(begin_[i] == t_other.begin_[i] && ++i != capacity_) {}

            return begin_[i] < t_other.begin_[i];
        }
    };
} // namespace ecs

#endif //ECS_BITMASK_H
