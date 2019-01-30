cmake_minimum_required(VERSION 2.8)

#common properties
include(config.cmake)
enable_testing()

set(CATCH_LIB_PROJ_PATH ${CMAKE_SOURCE_DIR}/Cross/Catch)
set(CATCH_SINGLE_INCLUDE_DIR ${CATCH_LIB_PROJ_PATH}/single_include/)
find_package(Boost 1.58 REQUIRED COMPONENTS system filesystem thread program_options iostreams)

#build
add_subdirectory(Examples)
