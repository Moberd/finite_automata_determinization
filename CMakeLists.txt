cmake_minimum_required(VERSION 3.20)
project(FiniteStateAutomata)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(finite_state_automata
        finite_state_automata.h
        main.cpp)
