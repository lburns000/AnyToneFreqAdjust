cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED On)

project(AnyToneFreqAdjust)

set(
    PROG_SRCS
    AnyToneFreqAdjust/main.cpp
    AnyToneFreqAdjust/CSVFile.cpp
    AnyToneFreqAdjust/MainFrame.cpp
    AnyToneFreqAdjust/ChannelOffsetListBox.cpp
)

set(INCLUDE_PATH AnyToneFreqAdjust)

find_package(wxWidgets COMPONENTS adv core base xrc REQUIRED)

include(${wxWidgets_USE_FILE} ${INCLUDE_PATH})

add_executable(${PROJECT_NAME} ${PROG_SRCS})

target_link_libraries(${PROJECT_NAME} ${wxWidgets_LIBRARIES})