cmake_minimum_required(VERSION 3.5)
project(wav2mp3)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(Lame)
if ( NOT LAME_FOUND )
    message(FATAL_ERROR "Package LAME required, but not found!")
endif( NOT LAME_FOUND )

set(CMAKE_THREAD_PREFER_PTHREAD)
find_package (Threads)
if ( NOT CMAKE_USE_PTHREADS_INIT )
    message(FATAL_ERROR "PThread library not found!")
endif( NOT CMAKE_USE_PTHREADS_INIT )

include_directories(${LAME_INCLUDE_DIRS})
add_definitions(${LAME_DEFINITIONS})

set(SOURCE_FILES main.cpp Encoder.cpp Encoder.h HandlerManager.h WAVHeader.h Processor.cpp Processor.h Filesystem.cpp Filesystem.h)

add_executable(wav2mp3 ${SOURCE_FILES})
target_link_libraries(wav2mp3 ${LAME_LIBRARIES})
target_link_libraries (wav2mp3 ${CMAKE_THREAD_LIBS_INIT})
