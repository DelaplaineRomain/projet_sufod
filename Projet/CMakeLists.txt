cmake_minimum_required(VERSION 3.7.0)

project(pgm)

set(CMAKE_BUILD_TYPE debug)
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

file(
	GLOB
	source_files
	src/*.[ch]pp
)

add_executable(
	pgm
	${source_files}
)

target_link_libraries(pgm Qt5::Widgets)
