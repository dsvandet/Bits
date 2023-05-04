#include "bits/mem/simd_bit_table_util.h"

using namespace bits;

template<size_t W>
RREF::RREF<W>(std::vector<size_t> heads,
        bits::simd_bit_table<W> matrix,
        bits::simd_bit_table<W> transform,
        size_t rank)
    : heads(heads), matrix(matrix), transform(transform), rankl(rank) {}

template<size_t W>
RREF::RREF<W> rref_complete(bits::simd_bit_table<W> in_matrix, size_t in_nrows, size_t in_ncols) {

    // Augment the matrix
    simd_bit_table<W> identity = simd_bit_table<W>::identity(nrows);
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
    std::vector<int> heads(hncols, -1);

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
            std::vector<size_t> diff;
            for (size_t index = j + 1; index < heads.size(); index++) {
                if (0 <= heads[index] && heads[index] < heads[j]) {
                    diff.push_back(heads[index]);
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

    std::vector<size_t> pivots;
    for (auto it = heads.begin(); it != heads.end(); it++) {
        if (*it != -1)
            pivots.push_back(*it);
    }
    
    size_t rnrows = std::distance(pivots.begin(),pivots.end());
    bits::simd_bit_table<W> co_row_reduced(rnrows, ncols);

    size_t index=0;
    for (auto it = pivots.begin(); it != pivots.end(); it++) {
        co_row_reduced[index] = co_row[*it];
        index++; 
    }

    bits::simd_bit_table<W> rref_mat(rnrows, hncols);

    // This should be replaced with a libbits method

    for (auto i = 0; i < rnrows; i++) {
        for(auto j = 0; j < hncols; j++) {
            rref_mat[i][j] = co_row_reduced[i][j];
        }
    }

    bits::simd_bit_table<W> transform_mat(rnrows, nrows);

    // This should be replaced with a libbits method
    for (auto i = 0; i < rnrows; i++) {
        for(auto j = 0; j < nrows; j++) {
            transform_mat[i][j] = co_row_reduced[i][j + hncols];
        }
    }

    for(auto it=heads.begin(); it != heads.end(); it++) {
        *it += 1;
        if (*it > 0)
            *it = 1;
    }

    size_t rank = std::reduce(heads.begin(), heads.end());

    RREF

}



    

    

    // Display results

    std::cout << "Heads" << std::endl << "[ ";
    for (auto element: heads) {
        std::cout << element << " ";
    }
    std::cout << "]" << std::endl << std::endl;

    std::cout << "RREF" << std::endl << rref_mat.str(rnrows, hncols) << std::endl << std::endl;

    std::cout << "Transform" << std::endl << transform_mat.str(rnrows, nrows) << std::endl << std::endl;

    std::cout << "Rank = " << rank << std::endl;

    return 1;
}