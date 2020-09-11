# Set the minimum required version of CMake for this project.
cmake_minimum_required(VERSION 2.8)

set(project OpenVR-BIAS-examples-actionmanifest)
project(
	${project}
	DESCRIPTION "Reading/Writing/Modifying action manifest reading/writing/modification"
	LANGUAGES CXX
	)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

add_executable(${project} main.cpp)
