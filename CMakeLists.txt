cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(AnyToneFreqAdjust)

set(
    PROG_SRCS
    AnyToneFreqAdjust/main.cpp
    AnyToneFreqAdjust/CSVFile.cpp
)

add_executable(${PROJECT_NAME} ${PROG_SRCS})