cmake_minimum_required(VERSION 3.16)

project(pokeapi-wrapper-cpp-qt LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Gui Network Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Gui Network Test)

add_executable(pokeapi-wrapper-cpp-qt
  main.cpp
  model/pokemon.h model/pokemon.cpp
  model/json_keys.h
)
target_link_libraries(pokeapi-wrapper-cpp-qt
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Network)

include(GNUInstallDirs)
install(TARGETS pokeapi-wrapper-cpp-qt
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

add_subdirectory(tests)
