cmake_minimum_required(VERSION 3.12.0)
project(opengl_project_1 VERSION 0.1.0)
cmake_policy(SET CMP0072 NEW)





add_subdirectory(lib/glfw-3.3)
add_subdirectory(lib/eigen-3.4.0)
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
link_directories(${Eigen_INCLUDE_DIRS})

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
src/factories/sphere.cpp
src/factories/octahedron.cpp
src/factories/cube.cpp
src/physics/softbody.cpp
src/physics/rigidbody.cpp
) 


if(MSVC)
  target_compile_options(opengl_project  PRIVATE /W4 )
else()
  target_compile_options(opengl_project PRIVATE -Wall -Wextra )
endif()

#set(CMAKE_C_FLAGS "-Og -g3")

set(CMAKE_CXX_FLAGS  "-g")
set(CMAKE_BUILD_TYPE Debug)

target_link_libraries(opengl_project Eigen3::Eigen)

target_include_directories(opengl_project PUBLIC
                           $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
                           $<INSTALL_INTERFACE:include>)
target_link_libraries(opengl_project glfw OpenGL::GL ${CMAKE_DL_LIBS})
