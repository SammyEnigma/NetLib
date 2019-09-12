cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(QtExtraWidgets)

if(WIN32)
message("WARNING: On Windows, please set the Qt5_DIR to the Qt5 CMake configuration path")
set(Qt5_DIR C:/Qt/5.13.0/msvc2017_64/lib/cmake/Qt5)
endif()

find_package(Qt5 COMPONENTS Core Widgets Gui REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(project_sources
./sources/ActiveLabel.cpp 
./sources/Button.cpp 
./sources/CentralWidget.cpp 
./sources/DataList.cpp 
./sources/HorizontalLine.cpp 
./sources/IndicatorBar.cpp 
./sources/IndicatorCircular.cpp 
./sources/PictureBox.cpp 
./sources/RichDataList.cpp 
./sources/RichLabel.cpp 
./sources/ThemeHandler.cpp 
./sources/VerticalLine.cpp 
)

set(project_headers 
./headers/ActiveLabel.cpp 
./headers/Button.cpp 
./headers/CentralWidget.cpp 
./headers/DataList.cpp 
./headers/HorizontalLine.cpp 
./headers/IndicatorBar.cpp 
./headers/IndicatorCircular.cpp 
./headers/PictureBox.cpp 
./headers/RichDataList.cpp 
./headers/RichLabel.cpp 
./headers/ThemeHandler.cpp 
./headers/VerticalLine.cpp 
)

target_link_libraries(${project} Qt5::Widgets Qt5::Core Qt5::Gui)
add_library(${project} ${project_sources} ${project_headers})
