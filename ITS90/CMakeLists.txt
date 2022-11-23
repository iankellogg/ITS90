include_guard(GLOBAL)

cmake_minimum_required(VERSION 3.13.0)

add_library(ITS90 ITS90_RTD.c)
target_sources(ITS90 PUBLIC ITS90/ITS90_RTD.h)
target_include_directories(ITS90 PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})