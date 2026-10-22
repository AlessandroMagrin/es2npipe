cmake_minimum_required(VERSION 3.28)
project(es2npipe C)

set(CMAKE_C_STANDARD 11)


add_executable(server server.c)
add_executable(client client.c)
