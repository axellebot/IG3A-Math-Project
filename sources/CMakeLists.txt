cmake_minimum_required(VERSION 3.8)
project(MiniProjetMath)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")

include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(spdlog CONFIG REQUIRED)

set(SOURCE_FILES
        main.h
        main.cpp
        Point.h
        Algorithm.cpp
        Algorithm.h
        )

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})
