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
set(VITA_TITLEID  "VSDK00001")
set(VITA_VERSION  "0.01")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_subdirectory(source)

add_executable(
  ${PROJECT_NAME}

  source/main.cpp
)

target_link_libraries(${PROJECT_NAME}
  m
  vita2d
  SceDisplay_stub
  SceCtrl_stub
  SceAudio_stub
  mpg123
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
  SceSysmodule_stub
  SceTouch_stub
  application.lib
  drivers.lib
  utilities.lib
  tinyxml.lib
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
)