CMAKE_MINIMUM_REQUIRED(VERSION 2.8.2)
PROJECT(GL31)
SET(CMAKE_MODULE_PATH ${GL31_SOURCE_DIR})
SET(EXECUTABLE_OUTPUT_PATH ${GL31_SOURCE_DIR}/bin)
set(CMAKE_CXX_FLAGS "-g -Wall")

find_package(OpenGL) #and GLU
IF (OPENGL_FOUND)
  MESSAGE("OpenGL found")
ELSE(OPENGL_FOUND)
  MESSAGE(FATAL_ERROR "OpenGL not found")
ENDIF(OPENGL_FOUND)

find_package(SDL)
IF (SDL_FOUND)
  MESSAGE("SDL found")
ELSE(SDL_FOUND)
  MESSAGE(FATAL_ERROR "Do you have sdl? (on Ubuntu 11.04 it's libsdl1.2-dev package)")
ENDIF(SDL_FOUND)

set(CORELIBS ${OPENGL_LIBRARIES} ${SDL_LIBRARY})

ADD_EXECUTABLE(main  main.cpp)

target_link_libraries(main ${CORELIBS})
