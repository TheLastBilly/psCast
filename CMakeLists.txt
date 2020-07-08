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
set(VITA_TITLEID  "PCAST0001")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

file(GLOB SRCS
  "src/*.cpp"
)

add_executable(
  ${PROJECT_NAME}

  src/main.cpp
  ${SRCS}
)

GET_PROPERTY(libs_l GLOBAL PROPERTY LIBS)
target_link_libraries(
  ${PROJECT_NAME}
  vita2d
  freetype
  png
  jpeg
  z
  c
  m
  mpg123
  SceCtrl_stub
  SceAudio_stub
  SceAppMgr_stub
  SceAppUtil_stub
  SceAudio_stub
  SceAudiodec_stub
  SceCommonDialog_stub
  SceCtrl_stub
  SceDisplay_stub
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
  ${CURL_LIBRARIES}
  ${OPENSSL_LIBRARIES}
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
  FILE sce_sys/Square.ttf system_font.tff
)