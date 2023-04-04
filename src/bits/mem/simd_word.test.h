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

/// Test utilities for working with various sizes of simd words.

#include "gtest/gtest.h"

#define TEST_EACH_WORD_SIZE_UP_TO_64(test_suite, test_name, ...) \
    TEST(test_suite, test_name##_64) {                           \
        constexpr size_t W = 64;                                 \
        __VA_ARGS__                                              \
    }

#define TEST_EACH_WORD_SIZE_UP_TO_128(test_suite, test_name, ...) \
    TEST(test_suite, test_name##_128) {                           \
        constexpr size_t W = 128;                                 \
        __VA_ARGS__                                               \
    }                                                             \
    TEST(test_suite, test_name##_64) {                            \
        constexpr size_t W = 64;                                  \
        __VA_ARGS__                                               \
    }

#define TEST_EACH_WORD_SIZE_UP_TO_256(test_suite, test_name, ...) \
    TEST(test_suite, test_name##_256) {                           \
        constexpr size_t W = 256;                                 \
        __VA_ARGS__                                               \
    }                                                             \
    TEST(test_suite, test_name##_128) {                           \
        constexpr size_t W = 128;                                 \
        __VA_ARGS__                                               \
    }                                                             \
    TEST(test_suite, test_name##_64) {                            \
        constexpr size_t W = 64;                                  \
        __VA_ARGS__                                               \
    }

#if __AVX2__
#define TEST_EACH_WORD_SIZE_W(test_suite, test_name, ...) \
    TEST_EACH_WORD_SIZE_UP_TO_256(test_suite, test_name, __VA_ARGS__)
#elif __SSE2__
#define TEST_EACH_WORD_SIZE_W(test_suite, test_name, ...) \
    TEST_EACH_WORD_SIZE_UP_TO_128(test_suite, test_name, __VA_ARGS__)
#else
#define TEST_EACH_WORD_SIZE_W(test_suite, test_name, ...) \
    TEST_EACH_WORD_SIZE_UP_TO_64(test_suite, test_name, __VA_ARGS__)
#endif
