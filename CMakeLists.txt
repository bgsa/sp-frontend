cmake_minimum_required(VERSION 3.0.0)
project(FrontEnd VERSION 0.1.0)

find_package(OpenGL REQUIRED)
find_package(GLFW3  REQUIRED)

message("CMAKE_CURRENT_LIST_DIR is ${CMAKE_CURRENT_LIST_DIR}")  


INCLUDE_DIRECTORIES(
	/usr/local/include/
	../vendor/imgui/
)

link_directories(
	/usr/local/lib/
	../vendor/imgui/lib/
)

add_executable(FrontEnd 
                main.cpp
)

target_link_libraries(FrontEnd
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
    ${OPENGL_LIBRARY}
    glfw3
    ${GLEW_LIBRARIES}
    ${CMAKE_CURRENT_LIST_DIR}/../vendor/imgui/lib/libimgui.a)

target_compile_definitions(FrontEnd 
    PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
