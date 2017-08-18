# Cmake required.
cmake_minimum_required (VERSION 2.6)
project (imGtp)

# Version number
set (imGtp_VERSION_MAJOR 0)
set (imGtp_VERSION_MINOR 1)

# Set compiler and linker flags for gcc
set (GCC_COVERAGE_LINK_FLAGS "-lm")
set (GCC_COVERAGE_COMPILE_FLAGS "-std=c++1y -Wall -fPIC")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")

# Add the binary tree to the search path
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/include")

# Build our library

add_library(imGtp
    src/Argument.cpp
    src/Command.cpp
    src/Controller.cpp
    src/Engine.cpp
	src/Response.cpp
    src/Move.cpp
    src/Player.cpp
    src/Vertex.cpp
    src/ParserUtil.cpp
)

# Build examples for testing

add_executable(rungnugo
	examples/rungnugo.cpp
)

# Link the examples with the gtp library

target_link_libraries(rungnugo
	imGtp
)

# Add the install targets
install (TARGETS imGtp DESTINATION lib)

install (DIRECTORY include/ DESTINATION "/usr/include/imGtp/")
