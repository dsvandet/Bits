#ifndef _BITS_VECTOR_H
#define _BITS_VECTOR_H

#include <cstdint>
#include <iostream>
#include <string>
#include <cassert>

#include "bits/mem/simd_word.h"
#include "bits/mem/simd_bits.h"
#include "bits/algebra/vector_ref.h"

namespace bits {

/// A Vector is a bit vector (vector over GF(2)). It is based on the simd_bits structure with
/// added protections and methods that allow the user not to be converned with bit manipulations, 
/// paddings, SIMD operations, etc. The Vector class stores the bit size of the vector as this data
/// is not stored with the simd_bits structure.

class Vector {
    public:

        friend class VectorRef;

        /// Constructors

        /// Constructs a size GF vector of zeros
        explicit Vector(size_t size);

        /// Constructs copy of a GF2 Vector
        Vector(const Vector& vector);

        /// Constructs copy of a GF2 VectorRef
        explicit Vector(const VectorRef ref_vector);

        // Constructs a Vector from strings or chars
        explicit Vector(const std::string value);
        explicit Vector(const char* char_ptr);


        /// Returns the length of the vector (number of bits)
        /// size() is the primary defintion. All other methods call this.
        size_t size() const;
        size_t length() const;


        /// Returns a copy of the raw simd_bits
        simd_bits<MAX_BITWORD_WIDTH> get_simd_bits() const;

    
    private:
        size_t m_size_bits;

        simd_bits<MAX_BITWORD_WIDTH> m_simd_bits;

};


}

#endif