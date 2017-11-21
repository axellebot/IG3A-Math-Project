cmake_minimum_required(VERSION 3.8)
project(MiniProjetMath)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
# For OpenGL and GLUT
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

set(SOURCE_FILES
        main.h
        main.cpp
        constants.h
        Point.h
        Algorithm.cpp
        Algorithm.h
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
