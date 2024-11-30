cmake_minimum_required(VERSION 3.16)

project(Guess-The-Prize LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Guess-The-Prize main.cpp
    qrcodegen.cpp
    qrcodegen.hpp)

include(GNUInstallDirs)
install(TARGETS Guess-The-Prize
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
