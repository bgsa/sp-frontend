cmake_minimum_required(VERSION 3.0.0)
project(FrontEnd VERSION 0.1.0)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/bin)
set (RUNTIME_OUTPUT_NAME SpectrumEngine)
set (SP_FOUNDATION_DIR "${CMAKE_CURRENT_LIST_DIR}/../sp-foundation")
set (VENDOR_DIR "${CMAKE_CURRENT_LIST_DIR}/../vendor")

set (CMAKE_C_STANDARD 11)
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(PkgConfig REQUIRED)
#pkg_search_module(GLFW REQUIRED glfw3)

find_package(OpenGL REQUIRED)
#find_package(GLFW REQUIRED)

file(GLOB HEADERS "src/*.h")
file(GLOB SOURCES "src/*.cpp")
file(GLOB DEFINES
	-DENV_32BITS 
	-DGLFW 
	-Wno-dangling-else
)
file(GLOB LIBRARIES
	${OPENGL_LIBRARY}
	${GLEW_LIBRARIES}
	/usr/local/lib/libglfw3.a
	${VENDOR_DIR}/imgui/lib/libimgui.a
)

INCLUDE_DIRECTORIES(
	/usr/local/include/
	${VENDOR_DIR}/imgui/
	${SP_FOUNDATION_DIR}/src/
	${HEADERS}
)

link_directories(
	/usr/local/lib/
	/usr/lib/x86_64-linux-gnu/
	${VENDOR_DIR}/imgui/lib/
)

add_executable(FrontEnd 
    ${SOURCES}
)

if ( ${UNIX} )
	list(APPEND LIBRARIES
	    "-framework Cocoa"
	    "-framework IOKit"
	    "-framework CoreVideo"
	)
	list(APPEND DEFINES -DUNIX)

elseif ( ${LINUX} )
	list(APPEND DEFINES -DLINUX)

elseif ( ${WINDOWS} )
	list(APPEND DEFINES -DWINDOWS)

else()
	message(SEND_ERROR "Operation System not defined.")
endif()


#add_definitions( ${DEFINES} )
add_definitions ( -DUNIX -DENV_32BITS -DGLFW -Wno-dangling-else)

target_link_libraries(FrontEnd ${LIBRARIES})

target_compile_definitions(FrontEnd 
    PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
