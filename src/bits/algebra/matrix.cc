#include "bits/algebra/matrix.h"

using namespace bits;

// Public methods

Matrix::Matrix(void)
    : m_nrows_bits(1), m_ncols_bits(1), m_matrix(1, 1) {}

Matrix::Matrix(size_t nrows, size_t ncols)
    : m_nrows_bits(nrows), m_ncols_bits(ncols), m_matrix(nrows, ncols) {}

Matrix::Matrix(const Matrix& in_matrix) 
    : m_nrows_bits(in_matrix.nrows()), m_ncols_bits(in_matrix.ncols()), m_matrix(in_matrix.m_matrix) {}

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

