cmake_minimum_required(VERSION 3.10)

project(BitmapPlusPlus
        VERSION "1.1.1"
        DESCRIPTION "Simple and Fast header only Bitmap (BMP) C++ library"
        LANGUAGES CXX
)

if (${CMAKE_SOURCE_DIR} STREQUAL ${PROJECT_SOURCE_DIR})
    set(BUILD_EXAMPLES_DEFAULT ON)
else ()
    set(BUILD_EXAMPLES_DEFAULT OFF)
endif ()

option(BPP_BUILD_EXAMPLES "Requires BPP to build all examples inside examples/ folder." ${BUILD_EXAMPLES_DEFAULT})

set(CMAKE_CXX_STANDARD 17)

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/lib")

if (BPP_BUILD_EXAMPLES)
    enable_testing()
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/examples")
endif ()
