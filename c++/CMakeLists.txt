cmake_minimum_required(VERSION 3.27)
project(c__)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(c__ main.cpp)
target_link_libraries(c__
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

