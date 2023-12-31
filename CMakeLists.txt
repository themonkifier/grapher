cmake_minimum_required(VERSION 3.26)
project(Grapher)

# grapher

file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/include/*.hh)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(/home/obie/Apps/GLFW/glfw-3.3.9 /home/obie/Apps/GLFW/glfw-3.3.8-build)
include_directories(/home/obie/Apps/glad/include)
find_package(OpenGL REQUIRED)

add_executable(grapher run.cc /home/obie/Apps/glad/src/glad.c ${sources} ${headers})

target_compile_options(grapher PRIVATE -Wall -Wextra -g -std=c++23)

target_link_libraries(grapher glfw)
target_link_libraries(grapher OpenGL::GL)

add_custom_target(Grapher DEPENDS grapher)

# matrices

file(GLOB matrices_sources ${PROJECT_SOURCE_DIR}/src/matrices/*.cc)
file(GLOB matrices_headers ${PROJECT_SOURCE_DIR}/src/matrices/*.hh)

add_executable(matrices src/matrices/matrix.cc ${matrices_sources} ${matrices_headers}) 

target_compile_options(matrices PRIVATE -Wall -Wextra -Wpedantic -g -std=c++23)

# calculator

file(GLOB calculator_sources ${PROJECT_SOURCE_DIR}/src/calculator/*.cc)
file(GLOB calculator_headers ${PROJECT_SOURCE_DIR}/src/calculator/*.hh)

list(REMOVE_ITEM calculator_sources ${PROJECT_SOURCE_DIR}/src/calculator/run.cc)

add_executable(calculator_test src/calculator/test.cc ${calculator_sources} ${calculator_headers}) 

target_compile_options(calculator_test PRIVATE -Wall -Wextra -Wpedantic -g -std=c++23)

list(REMOVE_ITEM calculator_sources ${PROJECT_SOURCE_DIR}/src/calculator/test.cc)
list(APPEND calculator_sources ${PROJECT_SOURCE_DIR}/src/calculator/run.cc)

add_executable(calculator src/calculator/run.cc ${calculator_sources} ${calculator_headers}) 

target_compile_options(calculator PRIVATE -Wall -Wextra -Wpedantic -g -std=c++23)
