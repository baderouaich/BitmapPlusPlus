cmake_minimum_required(VERSION 3.10)
project(demo_fc)

set(CMAKE_CXX_STANDARD 17) # C++17 or newer is required

include(FetchContent)
FetchContent_Declare(BitmapPlusPlus
        GIT_REPOSITORY "https://github.com/baderouaich/BitmapPlusPlus"
        GIT_TAG "master"
)
FetchContent_MakeAvailable(BitmapPlusPlus)

add_executable(${PROJECT_NAME}
        "${CMAKE_CURRENT_SOURCE_DIR}/Main.cpp"
)

target_link_libraries(${PROJECT_NAME} LINK_PRIVATE bmp::BitmapPlusPlus)
