//
// Created by ethanthoma on 2022-01-04.
//

#ifndef ECS_BITMASK_H
#define ECS_BITMASK_H

#include <cstdlib>
#include <cstdint>
#include <set>
#include <utility>
#include <functional>

namespace ecs {
    class bitmask {
    private:
        using bitmask_size = uint32_t;
        using index_size = uint32_t;
        uint32_t BITS = 32;

        index_size capacity_ = 1;
        index_size size_ = 0;
        bitmask_size * begin_;
    public:
        bitmask() {
            begin_ = (bitmask_size *) calloc(capacity_, sizeof(bitmask_size));
        }

        bitmask(const bitmask & t_bm) {
            size_ = t_bm.size_;
            capacity_ = t_bm.capacity_;
            begin_ = (bitmask_size *) calloc(capacity_, sizeof(bitmask_size));

            for (index_size i = 0; i != capacity_; ++i) {
                begin_[i] = t_bm.begin_[i];
            }
        }

        bitmask(std::initializer_list<index_size> t_is) {
            begin_ = (bitmask_size *) calloc(capacity_, sizeof(bitmask_size));
            for (auto i : t_is) {
                insert(i);
            }
        }

        ~bitmask() {
            free(begin_);
        }

        void insert(index_size t_i) {
            // divide by BITS to see which part of the array it indexes into
            index_size index = t_i / BITS;
            // get the current bit
            bitmask_size val = 1 << (BITS - 1 - (t_i % BITS));

            // increase the size of the array if you have to grow to fit more components
            if (index >= capacity_) {
                capacity_ = index + 1;
                begin_ = (bitmask_size *) realloc(begin_, capacity_);
                begin_[index] &= 0;
            }

            // increase the size if it's bigger than the current size
            size_ = size_ < t_i ? t_i : size_;
            // set bit
            begin_[index] |= val;
        }

        bool has_type(index_size t_i) {
            // divide by BITS to see which part of the array it indexes into
            index_size index = t_i / BITS;
            // get the current bit
            bitmask_size val = (bitmask_size) 1 << (BITS - 1 - (t_i % BITS));

            return index < capacity_ && begin_[index] & val;
        }

        [[nodiscard]] size_t hash() const {
            std::size_t seed = 0;
            index_size i = 0;
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

            index_size i = 0, j = 0;
            while (i++ != capacity_) {
                j |= begin_[i] ^ t_other.begin_[i];

                if (j) break;
            }

            return !j;
        }

        bool operator!=(const bitmask & t_other) const {
            if (size_ != t_other.size_) return true;

            index_size i = 0, j = 0;
            while (i++ != capacity_) {
                j |= begin_[i] ^ t_other.begin_[i];

                if (j) break;
            }

            return j;
        }

        bool operator<(const bitmask & t_other) const {
            if (size_ != t_other.size_) return size_ < t_other.size_;

            // capacity_ >= 1 so begin_[0] is always defined
            index_size i = 0;
            while(begin_[i] == t_other.begin_[i] && ++i != capacity_) {}

            return begin_[i] < t_other.begin_[i];
        }

        void foreach(const std::function<void(uint32_t)> & func) {
            index_size shift, i;
            bitmask_size mask;
            // iterate over all components
            for (i = 0; i != capacity_; ++i) {
                // shift to check for each component
                for (shift = 0; shift != BITS; ++shift) {
                    // get the bit we care about
                    mask = 1 << (BITS - 1 - shift);
                    // if we have it, call callback with it
                    if (begin_[i] & mask) {
                        func(shift + (i * BITS));
                    }
                }
            }
        }

        template<class T>
        auto map(const std::function<T(uint32_t)> & func) {
            auto arr = new T[size_];
            index_size shift, i, j = 0;
            bitmask_size mask;
            // iterate over all components
            for (i = 0; i != capacity_; ++i) {
                // shift to check for each component
                for (shift = 0; shift != BITS; ++shift) {
                    // get the bit we care about
                    mask = 1 << (BITS - 1 - shift);
                    // if we have it, call callback with it
                    if (begin_[i] & mask) {
                        arr[j++] = func(shift + (i * BITS));
                    }
                }
            }

            return std::unique_ptr<char[]>(arr);
        }

        [[nodiscard]] bool subset_of(const bitmask& t_b) const {
            if (size_ < t_b.size_) return false;
            index_size smallest_capacity = std::min(capacity_, t_b.capacity_);

            bool is_subset = false;
            for (index_size i = 0; i != smallest_capacity && !is_subset; ++i) {
                is_subset |= (t_b.begin_[i] | begin_[i]) ^ t_b.begin_[i];
            }
            return is_subset;
        }
    };

    struct bitmask_hash {
        std::size_t operator()(const bitmask & t_a) const noexcept {
            return t_a.hash();
        }
    };
} // namespace ecs

#endif //ECS_BITMASK_H
