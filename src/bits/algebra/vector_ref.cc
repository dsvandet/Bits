#include "bits/algebra/vector_ref.h"

using namespace bits;

// Public methods

VectorRef:VectorRef(const simd_bits_range_ref<MAX_BITWORD_WIDTH>& range_ref, size_t start_bit, size_t bit_length)
    : 