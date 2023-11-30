cmake_minimum_required(VERSION 2.8)

if(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  if(DEFINED ENV{VITASDK})
    set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
  else()
    message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
  endif()
endif()

project(raylib_models_mesh_generation)
include("${VITASDK}/share/vita.cmake" REQUIRED)

set(VITA_APP_NAME "RAYLIB 9")
set(VITA_TITLEID  "RAYL00009")
set(VITA_VERSION  "01.00")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVITA -Wall -fno-lto -fno-pic -fno-plt -DRLGL_IMPLEMENTATION -DGRAPHICS_API_OPENGL_ES2 -DSUPPORT_MESH_GENERATION -DPLATFORM_VITA")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(VITA_MKSFOEX_FLAGS "${VITA_MKSFOEX_FLAGS} -d PARENTAL_LEVEL=1")

add_executable(${PROJECT_NAME}
  main.c
)

target_link_libraries(${PROJECT_NAME}
  raylib
  pib
  libScePiglet_stub
  SceShaccCg_stub
  taihen_stub
  SceAppMgr_stub
  SceLibKernel_stub
  #SceThreadmgr_stub
  #SceModulemgr_stub
  SceSysmodule_stub
  SceIofilemgr_stub
  SceNetCtl_stub 
  SceNet_stub
  SceGxm_stub
  SceDisplay_stub
  SceCtrl_stub
  SceTouch_stub
  pthread
  m
)

set(VITA_ELF_CREATE_FLAGS "${VITA_ELF_CREATE_FLAGS}")

vita_create_self(eboot.bin ${PROJECT_NAME} UNSAFE NOSTRIP)

vita_create_vpk(${PROJECT_NAME}.vpk ${VITA_TITLEID} eboot.bin
  VERSION ${VITA_VERSION}
  NAME ${VITA_APP_NAME}
)
