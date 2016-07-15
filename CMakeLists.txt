cmake_minimum_required(VERSION 3.5)
project(wav2mp3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Lame)
if ( NOT LAME_FOUND )
    message(FATAL_ERROR "Package LAME required, but not found!")
endif( NOT LAME_FOUND )

include_directories(${LAME_INCLUDE_DIRS})
add_definitions(${LAME_DEFINITIONS})

set(SOURCE_FILES main.cpp Encoder.cpp Encoder.h HandlerManager.h)

add_executable(wav2mp3 ${SOURCE_FILES})
target_link_libraries(wav2mp3 ${LAME_LIBRARIES})
