# Set the minimum required version of CMake for this project.
cmake_minimum_required(VERSION 2.8)

# Set project name.
set(project OpenVR-BIAS)

project(
	${project}
	DESCRIPTION "Qt wrapper around the OpenVR API and SDK"
	LANGUAGES CXX
	)

# Build options
option(BUILD_EXAMPLES "Builds the example programs for the library" ON)

# Set a default build type if none was specified
set(default_build_type "Debug")
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to '${default_build_type}' as none was specified.")
  set(CMAKE_BUILD_TYPE "${default_build_type}" CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Set up Qt build
find_package(
	Qt5
	COMPONENTS
	    Core
		Gui
		OpenGL
		Widgets
		Concurrent
	REQUIRED
)
if (NOT Qt5Core_FOUND)
	message(FATAL_ERROR "No Qt installation found, aborting")
endif()
set(Qt5_VERSION ${Qt5Core_VERSION})
list(APPEND ${project}_LIBS ${Qt5Core_LIBRARIES}    ${Qt5Gui_LIBRARIES}    ${Qt5OpenGL_LIBRARIES}    ${Qt5Widgets_LIBRARIES}    ${Qt5Concurrent_LIBRARIES})
list(APPEND ${project}_DIRS ${Qt5Core_INCLUDE_DIRS} ${Qt5Gui_INCLUDE_DIRS} ${Qt5OpenGL_INCLUDE_DIRS} ${Qt5Widgets_INCLUDE_DIRS} ${Qt5Concurrent_INCLUDE_DIRS})
get_target_property (QT_QMAKE_EXECUTABLE Qt5::qmake IMPORTED_LOCATION)
macro(qt_add_resources)
   qt5_add_resources(${ARGN})
endmacro()
macro(qt_wrap_cpp)
	qt5_wrap_cpp(${ARGN})
endmacro()
macro(qt_wrap_ui)
	qt5_wrap_ui(${ARGN})
endmacro()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_subdirectory(${SOURCE_DIR}/openvr)

# Include source files
add_library(${project}
	${SOURCE_DIR}/vraction.cpp
	${SOURCE_DIR}/vractionset.cpp
	${SOURCE_DIR}/vraction.h
	${SOURCE_DIR}/vractionset.h
	${SOURCE_DIR}/vrbinding.h
	${SOURCE_DIR}/vrbinding.cpp
	${SOURCE_DIR}/vrmanifest.h
	${SOURCE_DIR}/vrmanifest.cpp
	)

include_directories(${${project}_DIRS})
target_link_libraries(${project} ${${project}_LIBS})

# Build examples
if(BUILD_EXAMPLES)
	add_subdirectory(examples)
endif()
