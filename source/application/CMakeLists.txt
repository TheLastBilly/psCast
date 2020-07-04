add_library(
    application.lib

    menu.cpp
)

target_include_directories(
    application.lib

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)