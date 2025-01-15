cmake_minimum_required( VERSION 3.10 )

include ( CPM.cmake )

# either define GIT_BRANCH at configure time by "cmake -D GIT_BRANCH=master .."
# or it is set automatically in CI
CPMAddPackage ( "gh:nerudaj/BitmapPlusPlus#${GIT_BRANCH}" )

project ( demo )

add_executable( ${PROJECT_NAME}
    "${CMAKE_CURRENT_SOURCE_DIR}/../../examples/draw_primitives.cpp"
)

target_link_libraries( ${PROJECT_NAME} LINK_PRIVATE bpp::BitmapPlusPlus )
