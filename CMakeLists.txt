cmake_minimum_required(VERSION 3.0.0)
project(FrontEnd VERSION 0.1.0)

find_package(OpenGL REQUIRED)
find_package(GLEW   REQUIRED)
find_package(imgui  REQUIRED)

if (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
endif()

set(GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

INCLUDE_DIRECTORIES(
    /usr/local/include/
    /usr/local/include/imgui)

link_directories(/usr/local/lib/)

add_executable(FrontEnd 
                main.cpp
                ~/Projects/imgui/imgui/imgui.cpp
                ~/Projects/imgui/imgui/imgui_draw.cpp
                ~/Projects/imgui/imgui/imgui_widgets.cpp
                ~/Projects/imgui/imgui/examples/imgui_impl_glfw.cpp
                ~/Projects/imgui/imgui/examples/imgui_impl_opengl2.cpp)

target_link_libraries(FrontEnd
    ${OPENGL_LIBRARY}
    glfw
    ${GLEW_LIBRARIES}
    /usr/local/lib/libimgui.a)

target_compile_definitions(FrontEnd 
    PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW
)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
