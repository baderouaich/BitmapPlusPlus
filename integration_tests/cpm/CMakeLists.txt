cmake_minimum_required(VERSION 3.10)
project(demo)
set(CMAKE_CXX_STANDARD 17) # C++17 or newer is required

include(CPM.cmake)
CPMAddPackage("gh:baderouaich/BitmapPlusPlus#master")

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp")

target_link_libraries(${PROJECT_NAME} LINK_PRIVATE bmp::BitmapPlusPlus)
