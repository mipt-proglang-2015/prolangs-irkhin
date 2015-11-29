По поводу CMakeLists
Во-первых, у меня на ноуте ужалось поставить только cmake 2.8 поэтому стоит cmake_minimum_required(VERSION 2.8) а не 3.0
Во-вторых, пришлось добавить set(Python_ADDITIONAL_VERSIONS 3.4) чтобы у меня нашлась PythonLibs
В-третьих, я использовал с++11, поэтому нужна строка set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
