cmake_minimum_required(VERSION 3.10)
project(BitmapPlusPlus VERSION "1.0.1" DESCRIPTION "Simple and Fast header only Bitmap (BMP) C++ library" LANGUAGES CXX)

# Examples
option(BPP_BUILD_EXAMPLES "Requires BPP to build all examples inside examples/ folder." ON)
if(BPP_BUILD_EXAMPLES)
    file(GLOB_RECURSE EXAMPLES_FILES ${CMAKE_SOURCE_DIR}/examples/*.cpp)
    foreach(example_file ${EXAMPLES_FILES})
        message(STATUS "Adding example ${example_file}")
        get_filename_component(target_name ${example_file} NAME_WE)
        include_directories(${target_name} ${CMAKE_SOURCE_DIR}/include/)
        add_executable(${target_name} ${example_file})
    endforeach()
endif()