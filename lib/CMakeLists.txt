cmake_minimum_required(VERSION 3.10)

add_library(BitmapPlusPlus INTERFACE)
target_include_directories(BitmapPlusPlus INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_library(bmp::BitmapPlusPlus ALIAS BitmapPlusPlus)
