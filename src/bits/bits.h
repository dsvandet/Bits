#include <algorithm>
#include <cassert>
#include <cstring>
#include <sstream>

#include "bits/mem/pointer_range.h"
#include "stim/mem/simd_bits.h"
#include "bits/mem/simd_bit_table.h"
#include "bits/mem/simd_util.h"



int main() {
    // create a test array ->identity
    size_t W=64;
    bits::simd_bit_table<W> matrix(3,3);
    std::cout << matrix.str(3) << std::endl;

    return 1;
}