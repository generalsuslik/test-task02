cmake_minimum_required(VERSION 3.30)
project(test_task02)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
  Core
  Gui
  Widgets
  Network
  REQUIRED)

add_executable(test_task02 main.cpp GridApp.cpp)
target_link_libraries(test_task02
  Qt5::Core
  Qt5::Gui
  Qt5::Widgets
  Qt5::Network
)

