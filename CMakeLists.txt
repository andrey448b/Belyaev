cmake_minimum_required(VERSION 3.10)
project(WarehouseApp LANGUAGES CXX)

# Enforce C++17 standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Source files
set(SOURCES
    src/main.cpp
    src/Product.h
    src/Product.cpp
    src/Warehouse.h
    src/Warehouse.cpp
    src/FileIO.h
    src/FileIO.cpp
    src/Menu.h
    src/Menu.cpp
)

# Executable target
add_executable(WarehouseApp ${SOURCES})

# Include directory setup
target_include_directories(WarehouseApp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# High compiler warnings for robust code
if(MSVC)
    target_compile_options(WarehouseApp PRIVATE /W4 /WX-)
else()
    target_compile_options(WarehouseApp PRIVATE -Wall -Wextra -Wpedantic)
endif()
