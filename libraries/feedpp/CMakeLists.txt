get_filename_component(MODULE ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(
    GLOB SRCS
    "*.cpp"
    "*.hpp"
    
    "*.c"
    "*.h"

    "${VITASDK}/arm-vita-eabi/include/libxml2/libxml/*.h"
)

add_library(
    ${MODULE}.lib

    ${SRCS}
)

target_include_directories(
    ${MODULE}.lib

    PUBLIC "${VITASDK}/arm-vita-eabi/include/libxml2"
)

target_include_directories(
    ${PROJECT_NAME}

    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
)

GET_PROPERTY(libs_l GLOBAL PROPERTY LIBS)
SET_PROPERTY(GLOBAL PROPERTY LIBS ${libs_l} ${MODULE}.lib)