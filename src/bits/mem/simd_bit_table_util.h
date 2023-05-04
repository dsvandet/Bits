#ifndef _BITS_MEM_BIT_TABLE_UTIL_H
#define _BITS_MEM_BIT_TABLE_UTIL_H

#include "bits/mem/simd_bit_table.h"

namespace bits {


/// RREF structure for a rref matrix
/// `matrix` is the reduced form matrix without zero rows. It is of dimension rank x nmatrix_cols.
/// `transform` is the transformation matrix of dimension rank x ntrans_cols that transforms, via
/// multiplication on the left, of the input matrix to the rref algorithm which is of dimension 
/// ntrans_cols x nmatrix_cols.
/// `heads` is a 0/1 indicator vector for the pivots
/// `rank` is the rank of the matrix `matrix`
/// Note: The dimensions of the matrix (ntrans_cols x nmatrix_cols) that was provided to the rref 
/// algorithm are not store in this structure

template<size_t W>
struct RREF {
    std::vector<int32_t> heads;
    bits::simd_bit_table<W> matrix;
    bits::simd_bit_table<W> transform;
    size_t rank;

    RREF(std::vector<int32_t> heads,
        bits::simd_bit_table<W> matrix,
        bits::simd_bit_table<W> transform,
        size_t rank);
};

template<size_t W>
RREF<W>::RREF(std::vector<int32_t> heads,
        bits::simd_bit_table<W> matrix,
        bits::simd_bit_table<W> transform,
        size_t rank)
    : heads(heads), matrix(matrix), transform(transform), rank(rank) {}


/// Computes the RREF for a given matrix
/// Returns a RREF structure
template<size_t W>
RREF<W> rref_complete(simd_bit_table<W> in_matrix, size_t in_nrows, size_t in_ncols) {

    // Augment the matrix
    simd_bit_table<W> identity = simd_bit_table<W>::identity(in_nrows);
    simd_bit_table<W> matrix = simd_bit_table<W>::from_halves(in_matrix,
                                                        in_nrows,
                                                        in_ncols, 
                                                        bits::OnLeft, 
                                                        identity, 
                                                        in_nrows, 
                                                        in_nrows);
        
    size_t nrows = in_nrows;
    size_t ncols = in_nrows + in_ncols;
    size_t hncols = in_ncols;  

    bits::simd_bit_table<W> co_row(nrows, ncols);
    size_t count = 0;
    std::vector<int32_t> heads(hncols, -1);

    // # Clear entries below pivot
    for (int32_t i = 0;  i < nrows; ++i) {
        simd_bits_range_ref<W> row = matrix[i];
        for(int32_t j = 0; j< hncols; ++j) {
            if (heads[j] != -1 && row[j] == true)
                row ^= co_row[heads[j]];
        }
        size_t lz = row.lzcnt();
        if (lz < hncols) {
            co_row[count++] = row;
            heads[lz] = count - 1;
        }
    }


    // # Clear entries above pivots
    for(int32_t j = hncols - 1; j > -1; j--) {
        if (heads[j] != -1) {
            std::vector<int32_t> diff;

            for (int32_t item = 0; item < heads[j]; item++) {
                bool found = false;
                for (int32_t i = j + 1; i < heads.size(); i++) {
                    if (item == heads[i]) {
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    diff.push_back(item);
                }
            }

            simd_bits_range_ref<W> row = co_row[0];
            for (auto i = diff.begin(); i != diff.end(); ++i) {
                row = co_row[*i];
                if (row[j])
                    co_row[*i] ^= co_row[heads[*i]];
            }
        }
    }

    // Extract pivots vector
    std::vector<int32_t> pivots;
    for (auto it = heads.begin(); it != heads.end(); it++) {
        if (*it != -1)
            pivots.push_back(*it);
    }
    
    // Find rank = rnrows
    size_t rnrows = std::distance(pivots.begin(),pivots.end());
    bits::simd_bit_table<W> co_row_reduced(rnrows, ncols);

    // Extract pivots rows
    size_t index = 0;
    for (int32_t i : pivots) {
        co_row_reduced[index++] =co_row[i];
    }

    simd_bit_table<W> rref_mat(rnrows, hncols);

    // This should be replaced with a libbits method
    for (auto i = 0; i < rnrows; i++) {
        for(auto j = 0; j < hncols; j++) {
            rref_mat[i][j] = co_row_reduced[i][j];
        }
    }

    simd_bit_table<W> transform_mat(rnrows, nrows);

    // This should be replaced with a libbits method
    for (auto i = 0; i < rnrows; i++) {
        for(auto j = 0; j < nrows; j++) {
            transform_mat[i][j] = co_row_reduced[i][j + hncols];
        }
    }

    // Convert heads to 0/1 indicator vector
    for(auto it=heads.begin(); it != heads.end(); it++) {
        *it += 1;
        if (*it > 0)
            *it = 1;
    }

    RREF<W> rref(heads, rref_mat, transform_mat, rnrows);

    return rref;
}


}

#endif