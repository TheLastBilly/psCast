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

set(VITA_APP_NAME "psCast Podcast Client")
set(VITA_TITLEID  "VSDK00014")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(CURL REQUIRED)
find_package(OpenSSL REQUIRED)

add_executable(
  ${PROJECT_NAME}

  src/main.cpp
)

SET_PROPERTY(GLOBAL PROPERTY LIBS "")
macro(add_module)
  get_filename_component(MODULE ${CMAKE_CURRENT_SOURCE_DIR} NAME)

  file(
      GLOB SRCS
      "*.cpp"
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

add_subdirectory(src)

GET_PROPERTY(libs_l GLOBAL PROPERTY LIBS)
target_link_libraries(
  ${PROJECT_NAME}
  m
  vita2d
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
  SceDisplay_stub
  SceIofilemgr_stub
  SceSysmodule_stub
  SceTouch_stub
  SceNet_stub
  SceNetCtl_stub
  SceHttp_stub
  SceSsl_stub
  ${CURL_LIBRARIES}
  ${OPENSSL_LIBRARIES}
  ${libs_l}
  c
  m
  z
)

vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME})
vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} ${PROJECT_NAME}.self
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
)