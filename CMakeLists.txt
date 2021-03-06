cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()


project(pscast)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "psCast")
set(VITA_TITLEID  "PCAST0001" CACHE INTERNAL "")
set(VITA_VERSION  "02.10")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

set(SOURCE_DIR "source")
set(INCLUDE_DIR "${SOURCE_DIR}/include")

file(GLOB SRCS
  "${SOURCE_DIR}/*.cpp"
  "${INCLUDE_DIR}/*.hpp"
)

SET_PROPERTY(GLOBAL PROPERTY LIBS "")
macro(add_module)
  get_filename_component(MODULE ${CMAKE_CURRENT_SOURCE_DIR} NAME)

  file(
      GLOB SRCS
      "*.cpp"
      "*.hpp"
      
      "*.c"
      "*.h"
  )

  add_library(
      ${MODULE}.lib

      ${SRCS}
  )

  target_include_directories(
      ${PROJECT_NAME}

      PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}
  )

  GET_PROPERTY(libs_l GLOBAL PROPERTY LIBS)
  SET_PROPERTY(GLOBAL PROPERTY LIBS ${libs_l} ${MODULE}.lib)
endmacro()

add_executable(
  ${PROJECT_NAME}

  ${SRCS}
)

add_subdirectory(libraries)

target_include_directories(
  ${PROJECT_NAME}
  PUBLIC "${INCLUDE_DIR}"
)

GET_PROPERTY(libs_l GLOBAL PROPERTY LIBS)
target_link_libraries(
  ${PROJECT_NAME}
  vita2d
  freetype
  png
  jpeg
  c
  m
  mpg123
  SceAppMgr_stub
  SceAppUtil_stub
  SceAudio_stub
  SceAudiodec_stub
  SceCommonDialog_stub
  SceCtrl_stub
  SceGxm_stub
  ScePower_stub
  SceRtc_stub
  SceShellSvc_stub
  SceDisplay_stub
  SceIofilemgr_stub
  SceSysmodule_stub
  SceTouch_stub
  SceNet_stub
  SceNetCtl_stub
  SceHttp_stub
  SceSsl_stub
  freetype
  xml2
  z
  ${CURL_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ${libs_l}
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME} UNSAFE)
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/icon0.png sce_sys/icon0.png
  FILE sce_sys/livearea/contents/bg.png sce_sys/livearea/contents/bg.png
  FILE sce_sys/livearea/contents/startup.png sce_sys/livearea/contents/startup.png
  FILE sce_sys/livearea/contents/template.xml sce_sys/livearea/contents/template.xml
  FILE sce_sys/Square.ttf system_font.tff
)