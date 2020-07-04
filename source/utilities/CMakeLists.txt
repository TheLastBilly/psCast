file(
    GLOB utilities_src
    "*.hpp"
    "*.cpp"
)

add_library(
    utilities.lib

    ${utilities_src}
)

target_include_directories(
    utilities.lib

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)