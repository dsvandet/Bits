#ifndef _BITS_VECTOR_REF_H
#define _BITS_VECTOR_REF_H

#include <cstdint>
#include <iostream>

#include "bits/mem/simd_word.h"
#include "bits/mem/simd_bits.h"

namespace bits {

/// A VectorRef is a bit vector reference. It is based on the simd_bits_range_ref structure with
/// added protections and methods that allow the user not to be convered with bit manipulations, 
/// paddings, SIMD operations, etc. The VectorRef class stores the bit size of the vector as this data
/// is not stored with the simd_bits_range_ref structure.

class VectorRef {
    public:

        friend class Vector;

        /// Constructors

        /// Constructs a reference vector to a given simd_bits_range_ref
        VectorRef(const simd_bits_range_ref<MAX_BITWORD_WIDTH>& range_ref, size_t start_bit, size_t bit_length);

        /// Returns the length of vector (number of bits)
        size_t size() const;
        size_t length() const;

        /// Returns the underlying simd_bits_range_ref member
        simd_bits_range_ref<MAX_BITWORD_WIDTH> range_ref() const;

    private:
        size_t m_start_bit_index;
        size_t m_size_bits;

        simd_bits_range_ref<MAX_BITWORD_WIDTH> m_simd_bits_range_ref;

};


}

#endif