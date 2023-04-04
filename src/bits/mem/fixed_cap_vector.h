// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Original Files from Craig Gidney's Stim Package https://github.com/quantumlib/Stim

#ifndef _BITS_MEM_FIXED_CAP_VECTOR_H
#define _BITS_MEM_FIXED_CAP_VECTOR_H

#include <array>
#include <cstddef>
#include <sstream>
#include <stdexcept>

namespace bits {

/// A vector with a variable number of items but a fixed size backing array.
template <typename T, size_t max_size>
class FixedCapVector {
    std::array<T, max_size> data;
    size_t num_used;

   public:
    FixedCapVector() : num_used(0){};
    FixedCapVector(std::initializer_list<T> list) : num_used(0) {
        if (list.size() > max_size) {
            throw std::out_of_range("list.size() > max_size");
        }
        for (auto &e : list) {
            push_back(std::move(e));
        }
    }

    T &operator[](size_t index) {
        return data[index];
    }

    const T &operator[](size_t index) const {
        return data[index];
    }

    const T &front() const {
        if (num_used == 0) {
            throw std::out_of_range("Empty.");
        }
        return data[0];
    }
    const T &back() const {
        if (num_used == 0) {
            throw std::out_of_range("Empty.");
        }
        return data[num_used - 1];
    }
    T &front() {
        if (num_used == 0) {
            throw std::out_of_range("Empty.");
        }
        return data[0];
    }
    T &back() {
        if (num_used == 0) {
            throw std::out_of_range("Empty.");
        }
        return data[num_used - 1];
    }
    T *begin() {
        return &data[0];
    }
    T *end() {
        return &data[0] + num_used;
    }
    const T *end() const {
        return &data[0] + num_used;
    }
    const T *begin() const {
        return &data[0];
    }
    size_t size() const {
        return num_used;
    }
    bool empty() const {
        return num_used == 0;
    }
    T *find(const T &item) {
        auto p = begin();
        while (p != end()) {
            if (*p == item) {
                break;
            }
            p++;
        }
        return p;
    }
    const T *find(const T &item) const {
        auto p = begin();
        while (p != end()) {
            if (*p == item) {
                break;
            }
            p++;
        }
        return p;
    }

    void clear() {
        num_used = 0;
    }

    void push_back(const T &item) {
        if (num_used == data.size()) {
            throw std::out_of_range("CappedVector capacity exceeded.");
        }
        data[num_used] = item;
        num_used++;
    }
    void push_back(T &&item) {
        if (num_used == data.size()) {
            throw std::out_of_range("CappedVector capacity exceeded.");
        }
        data[num_used] = std::move(item);
        num_used++;
    }
    void pop_back() {
        if (num_used == 0) {
            throw std::out_of_range("Popped empty CappedVector.");
        }
        num_used -= 1;
    }
    bool operator==(const FixedCapVector<T, max_size> &other) const {
        if (num_used != other.num_used) {
            return false;
        }
        for (size_t k = 0; k < num_used; k++) {
            if (data[k] != other.data[k]) {
                return false;
            }
        }
        return true;
    }
    bool operator!=(const FixedCapVector<T, max_size> &other) const {
        return !(*this == other);
    }
    bool operator<(const FixedCapVector<T, max_size> &other) const {
        if (num_used != other.num_used) {
            return num_used < other.num_used;
        }
        for (size_t k = 0; k < num_used; k++) {
            if (data[k] != other.data[k]) {
                return data[k] < other.data[k];
            }
        }
        return false;
    }

    std::string str() const {
        std::stringstream ss;
        ss << *this;
        return ss.str();
    }
};

template <typename T, size_t max_size>
std::ostream &operator<<(std::ostream &out, const FixedCapVector<T, max_size> &v) {
    out << "FixedCapVector{";
    bool first = true;
    for (const auto &t : v) {
        if (first) {
            first = false;
        } else {
            out << ", ";
        }
        out << t;
    }
    out << "}";
    return out;
}

}  // namespace bits

#endif
