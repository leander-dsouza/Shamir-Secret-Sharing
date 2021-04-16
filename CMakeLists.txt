cmake_minimum_required(VERSION 3.16)
project(SSS C)

set(CMAKE_C_STANDARD 11)


include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src
)

add_executable(SSS src/SSS.c)
set_target_properties(SSS PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
