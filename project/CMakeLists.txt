
cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(Project CoreLib)
project(${Project})

add_subdirectory(database)
add_subdirectory(core)
add_subdirectory(networking)
add_subdirectory(qtextrawidgets)

