add_library(
    drivers.lib

    networking.cpp
    sound.cpp
)

target_include_directories(
    drivers.lib

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)