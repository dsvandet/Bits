#include <algorithm>

#include "bits/algebra/matrix.h"

using namespace bits;

// Public methods

Matrix::Matrix(void)
    : m_nrows_bits(1), m_ncols_bits(1), m_matrix(1, 1) {}

Matrix::Matrix(size_t nrows, size_t ncols)
    : m_nrows_bits(nrows), m_ncols_bits(ncols), m_matrix(nrows, ncols) {}

Matrix::Matrix(const Matrix& in_matrix) 
    : m_nrows_bits(in_matrix.nrows()), m_ncols_bits(in_matrix.ncols()), m_matrix(in_matrix.m_matrix) {}

Matrix::Matrix(const std::string str) {

}

simd_bit_table<MAX_BITWORD_WIDTH> Matrix::vector_vector_to_table(
        std::vector<std::vector<bool>>& vector_matrix,
        size_t min_nrows,
        size_t min_ncols) {

    // Determine the dimensions of the matrix requested

    size_t num_rows = std::max(min_nrows, vector_matrix.size());
    size_t num_cols = min_ncols;
    for (auto vector : vector_matrix) {
        if (vector.size() > num_cols)
            num_cols = vector.size();
    }

    // Build the matrix

    simd_bit_table<MAX_BITWORD_WIDTH> out(num_rows, num_cols);
    for (size_t row = 0; row < vector_matrix.size(); row++) {
        for (size_t col = 0; col < vector_matrix[row].size(); col++) {
            out[row][col] = vector_matrix[row][col];
        }
    }

    return out;
}

size_t Matrix::nrows() const {
    return this->m_nrows_bits;
}

size_t Matrix::ncols() const {
    return this->m_ncols_bits;
}

simd_bit_table<MAX_BITWORD_WIDTH> Matrix::bit_table_copy() const{
    simd_bit_table<MAX_BITWORD_WIDTH> matrix(this->m_matrix);
    return matrix;
}

