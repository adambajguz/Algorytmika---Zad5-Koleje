cmake_minimum_required(VERSION 3.9)
project(PS5_ZAD3 C)

set(CMAKE_C_STANDARD 11)

add_executable(PS5_ZAD3 main.c Graph.c Graph.h dijkstra.c dijkstra.h input.c input.h)