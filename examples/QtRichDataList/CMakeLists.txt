cmake_minimum_required(VERSION 3.1.0)

project(QtDemo)

if(WIN32)
message("WARNING: On Windows, please set the Qt5_DIR to the Qt5 CMake configuration path")
set(Qt5_DIR C:/Qt/5.13.0/msvc2017_64/lib/cmake/Qt5)
endif()

include_directories(./sources/qtextrawidgets/headers)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(project_sources
sources/qtextrawidgets/sources/ActiveLabel.cpp
sources/qtextrawidgets/sources/Button.cpp
sources/qtextrawidgets/sources/CentralWidget.cpp
sources/qtextrawidgets/sources//HorizontalLine.cpp 
sources/qtextrawidgets/sources/IndicatorBar.cpp
sources/qtextrawidgets/sources/IndicatorCircular.cpp
sources/qtextrawidgets/sources/PictureBox.cpp 
sources/qtextrawidgets/sources/RichDataList.cpp
sources/qtextrawidgets/sources/RichLabel.cpp
sources/qtextrawidgets/sources//ThemeHandler.cpp 
sources/qtextrawidgets/sources//VerticalLine.cpp 
sources/QtDemo.cpp 
sources/MainWindow.cpp 
sources/main.cpp
)

set(project_headers 
sources/qtextrawidgets/headers/ActiveLabel.h
sources/qtextrawidgets/headers/Button.h
sources/qtextrawidgets/headers/CentralWidget.h
sources/qtextrawidgets/headers//HorizontalLine.h 
sources/qtextrawidgets/headers/IndicatorBar.h
sources/qtextrawidgets/headers/IndicatorCircular.h
sources/qtextrawidgets/headers/PictureBox.h 
sources/qtextrawidgets/headers/RichDataList.h
sources/qtextrawidgets/headers/RichLabel.h
sources/qtextrawidgets/headers//ThemeHandler.h 
sources/qtextrawidgets/headers//VerticalLine.h 
sources/QtDemo.h 
sources/MainWindow.h 
)

if(WIN32)
add_executable(QtDemo WIN32 ${project_sources} ${project_headers} ${project_resources})
else()
add_executable(QtDemo ${project_sources} ${project_headers} ${project_resources})
link_libraries(rt pthread dl)
endif()

target_link_libraries(QtDemo Qt5::Widgets Qt5::Core)
