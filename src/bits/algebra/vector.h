#ifndef _BITS_VECTOR_H
#define _BITS_VECTOR_H

#include <cstdint>
#include <iostream>
#include <string>
#include <cassert>
#include <bitset>

#include "bits/macros.h"
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
        Vector(const std::string str);
        explicit Vector(const char* const str, size_t length);

        // Returns a VectorRef to the given vectror
        VectorRef ref();

        /// Returns the length of the vector (number of bits)
        /// size() is the primary defintion. All other methods call this.
        size_t size() const;
        size_t length() const;


        /// Returns a copy of the raw simd_bits
        simd_bits<MAX_BITWORD_WIDTH> get_simd_bits() const;

        /// Returns bit_ref for the ith bit
        bit_ref operator[](int_fast32_t index);

        /// Returns a bool representing the ith bit 
        const bool operator[](int_fast32_t index) const;
        bool get(int index) const;

        /// Set the bit to value at a given index
        void set(int index, bool value);
        
        /// Toggle a given bit at the given index
        void toggle(int index);

        // Assignment operator
        Vector& operator=(const Vector& rhs);

        /// Comparision operators
        /// Equality
        bool operator==(const Vector& rhs) const;
        /// Inequality
        bool operator!= (const Vector& rhs) const;

        /// GF2 addition of vectors
        Vector operator+(const Vector& rhs);
        Vector operator+=(const Vector& rhs);
        Vector operator-(const Vector& rhs);
        Vector operator-=(const Vector& rhs);
        Vector operator^(const Vector& rhs);
        Vector operator^=(const Vector& rhs);

        Vector operator*(const Vector& rhs);
        Vector operator*=(const Vector& rhs);
        Vector operator&(const Vector& rhs);
        Vector operator&=(const Vector& rhs);   

        Vector operator|(const Vector& rhs);
        Vector operator|=(const Vector& rhs);     

        
        // Right bit shift
        Vector operator>>(int_fast32_t shift) const;
        // Left bit shift
        Vector operator<<(int_fast32_t shift) const;
              

        /*!
        * Comparison < of two Vectors
        * The comparison uses lexicographic order. One vector A is less than
        * another B if it has fewer elements or if it has the same number of
        * elements and the first unequal pair of elements satisfies A[i] < B[i]
        */
        bool Vector::operator< (const Vector &rhs) const;
        bool Vector::operator> (const Vector &rhs) const;

        friend std::ostream& operator<< (std::ostream& output,
                                        const Vector& input);

        
    
    private:
        size_t m_size_bits;

        simd_bits<MAX_BITWORD_WIDTH> m_simd_bits;

};

static bool isBitstring(const std::string& str);


}

#endif