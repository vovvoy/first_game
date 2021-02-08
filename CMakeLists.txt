cmake_minimum_required(VERSION 3.17)
project(my_game C)

set(CMAKE_C_FLAGS "-L./mlx -lmlx -L/usr/include/../lib -lXext -lX11 -lm -lbsd")

add_executable(my_game main.c)



#screensize = xdpyinfo | awk '/dimensions/ {print $2}'