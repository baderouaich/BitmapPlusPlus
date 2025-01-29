cmake_minimum_required(VERSION 3.10)

file(GLOB_RECURSE EXAMPLES_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

foreach (example_file ${EXAMPLES_FILES})
    message(STATUS "Adding example ${example_file}")
    get_filename_component(target_name ${example_file} NAME_WE)

    add_executable(${target_name} ${example_file})
    target_link_libraries(${target_name} bmp::BitmapPlusPlus)
    target_compile_definitions(${target_name} PRIVATE "ROOT_DIR=\"${PROJECT_SOURCE_DIR}\"")
    target_compile_definitions(${target_name} PRIVATE "BIN_DIR=\"${PROJECT_BINARY_DIR}\"")

    add_test(
            NAME ${target_name}
            COMMAND ${target_name}
    )
endforeach ()
