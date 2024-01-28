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

include_directories("/opt/homebrew/Cellar/mysql-connector-c++/8.3.0/include/")


link_directories("/opt/homebrew/Cellar/mysql-connector-c++/8.3.0/lib/")

add_executable(c__ main.cpp
)

target_link_libraries(c__
        Qt::Core
        Qt::Gui
        Qt::Widgets
)

target_link_libraries(c__
        mysqlcppconn8
)
