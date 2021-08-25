cmake_minimum_required(VERSION 3.6)
project(gradebook CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FILES
        src/menu/Menu.cpp
        src/menu/Menu.h

        src/example/example.cpp
        src/example/ContactBook/ContactBook.cpp
        src/example/ContactBook/ContactBook.h
        src/example/example.cpp
        )

add_executable(gradebook ${FILES})