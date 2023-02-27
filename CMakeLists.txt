cmake_minimum_required(VERSION 3.2.0)
include(CheckCXXCompilerFlag)

project(rubik)

set (CMAKE_VERBOSE_MAKEFILE ON)

if (NOT CMAKE_BUILD_TYPE)
    set (CMAKE_BUILD_TYPE "Release" CACHE STRING "Debug|Release|RelWithDebInfo|MinSizeRel" FORCE)
endif()

check_cxx_compiler_flag (-Wall HAS_WALL)
if (HAS_WALL)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
endif()

check_cxx_compiler_flag (-Wextra HAS_EXTRA)
if (HAS_EXTRA)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
endif()

check_cxx_compiler_flag (-Werror HAS_ERROR)
if (HAS_ERROR)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
endif()

check_cxx_compiler_flag (-Wconversion HAS_CONVERSION)
if (HAS_CONVERSION)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion")
endif()

#find_package (OpenMP)
#if (OpenMP_CXX_FOUND)
#    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
#endif()

check_cxx_compiler_flag (-xHost HAS_XHOST)
check_cxx_compiler_flag (-march=native HAS_MARCH_NATIVE)
if (HAS_XHOST)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -xHost")
elseif (HAS_MARCH_NATIVE)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
endif()

check_cxx_compiler_flag (-ipo HAS_IPO)
if (HAS_IPO)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ipo")
endif()

check_cxx_compiler_flag (-flto HAS_FLTO)
if (HAS_FLTO)
    set (CMAKE_CXX_FLAGS "-flto ${CMAKE_CXX_FLAGS}")
endif()

enable_testing()

add_subdirectory (src)
add_subdirectory (tests)

