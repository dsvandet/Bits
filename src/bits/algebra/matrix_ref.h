#ifndef _BITS_MATRIX_REF_H
#define _BITS_MATRIX_REF_H

#include <cstdsize_t>
#include <iostream>

#include "bits/mem/simd_word.h"
#include "bits/mem/simd_bit_table.h"
#include "bits/algebra/vector.h"

namespace bits {

/// A Matrix is a bit matrix (matrix over GF(2)). It is based on the simd_bit_table structure with
/// added protections and methods that allow the user not to be converned with bit manipulations, 
/// paddings, SIMD operations, etc. The Matrix class stores the bit sizes of the matrix as this data
/// is not stored with the simd_bit_table structure.

class MatrixRef {
    public:

        /// Constructors

        /// Constructs MatrixRef using an existing simd_bit_table
        explicit MatrixRef(size_t nrows, size_t ncols, simd_bit_table& table);

        /// Constructs copy of a GF2 Matrix
        MatrixRef(const Matrix& in_matrix);


        // Operators

        MatrixRef& operator=(const MatrixRef& rhs);
        bool operator==(const MatrixRef& rhs);
        MatrixRef operator+(const MatrixRef& rhs);
        MatrixRef operator*(const MatrixRef& rhs);
        Vector operator*(const Vector& rhs);
        Vector operator*(const VectorRef& rhs);


        /// Get and set methods

        /// Returns the number of rows of matrix (number of bots)
        size_t nrows() const;
        size_t rows() const;

        /// Returns the number of columns of matrix (number of bots)
        size_t ncols() const;
        size_t columns() const;
      
        /// Returns the bit at matrix[row][column] as a bool
        bool get(size_t row, size_t column) const;
        /// Returns a const bit reference for the bit at matrix[row][column] 
        const bit_ref get_ref(size_t row, size_r column) const;
        /// Return a reference to the bit a matrix[row][column]
        bit_ref bit_ref get_ref(size_t row, size_r column);
        /// Sets the bit as matrix[row][column] as bool value
        void set(size_t row, size_t column, bool value);

        /// Returns copy of the row matrix[row]
        Vector get_row(size_t row) const;
        /// Returns a ref to the row matrix[row]
        VectorRef get_row_ref(size_t row) const;

        /// Sets the row'th row as row_vector
        void set_row(size_t row, const Vector& row_vector);

        /// Returns a copy of the column indexed by column as a Vector
        Vector get_column(size_t column) const;

        /// Sets the column of index column from Vector/VectorRef
        void set_column(size_t column, const Vector& vector);
        void set_column(size_t column, const VectorRef& vector_ref)


        /// Returns a copy of the raw simd_bit_table
        simd_bit_table<MAX_BITWORD_WIDTH> bit_table_copy() const;
    
    private:
        size_t m_nrows_bits;
        size_t m_ncols_bits;

        simd_bit_table<MAX_BITWORD_WIDTH>* m_matrix;

};


}

#endif