cmake_minimum_required(VERSION 3.17)
project (MultiQueueProcessor)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17")
endif(MSVC)

set(CMAKE_CXX_STANDARD 17)

add_executable(MultiQueueProcessor main.cpp)



