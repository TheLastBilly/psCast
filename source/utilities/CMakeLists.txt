add_library(
    utilities.lib

    downloads.cpp
    xml.cpp
)

target_include_directories(
    utilities.lib

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)