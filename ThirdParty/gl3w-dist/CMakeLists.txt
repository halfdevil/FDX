cmake_minimum_required(VERSION 3.1.3 FATAL_ERROR)

project(gl3w)

set(CMAKE_VERBOSE_MAKEFILE false)
set(CMAKE_POSITION_INDEPENDENT_CODE ON) # -fPIC

set(SOURCE_DIR src)
set(HEADER_DIR include)

include_directories(include)

# add and depend on OpenGL
find_package(OpenGL REQUIRED)
set(EXTERNAL_INCLUDE_DIRS ${EXTERNAL_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})
set(EXTERNAL_LIBRARIES ${EXTERNAL_LIBRARIES} ${OPENGL_LIBRARIES})

# create gl3w target
add_library(${PROJECT_NAME} STATIC 
	${HEADER_DIR}/GL/gl3w.h
	${HEADER_DIR}/GL/glcorearb.h
	${SOURCE_DIR}/gl3w.c)

set(MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(BUILD_CMAKE_DIR "${CMAKE_CURRENT_BINARY_DIR}/cmake")
