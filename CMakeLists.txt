cmake_minimum_required(VERSION 3.0.0)

if (POLICY CMP0054)
    cmake_policy(SET CMP0054 NEW)
endif()

if (POLICY CMP0077)
    cmake_policy(SET CMP0077 NEW)
endif()

set (MAJOR_VERSION 0)
set (MINOR_VERSION 1)
set (PATCH_VERSION 0)
project(SpEngine VERSION ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION})

option(OPERATING_SYSTEM "Defined which operating system will be built for" "WINDOWS")
option(GLFW_ENABLED     "Defined if GLFW will be used" ON)
option(QT_ENABLED       "Defined if QT will be used"   OFF)
option(ARCH_BITS        "Defined the compilation to 64 bits"   "64")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "bin")
set (RUNTIME_OUTPUT_NAME SpectrumEngine)
set (SP_FOUNDATION_DIR "../sp-foundation")
set (VENDOR_DIR "../vendor")

set (CMAKE_C_STANDARD 11)
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if ( ${ARCH_BITS} STREQUAL "64" )
	set (ARCH "x86_64")
	set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m64 -std=c11" )
	set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -m64 -std=c++11" )
else()
	set(ARCH "x86")	
	set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -m32 -std=c11" )
	set (CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -m32 -std=c++11" )
endif()

if ( ${OPERATING_SYSTEM} STREQUAL "WINDOWS" )
	set (GLFW_LIB_NAME "glfw3.lib")
	set (IMGUI_LIB_NAME "imgui.lib")
elseif ( ${OPERATING_SYSTEM} STREQUAL "LINUX" OR ${OPERATING_SYSTEM} STREQUAL "UNIX" )
	set (GLFW_LIB_NAME "libglfw3.a")
	set (IMGUI_LIB_NAME "libimgui.a")
else()
    message(FATAL_ERROR "Operating System not defined!")
endif()

find_package(OpenGL REQUIRED)

file(GLOB HEADERS "src/*.h")
file(GLOB SOURCES "src/*.cpp")

include_directories(
	/usr/local/include
	${VENDOR_DIR}/glfw/include
	${VENDOR_DIR}
	${SP_FOUNDATION_DIR}/src
	${HEADERS}
)

link_directories(
	/usr/local/lib
	/usr/lib/i386-linux-gnu/
	${VENDOR_DIR}/lib
)

file(GLOB LIBRARIES
	${OPENGL_LIBRARY}
	${GLEW_LIBRARIES}
	${VENDOR_DIR}/lib/${GLFW_LIB_NAME}
	${VENDOR_DIR}/lib/${IMGUI_LIB_NAME}
)

add_executable(${PROJECT_NAME} ${SOURCES})

if ( ${OPERATING_SYSTEM} STREQUAL "UNIX" )
	list(APPEND LIBRARIES
	    "-framework Cocoa"
	    "-framework IOKit"
	    "-framework CoreVideo"
	)
endif()

if ( ${OPERATING_SYSTEM} STREQUAL "WINDOWS" )
    add_definitions(-DENV_32BITS -DAPI_EXPORT -DGLFW_ENABLED -DOPENGL -Wno-dangling-else -DWINDOWS -D_CRT_SECURE_NO_WARNINGS)
elseif ( ${OPERATING_SYSTEM} STREQUAL "LINUX" )
    add_definitions(-DENV_32BITS -DAPI_EXPORT -DGLFW_ENABLED -DOPENGL -Wno-dangling-else -DLINUX)
elseif ( ${OPERATING_SYSTEM} STREQUAL "UNIX" )
    add_definitions(-DENV_32BITS -DAPI_EXPORT -DGLFW_ENABLED -DOPENGL -Wno-dangling-else -DUNIX)
else()
    message(FATAL_ERROR "Operating System not defined!")
endif()

target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

target_compile_definitions(${PROJECT_NAME} 
    PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
