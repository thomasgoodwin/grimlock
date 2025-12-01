cmake_minimum_required(VERSION 3.28.0)

project(Grimlock VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

add_subdirectory(glfw)
add_subdirectory(glew)
add_subdirectory(glm)

file(GLOB_RECURSE SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

set(ASSETS_DIR "${PROJECT_SOURCE_DIR}/assets")
set(OUTPUT_ASSETS_DIR "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${ASSETS_DIR}"
  "${OUTPUT_ASSETS_DIR}"
  COMMENT "Copying assets to build output directory..."
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /FS)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}/glfw/include
  ${CMAKE_CURRENT_SOURCE_DIR}/glew/include
  ${CMAKE_CURRENT_SOURCE_DIR}/glm/glm
  ${CMAKE_CURRENT_SOURCE_DIR}/stb/stb
  ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PUBLIC
  glfw
  libglew_static
  glm::glm
)

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/shaders/*.frag"
  "${PROJECT_SOURCE_DIR}/shaders/*.vert"
  "${PROJECT_SOURCE_DIR}/textures/*"
)

include(CPack)