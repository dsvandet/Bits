// Copyright 2021 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Original Files from Craig Gidney's Stim Package https://github.com/quantumlib/Stim

#include <cstddef>

#ifndef _BITS_MEM_SIMD_WORD_H
#define _BITS_MEM_SIMD_WORD_H

namespace bits {

/// A `bitword` is a bag of bits that can be operated on in a SIMD-esque fashion
/// by individual CPU instructions.
///
/// This template would not have to exist, except that different architectures and
/// operating systems expose different interfaces between native types like
/// uint64_t and intrinsics like __m256i. For example, in some contexts, __m256i
/// values can be operated on by operators (e.g. you can do `a ^= b`) while in
/// other contexts this does not work. The bitword template implementations define
/// a common set of methods required by bits to function, so that the same code
/// can be compiled to use 256 bit registers or 64 bit registers as appropriate.
template <size_t bit_size>
struct bitword;

}  // namespace bits

#if __AVX2__
#include "bits/mem/simd_word_128_sse.h"
#include "bits/mem/simd_word_256_avx.h"
#include "bits/mem/simd_word_64_std.h"
namespace bits {
constexpr size_t MAX_BITWORD_WIDTH = 256;
}  // namespace bits
#elif __SSE2__
#include "bits/mem/simd_word_128_sse.h"
#include "bits/mem/simd_word_64_std.h"
namespace bits {
constexpr size_t MAX_BITWORD_WIDTH = 128;
}  // namespace bits
#else
#include "bits/mem/simd_word_64_std.h"
namespace bits {
constexpr size_t MAX_BITWORD_WIDTH = 64;
}  // namespace bits
#endif

namespace bits {
typedef bitword<MAX_BITWORD_WIDTH> simd_word;
}  // namespace bits

#endif
