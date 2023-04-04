# Copyright 2021 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# Based on Craig Gidney's Stim Package https://github.com/quantumlib/Stim

cmake_minimum_required(VERSION 3.13)
project(bits)

set(CMAKE_VERBOSE_MAKEFILE ON)

# Avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24:
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
	cmake_policy(SET CMP0135 NEW)
endif()

include_directories(src)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY out)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY out)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
include(GoogleTest)

# Convert desired SIMD_WIDTH into machine architecture flags.
if(NOT(SIMD_WIDTH))
    set(MACHINE_FLAG "-march=native")
elseif(SIMD_WIDTH EQUAL 256)
    set(MACHINE_FLAG "-mavx2" "-msse2")
elseif(SIMD_WIDTH EQUAL 128)
    set(MACHINE_FLAG "-mno-avx2" "-msse2")
elseif(SIMD_WIDTH EQUAL 64)
    set(MACHINE_FLAG "-mno-avx2" "-mno-sse2")
endif()

# make changes to file_lists trigger a reconfigure
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS file_lists/source_files)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS file_lists/test_files)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS file_lists/benchmark_files)

file(STRINGS file_lists/source_files SOURCE_FILES)
file(STRINGS file_lists/test_files TEST_FILES)
file(STRINGS file_lists/benchmark_files BENCHMARK_FILES)

add_library(libbits ${SOURCE_FILES})
set_target_properties(libbits PROPERTIES PREFIX "")
target_include_directories(libbits PUBLIC src)
if(NOT(MSVC))
    target_compile_options(libbits PRIVATE -O3 -Wall -Wpedantic -fPIC -fno-strict-aliasing ${MACHINE_FLAG})
    target_link_options(libbits PRIVATE -pthread -O3)
else()
    target_compile_options(libbits PRIVATE ${MACHINE_FLAG})
endif()

add_executable(bits_benchmark ${SOURCE_FILES} ${BENCHMARK_FILES})
if(NOT(MSVC))
    target_compile_options(bits_benchmark PRIVATE -Wall -Wpedantic -O3 -fno-strict-aliasing ${MACHINE_FLAG})
    target_link_options(bits_benchmark PRIVATE -pthread)
else()
    target_compile_options(bits_benchmark PRIVATE ${MACHINE_FLAG})
endif()



set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS file_lists/my_tests)
file(STRINGS file_lists/my_tests MYTESTS)


add_executable(identity_test ${MYTESTS})
target_link_libraries(identity_test PRIVATE libbits)
if(NOT(MSVC))
    target_compile_options(identity_test PRIVATE -Wall -Wpedantic -O3 -fno-strict-aliasing ${MACHINE_FLAG})
    target_link_options(identity_test PRIVATE -pthread)
else()
    target_compile_options(identity_test PRIVATE ${MACHINE_FLAG})
endif()



#enable_testing()
#add_executable(bits_test ${SOURCE_FILES} ${TEST_FILES})
#target_link_libraries(bits_test gtest gtest_main)
#target_compile_options(bits_test PRIVATE -Wall -Wpedantic -fno-strict-aliasing -fsanitize=undefined -fsanitize=address ${MACHINE_FLAG})
#target_link_options(bits_test PRIVATE -fsanitize=undefined -fsanitize=address -pthread)
#add_executable(bits_test_o3 ${SOURCE_FILES} ${TEST_FILES})
#target_link_libraries(bits_test_o3 gtest gtest_main)
#target_compile_options(bits_test_o3 PRIVATE -O3 -Wall -Wpedantic -fno-strict-aliasing ${MACHINE_FLAG})
#target_link_options(bits_test_o3 PRIVATE -pthread)


