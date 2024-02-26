cmake_minimum_required(VERSION 3.12.0)
project(opengl_project_test_1 VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)



add_subdirectory(lib/glfw-3.3)

find_package(OpenGL REQUIRED)
add_executable(opengl_project
src/config.h
src/main.cpp
"include/glad/glad.c"
src/systems/camera_system.cpp
src/systems/motion_system.cpp
src/systems/render_system.cpp
src/controller/app.cpp
src/view/shader.cpp
src/factories/factory.cpp

) 




target_include_directories(opengl_project PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
target_link_libraries(opengl_project glfw OpenGL::GL ${CMAKE_DL_LIBS})
