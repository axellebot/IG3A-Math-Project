cmake_minimum_required(VERSION 3.8)
project(sources)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

find_package (OpenGL REQUIRED)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        main.cpp
        constants.h
        Point.h
        Algorithm.cpp
        Algorithm.h)

add_executable(sources ${SOURCE_FILES})

# For OpenGL and GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
