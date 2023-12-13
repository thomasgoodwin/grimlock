cmake_minimum_required(VERSION 3.28.0)
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")
project(Grimlock VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_subdirectory("glfw")
add_subdirectory("glew")

file(GLOB SOURCES
  src/*.cpp
)

add_executable(Grimlock ${SOURCES})

target_link_libraries(Grimlock
  glfw
  libglew_static
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/glew/include)
