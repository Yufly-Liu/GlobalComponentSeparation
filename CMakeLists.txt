cmake_minimum_required(VERSION 3.28)
project(GlobalComponentSeparation)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR F:\\opencv\\build)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_DIR} header)

add_executable(${PROJECT_NAME} main.cpp
        header/QuickDemo.h
        header/config.h
        header/separation.h
)

target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})