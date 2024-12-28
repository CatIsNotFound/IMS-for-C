cmake_minimum_required(VERSION 3.29)
project(IMS)

set(CMAKE_CXX_STANDARD 11)

add_executable(IMS
        main.c
        Funcs/UI.h
        Funcs/UI.c
        Funcs/Graphic.h
        Funcs/Graphic.c
        Funcs/Control.h
        Funcs/Control.c
        Funcs/Datas.h
        Funcs/Datas.c
)
