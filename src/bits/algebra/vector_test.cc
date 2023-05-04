
#include <string>

#include "gtest/gtest.h"

#include "bits/algebra/vector.h"
#include "bits/mem/simd_word.h"
#include "bits/mem/simd_bits.h"
#include "bits/mem/simd_util.h"
#include "bits/test_util.test.h"

using namespace bits;

TEST(vector, creation) {
    Vector vector(10);
    ASSERT_EQ(vector.str(), "..........");
    Vector vector_from_vector(vector);
    ASSERT_EQ(vector_from_vector.str(), "..........");
    Vector vector_from_ref(vector.ref());
    ASSERT_EQ(vector_from_ref.str(), "..........");
    std::string str("1.1...1.1.1..11.1.1.1.1.1.1.1...1..1.1...11.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1..1......1");
    Vector str_vector(str);
    ASSERT_EQ(str_vector.str(),str);
    char str_char[]="1.1.1..1.1";
    Vector char_str_vector(str_char);
    std::string answer_str(str_char);
    ASSERT_EQ(char_str_vector.str(), answer_str);
}

TEST(vector, reference) {
    std::string str("1.1...1.1.1..11.1.1.1.1.1.1.1...1..1.1...11.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1..1......1");
    Vector str_vector(str);
    uint64_t limb = 6148915600695846213ULL;
    VectorRef ref_vector = str_vector.ref();
    ASSERT_EQ(ref_vector.start(), 0);
    ASSERT_EQ(ref_vector.size(), 87);
    ASSERT_EQ(ref_vector.range_ref().u64[0], limb);
}

TEST(vector, size) {
    Vector vector(1001);
    ASSERT_EQ(vector.size(), 1001);
    ASSERT_EQ(vector.length(), 1001);
}

TEST(vector, copy) {
    std::string str("1.1...1.1.1..11.1.1.1.1.1.1.1...1..1.1...11.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1..1......1");
    Vector vector(str);
    uint64_t limb = 6148915600695846213ULL;
    simd_bits<MAX_BITWORD_WIDTH> val_simd_bits = vector.get_simd_bits();
    ASSERT_EQ(val_simd_bits.u64[0], limb);
}

TEST(vector, bit_ref) {
    std::string str("1.1...1.1.1..11.1.1.1.1.1.1.1...1..1.1...11.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1..1......1");
    Vector vector(str);
    //uint64_t word = 69;
    const bool value = vector[37];
    ASSERT_EQ(value, true);
    ASSERT_EQ((int)*vector[37].byte, 41);
    ASSERT_EQ(vector[37].bit_index, 5);
}

TEST(vector, isequalnot) {
    Vector vector_one(10);
    Vector vector_two(10);
    ASSERT_EQ(vector_one == vector_two, true);
    ASSERT_EQ(vector_one != vector_two, false);
    Vector vector_three("1010100101010");
    Vector vector_four("1010100101010");
    vector_four[1] = true;
    ASSERT_EQ( vector_three == vector_four, false);
    ASSERT_EQ( vector_three != vector_four, true);
}

TEST(vector, addxor) {
    Vector vector_one("01001");
    Vector vector_two("01000");
    Vector vector_plus(vector_one.size());
    vector_plus = vector_one + vector_two;
    Vector vector_three("00001");
    ASSERT_EQ(vector_plus == vector_three, true);
    Vector vector_four("11101");
    vector_four += vector_three;
    Vector vector_five("11100");
    ASSERT_EQ(vector_four == vector_five, true);
}

TEST(vector, lexcompare) {
    Vector vector_one("01001");
    Vector vector_two("01000");
    Vector vector_three("000011001");
    ASSERT_EQ(vector_two < vector_one, true);
    ASSERT_EQ(vector_one < vector_three, true);
    std::string str_one("101000101010011010101010101010001001010001101010101010101010101010101010101010010000001");
    std::string str_two("101000101010011010101010101010001001010001101010101010101010101010101010101010011000001");
    Vector vector_a(str_one);
    Vector vector_b(str_two);
    ASSERT_EQ(vector_a < vector_b, true);
    ASSERT_EQ(vector_a > vector_b, false);
}

TEST(vector, shift) {
    for (int32_t size = 12; size < 1079; size +=64) {
        Vector vector(size);
        vector.randomize(size, SHARED_TEST_RNG());
        for (int32_t shift = 0; shift <= size; shift += 16) {
            Vector test_vector(vector);
            test_vector[0]= true;
            test_vector >>= shift;
            ASSERT_EQ(test_vector.lzcnt(), shift);
            test_vector = vector;
            test_vector[size-1]= true;
            test_vector <<= shift;
            ASSERT_EQ(test_vector.tzcnt(), shift);
        }
    }
}