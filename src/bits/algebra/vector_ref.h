#ifndef _BITS_VECTOR_REF_H
#define _BITS_VECTOR_REF_H

#include <cstdint>
#include <iostream>

#include "bits/algebra/vector.h"
#include "bits/mem/simd_word.h"
#include "bits/mem/simd_bits.h"

namespace bits {

/// A VectorRef is a bit vector reference. It is based on the simd_bits_range_ref structure with
/// added protections and methods that allow the user not to be concerned with bit manipulations, 
/// paddings, SIMD operations, etc. The VectorRef class stores the bit size of the vector as this data
/// is not stored with the simd_bits_range_ref structure.

class VectorRef {
    public:

        friend class Vector;

        /// Constructors

        /// Constructs a reference vector to a given simd_bits_range_ref
        inline VectorRef(const simd_bits_range_ref<MAX_BITWORD_WIDTH> range_ref, size_t start_bit, size_t bit_length)
            : m_start_bit_index(start_bit), m_size_bits(bit_length), m_simd_bits_range_ref(range_ref.ptr_simd, range_ref.num_simd_words) {}

        /// Constructs a reference vector to a given simd_bits_range_ref from a provided Vector
        inline VectorRef(const Vector vector) :
            m_start_bit_index(0), m_size_bits(vector.m_size_bits), m_simd_bits_range_ref(simd_bits_range_ref<MAX_BITWORD_WIDTH>(vector.m_simd_bits)) {} 

        /// Returns the starting index of the VectorRef
        inline size_t start() const {
            return this->m_start_bit_index;}

        /// Returns the length of vector (number of bits)
        inline size_t size() const {
            return this->m_size_bits;
        }
        inline size_t length() const {
            return this->size();
        }

        /// Returns the underlying simd_bits_range_ref member
        inline simd_bits_range_ref<MAX_BITWORD_WIDTH> range_ref() const {
            return this->m_simd_bits_range_ref;
        }

        /// Returns a reference to a given bit within the referenced range.
        inline bit_ref operator[](size_t k) {
            return this->m_simd_bits_range_ref[k];
        }
        /// Returns a const reference to a given bit within the referenced range.
        inline const bit_ref operator[](size_t k) const {
            return this->m_simd_bits_range_ref[k];
        }

        friend std::ostream &operator<<(std::ostream &out, const VectorRef m);

        /// Returns a description of the contents of the range.
        inline std::string str() const {
            std::stringstream ss;
            ss << *this;
            return ss.str();
        }

    private:
        size_t m_start_bit_index;
        size_t m_size_bits;

        simd_bits_range_ref<MAX_BITWORD_WIDTH> m_simd_bits_range_ref;

};

/// Writes a description of the contents of the range to `out`.
inline std::ostream &operator<<(std::ostream &out, const VectorRef m) {
    for (size_t k = m.start(); k < m.start() + m.length(); k++) {
        out << ".1"[m[k]];
    }
    return out;
}

}

#endif