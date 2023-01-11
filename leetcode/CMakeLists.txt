cmake_minimum_required(VERSION 3.23)
project(leetcode)
find_package(OpenCV REQUIRED)
set(CMAKE_CXX_STANDARD 17)
INCLUDE_DIRECTORIES(/home/xu736946693/Desktop/DataStructure/lib)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(leetcode main.cpp)
TARGET_LINK_LIBRARIES(leetcode /home/xu736946693/Desktop/DataStructure/build/bin/libDataStructure_shared.so )
target_link_libraries(leetcode ${OpenCV_LIBS})

