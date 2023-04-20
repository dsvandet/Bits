#include "bits/algebra/vector.h"

using namespace bits;

// Public methods

Vector::Vector(size_t size)
    : m_size_bits(size), m_simd_bits(size) {}

Vector::Vector(const Vector& vector) 
    : m_size_bits(vector.m_size_bits), m_simd_bits(vector.m_simd_bits) {}

Vector::Vector(const VectorRef ref_vector)
    : m_size_bits(ref_vector.m_size_bits), m_simd_bits(ref_vector.m_simd_bits_range_ref) {}

Vector::Vector(const std::string str) 
    : m_size_bits(str.length()), m_simd_bits(m_size_bits) {
    assert(m_size_bits > 0);

    for(size_t i = 0; i < m_size_bits; i++) {
        if (str[i] == '0' || str[i] == '.' || str[i] == '_')
            m_simd_bits[i] = 0;
        else if (str[i] == '1')
            m_simd_bits[i] = 1;
        else
            throw std::invalid_argument("Input string not a binary string. Got" + str);
    }
}

Vector::Vector(const char* const str, size_t length) 
    : m_size_bits(length), m_simd_bits(length) {
    *this = Vector(std::string(str, length));
}

VectorRef Vector::ref() {
    return VectorRef(this->m_simd_bits, 0, this->m_size_bits);
}

size_t Vector::length() const {
    return this->size();
}

size_t Vector::size() const {
    return this->m_size_bits;
}

simd_bits<MAX_BITWORD_WIDTH> Vector::get_simd_bits() const {
    simd_bits<MAX_BITWORD_WIDTH> simd_bits(this->m_simd_bits);
    return simd_bits;
}

bit_ref Vector::operator[](int_fast32_t index) {
    if (index < 0 || index >= this->m_size_bits)
        throw std::out_of_range("Vector index out of range"+std::to_string(index));
    return this->m_simd_bits[index];
}

const bool Vector::operator[](int_fast32_t index) const {
    if (index < 0 || index >= this->m_size_bits)
        throw std::out_of_range("Vector index out of range"+std::to_string(index));
    return bool(this->m_simd_bits[index]);
}

Vector& Vector::operator=(const Vector& rhs) {
    Vector vector(rhs);
    return vector;
}

bool Vector::operator==(const Vector& rhs) const {
    return (this->m_size_bits == rhs.m_size_bits && this->m_simd_bits == rhs.m_simd_bits);
}

bool Vector::operator!=(const Vector& rhs) const {
    return (this->m_size_bits != rhs.m_size_bits || this->m_simd_bits != rhs.m_simd_bits);
}


Vector Vector::operator+(const Vector& rhs) {
    if (this->m_size_bits != rhs.m_size_bits)
        throw  std::invalid_argument("Both vectors need to be the same length");
    Vector vector(*this);
    vector.m_simd_bits ^= rhs.m_simd_bits;
    return vector;
}

Vector Vector::operator+=(const Vector& rhs) {
    if (this->m_size_bits != rhs.m_size_bits)
        throw  std::invalid_argument("Both vectors need to be the same length");
    this->m_simd_bits ^= rhs.m_simd_bits;
    return *this;
}

Vector Vector::operator^(const Vector& rhs) {
    if (this->m_size_bits != rhs.m_size_bits)
        throw  std::invalid_argument("Both vectors need to be the same length");
    Vector vector(*this);
    vector.m_simd_bits ^= rhs.m_simd_bits;
    return vector;
}

Vector Vector::operator^=(const Vector& rhs) {
    if (this->m_size_bits != rhs.m_size_bits)
        throw  std::invalid_argument("Both vectors need to be the same length");
    this->m_simd_bits ^= rhs.m_simd_bits;
    return *this;
}

Vector Vector::operator-(const Vector& rhs) {
    if (this->m_size_bits != rhs.m_size_bits)
        throw  std::invalid_argument("Both vectors need to be the same length");
    Vector vector(*this);
    vector.m_simd_bits ^= rhs.m_simd_bits;
    return vector;
}

Vector Vector::operator-=(const Vector& rhs) {
    if (this->m_size_bits != rhs.m_size_bits)
        throw  std::invalid_argument("Both vectors need to be the same length");
    this->m_simd_bits ^= rhs.m_simd_bits;
    return *this;
}

Vector Vector::operator*(const Vector& rhs) {
    if (this->m_size_bits != rhs.m_size_bits)
        throw  std::invalid_argument("Both vectors need to be the same length");
    Vector vector(*this);
    vector.m_simd_bits &= rhs.m_simd_bits;
    return vector;
}

Vector Vector::operator*=(const Vector& rhs) {
    if (this->m_size_bits != rhs.m_size_bits)
        throw  std::invalid_argument("Both vectors need to be the same length");
    this->m_simd_bits &= rhs.m_simd_bits;
    return *this;
}

Vector Vector::operator&(const Vector& rhs) {
    if (this->m_size_bits != rhs.m_size_bits)
        throw  std::invalid_argument("Both vectors need to be the same length");
    Vector vector(*this);
    vector.m_simd_bits &= rhs.m_simd_bits;
    return vector;
}

Vector Vector::operator&=(const Vector& rhs) {
    if (this->m_size_bits != rhs.m_size_bits)
        throw  std::invalid_argument("Both vectors need to be the same length");
    this->m_simd_bits &= rhs.m_simd_bits;
    return *this;
}

// Note: This only works if those bits in the simd_bits that are not within the vectors
// range are zero. If they may not be zero then extra work in necessary to make sure that 
// non zero bits are not pushed on the vectors bitstring.
Vector Vector::operator<<(int_fast32_t shift) const {
    // I think it is pretty safe to predict that shift is almost always not zero.
    if (__builtin_expect(shift != 0, 1))
    {
        const size_t g_shift = shift / 64;
        const size_t l_shift = shift % 64; 

        uint64_t* limb = this->m_simd_bits.u64;
        const uint64_t bound = this->m_simd_bits.num_u64_padded();

        if (l_shift == 0)
          for (size_t n = bound - 1; n >= g_shift; --n)
            limb[n] = limb[n - g_shift];
        else
        {
            const size_t comp_l_shift = 64 - l_shift;

            for (size_t n = bound - 1; n > g_shift; --n)
                limb[n] = ((limb[n - g_shift] << l_shift)
                   | (limb[n - g_shift - 1] >> comp_l_shift));

            limb[g_shift] = limb[0] << l_shift;
        } 
        std::fill(limb + 0, limb + g_shift, 0ULL);
    }

}

Vector Vector::operator>>(int_fast32_t shift) const {
    // I think it is pretty safe to predict that shift is almost always not zero.
    if (__builtin_expect(shift != 0, 1))
    {
        const size_t g_shift = shift / 64;
        const size_t l_shift = shift % 64;
        const uint64_t bound = this->m_simd_bits.num_u64_padded();
        const size_t comp_bound = bound - g_shift - 1;  
        uint64_t* limb = this->m_simd_bits.u64;

        if (l_shift == 0)
            for (size_t n = 0; n <= comp_bound; ++n)
                limb[n] = limb[n + g_shift];
        else {
                const size_t comp_l_shift = (64 - l_shift);
                for (size_t n = 0; n < comp_bound; ++n)
                    limb[n] = ((limb[n + g_shift] >> l_shift)
                        | (limb[n + g_shift + 1] << comp_l_shift));
                limb[comp_bound] = limb[bound-1] >> l_shift;
            }   
        std::fill(limb + comp_bound + 1, limb + bound, 0ULL);
    }
}


bool Vector::operator< (const Vector &rhs) const {
    int_fast8_t diff = SIGNUM(DIFF(this->m_size_bits, rhs.length())); 
    // Check if the lengths of the vectors differ and if so return the correct result.
    if (diff) return (bool)(diff - 1);

    // Given vectors are the same length, so see if/where they are different
    int_fast32_t i = 0, ctz;
    uint64_t m; 
 
    while (i < this->m_simd_bits.num_u64_padded()) {
      m = this->m_simd_bits.u64[i] ^ rhs.m_simd_bits.u64[i];
      if (m == 0) {
        i++;
        continue;
      }
      ctz = tzcnt64(m); // find number of trailing zeros 
                        // (note that binary rep is reversed)
      diff = (1ULL << ctz) & this->m_simd_bits.u64[i];
      return (bool)(!diff);
    }
    return false;  // they are equal
}

bool Vector::operator> (const Vector &rhs) const {
    return (rhs < *this);
}


std::ostream &operator<<(std::ostream &op_ostream, const Vector& rhs) {
  op_ostream << "[";
  for (int_fast32_t i = 0; i < rhs.length(); ++i)
    op_ostream << rhs[i];
  op_ostream << "]";
  return op_ostream;
}