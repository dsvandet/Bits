#include "bits/algebra/vector.h"

using namespace bits;

// Public methods

Vector::Vector(size_t size)
    : m_size_bits(size), m_simd_bits(size) {}

Vector::Vector(const Vector& vector) 
    : m_size_bits(vector.m_size_bits), m_simd_bits(vector.m_simd_bits) {}

Vector::Vector(const VectorRef ref_vector)
    : m_size_bits(ref_vector.m_size_bits), m_simd_bits(ref_vector.m_simd_bits_range_ref) {}

Vector::Vector(const std::string str) {
    size_t length = str.length();
    assert(length > 0);
    Vector vector(length);


    std::bitset<64> bits;
    for(size_t i = 0; i < length; i+= 64) {
        std::string chunk = str.substr(i, 64);
        try {
            bits = std::bitset<64>(chunk);
        }
        catch(std::invalid_argument) {throw "Input string not a binary string";}
        for (size_t j = 0; j < 64; j++) {
            vector[i*64+j] = bits[j];
        }
        
    }




std::vector<std::vector<bool>> lines;
    lines.push_back({});

    // Skip indentation.
    while (*text == '\n' || *text == ' ') {
        text++;
    }

    for (const char *c = text; *c;) {
        if (*c == '\n') {
            lines.push_back({});
            c++;
            // Skip indentation.
            while (*c == ' ') {
                c++;
            }
        } else if (*c == '0' || *c == '.' || *c == '_') {
            lines.back().push_back(false);
            c++;
        } else if (*c == '1') {
            lines.back().push_back(true);
            c++;
        } else {
            throw std::invalid_argument(
                "Expected indented characters from \"10._\\n\". Got '" + std::string(1, *c) + "'.");
        }
    }

    // Remove trailing newline.
    if (!lines.empty() && lines.back().empty()) {
        lines.pop_back();
    }

    size_t num_cols = min_cols;
    for (const auto &v : lines) {
        num_cols = std::max(v.size(), num_cols);
    }
    size_t num_rows = std::max(min_rows, lines.size());
    simd_bit_table<W> out(num_rows, num_cols);
    for (size_t row = 0; row < lines.size(); row++) {
        for (size_t col = 0; col < lines[row].size(); col++) {
            out[row][col] = lines[row][col];
        }
    }

    return out;
}
    
    
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
