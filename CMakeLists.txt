cmake_minimum_required(VERSION 3.20)
project(qTTWTime)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_PREFIX_PATH "/opt/Qt/6.3.0/gcc_64/")

find_package(Qt6Widgets REQUIRED)
find_package(Qt6Core REQUIRED)
find_package(Qt6SerialPort REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core5Compat)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_executable(qTTWTime
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        tinygpspp.h
        tinygpspp.cpp
        )
target_link_libraries(qTTWTime Qt6::Widgets Qt6::Core Qt6::SerialPort Qt6::Core5Compat m)