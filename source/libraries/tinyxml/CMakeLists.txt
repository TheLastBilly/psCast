add_library(
    tinyxml.lib

    tinystr.cpp
    tinyxml.cpp
    tinyxmlerror.cpp
    tinyxmlparser.cpp
)

target_include_directories(
    tinyxml.lib

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)