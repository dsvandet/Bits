#include <string>

#include "gtest/gtest.h"

#include "bits/algebra/vector_ref.h"
#include "bits/algebra/vector.h"
#include "bits/mem/simd_word.h"
#include "bits/mem/simd_bits.h"
#include "bits/mem/simd_util.h"
#include "bits/test_util.test.h"

using namespace bits;

TEST(vector_ref, creation) {
    simd_bits<MAX_BITWORD_WIDTH> d(1024);
    d.randomize(735, SHARED_TEST_RNG());
    simd_bits_range_ref<MAX_BITWORD_WIDTH> ref(d);
    VectorRef vector_ref(ref, 0, 735);
    ASSERT_EQ(vector_ref.start(), 0);
    ASSERT_EQ(vector_ref.size(), 735);
    ASSERT_EQ(vector_ref.range_ref()==ref, true);
}

TEST(vector_ref, operator) {
    simd_bits<MAX_BITWORD_WIDTH> d(1024);
    d.randomize(735, SHARED_TEST_RNG());
    simd_bits_range_ref<MAX_BITWORD_WIDTH> ref(d);
    VectorRef vector_ref(ref, 0, 735);
    vector_ref[101] = 1;
    ASSERT_EQ(vector_ref[101] == true,1);
}

TEST(vector_ref, str) {
    std::string str_vector("1.1.1....1..1...1.1.1.1..1.1.");
    Vector vector(str_vector);
    VectorRef vector_ref = VectorRef(vector);
    std::string str = vector_ref.str();
    ASSERT_EQ(str, str_vector);
}