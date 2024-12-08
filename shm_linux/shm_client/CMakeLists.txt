cmake_minimum_required(VERSION 3.17)
project(shm_client)

set(CMAKE_CXX_STANDARD 14)

add_executable(shm_client main.cpp h_client.h)