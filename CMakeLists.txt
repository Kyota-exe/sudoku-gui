cmake_minimum_required(VERSION 3.22)
project(sudoku CXX)

set(CMAKE_CXX_STANDARD 23)

set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR})

# CONFIG option is important so that CMake doesnt search for modules into the default modules directory
find_package(imgui CONFIG)
find_package(glfw3 CONFIG)
find_package(glew CONFIG)

add_executable( sudoku
                main.cpp
                style.cpp
                style.h
                State.cpp
                State.h
                GUI.cpp
                GUI.h
                Solver.cpp
                Solver.h
                bindings/imgui_impl_glfw.cpp
                bindings/imgui_impl_opengl3.cpp
                bindings/imgui_impl_glfw.h
                bindings/imgui_impl_opengl3.h )

target_compile_definitions(sudoku PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(sudoku imgui::imgui GLEW::glew_s glfw::glfw)
