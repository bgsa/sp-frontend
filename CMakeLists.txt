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
option(ARCH_BITS        "Defined the compilation to 64 bits"   "32")

set (PROJECT_DIR  ${CMAKE_CURRENT_SOURCE_DIR})
set (SOLUTION_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set (ENV_DEFINITION "-DENV_32BITS")
set (ARCH_DIR "x86")

set (SP_FOUNDATION_DIR ${SOLUTION_DIR}/sp-foundation)
set (SP_PHYSICS_DIR ${SOLUTION_DIR}/sp-physics)
set (SP_RENDERING_DIR ${SOLUTION_DIR}/sp-rendering)
set (VENDOR_DIR ${SOLUTION_DIR}/vendor)

set (CMAKE_C_STANDARD 11)
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_CXX_STANDARD_REQUIRED ON)

if ( ${ARCH_BITS} STREQUAL "64" )
	set (ARCH_DIR "x86_64")
	set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -m64 -std=c11" )
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -std=c++11" )
	set (ENV_DEFINITION  "-DENV_64BITS")
	set (OS_LIB_DIR      /usr/lib/x86_64-linux-gnu)

	list(APPEND CUSTOM_DEFINITIONS "-DENV_64BITS")
else()
	set(ARCH_DIR  "x86")	
	set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -m32 -std=c11" )
	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -std=c++11" )
	set (ENV_DEFINITION  "-DENV_32BITS")
	set (OS_LIB_DIR      /usr/lib/i386-linux-gnu)

	list(APPEND CUSTOM_DEFINITIONS "-DENV_32BITS")
endif()

set (LIB_DIR ${VENDOR_DIR}/lib/${ARCH_DIR}/${CMAKE_BUILD_TYPE})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_DIR}/bin/${ARCH_DIR}/${CMAKE_BUILD_TYPE})

if ( ${OPERATING_SYSTEM} STREQUAL "WINDOWS" )
	set (POSIX_THREAD_LIBRARY "")
	set (GLFW_LIB_NAME "glfw3.lib")
	set (IMGUI_LIB_NAME "imgui.lib")
	set (SP_FOUNDATION_LIBRARY "SpFoundation.lib")
	set (SP_PHYSICS_LIBRARY "SpPhysics.lib")
	set (SP_RENDERING_LIBRARY "SpRendering.lib")

	list(APPEND CUSTOM_DEFINITIONS "-DWINDOWS")
	list(APPEND CUSTOM_DEFINITIONS "-D_CRT_SECURE_NO_WARNINGS")

elseif ( ${OPERATING_SYSTEM} STREQUAL "LINUX" )
	
	find_package(GLEW REQUIRED)

	set (POSIX_THREAD_LIBRARY "pthread")
	set (GLFW_LIB_NAME "libglfw3.a")
	set (IMGUI_LIB_NAME "libimgui.a")
	set (SP_FOUNDATION_LIBRARY "libSpFoundation.a")
	set (SP_PHYSICS_LIBRARY "libSpPhysics.a")
	set (SP_RENDERING_LIBRARY "libSpRendering.a")

	list(APPEND CUSTOM_DEFINITIONS "-DLINUX")
	list(APPEND CUSTOM_DEFINITIONS "-Wno-dangling-else")

elseif ( ${OPERATING_SYSTEM} STREQUAL "UNIX" )

	find_package(GLEW REQUIRED)

	set (POSIX_THREAD_LIBRARY "pthread")
	set (GLFW_LIB_NAME "libglfw3.a")
	set (IMGUI_LIB_NAME "libimgui.a")
	set (SP_FOUNDATION_LIBRARY "libSpFoundation.a")
	set (SP_PHYSICS_LIBRARY "libSpPhysics.a")
	set (SP_RENDERING_LIBRARY "libSpRendering.a")

	list(APPEND CUSTOM_DEFINITIONS "-DUNIX")
	list(APPEND CUSTOM_DEFINITIONS "-DOSX")
	list(APPEND CUSTOM_DEFINITIONS "-Wno-dangling-else")

else()
    message(FATAL_ERROR "Operating System not defined!")
endif()

find_package(OpenGL REQUIRED)

if ( ${CMAKE_BUILD_TYPE} STREQUAL "Debug" )
	list(APPEND CUSTOM_DEFINITIONS "-DDEBUG")
endif()

file(GLOB HEADERS 
	"src/*.h"   
	"src/addons/*.h"
	"src/addons/FileBrowser/*.h"
)
file(GLOB SOURCES 
	"src/*.cpp" 
	"src/addons/*.cpp"
	"src/addons/FileBrowser/*.cpp"
)

include_directories(
	${HEADERS}
	${SP_FOUNDATION_DIR}/src
	${SP_PHYSICS_DIR}/src
	${SP_RENDERING_DIR}/src
	${VENDOR_DIR}/include
	/usr/local/include
)

link_directories(
	${LIB_DIR}
	${OS_LIB_DIR}
)

add_executable(${PROJECT_NAME} ${SOURCES})

list(JOIN CUSTOM_DEFINITIONS " " CUSTOM_DEFINITIONS)
message(STATUS "FLAGS: " ${CUSTOM_DEFINITIONS})
add_definitions(-DAPI_EXPORT -DOPENGL_ENABLED -DGLFW_ENABLED -DOPENCL_ENABLED ${CUSTOM_DEFINITIONS})

target_link_libraries(${PROJECT_NAME}
	${POSIX_THREAD_LIBRARY}
	${LIB_DIR}/libGLEW.a
	${OS_LIB_DIR}/libGL.so.1
	${LIB_DIR}/${GLFW_LIB_NAME}
	${CMAKE_DL_LIBS}
	${LIB_DIR}/${IMGUI_LIB_NAME}
	${LIB_DIR}/libOpenCL.a
	${SP_FOUNDATION_DIR}/lib/${ARCH_DIR}/${CMAKE_BUILD_TYPE}/${SP_FOUNDATION_LIBRARY}
	${SP_PHYSICS_DIR}/lib/${ARCH_DIR}/${CMAKE_BUILD_TYPE}/${SP_PHYSICS_LIBRARY}
	${SP_RENDERING_DIR}/lib/${ARCH_DIR}/${CMAKE_BUILD_TYPE}/${SP_RENDERING_LIBRARY}
)
if ( ${OPERATING_SYSTEM} STREQUAL "WINDOWS" )
	target_link_libraries(${PROJECT_NAME} glew32s.lib)
elseif ( ${OPERATING_SYSTEM} STREQUAL "UNIX" )
	target_link_libraries(${PROJECT_NAME}
	    "-framework Cocoa"
	    "-framework IOKit"
	    "-framework CoreVideo"
	)
endif()

target_compile_definitions(${PROJECT_NAME} 
	PUBLIC GLEW_STATIC
    PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

# Copy OpenCL sources to destination
set(OPENCL_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/gpgpu/opencl/source/")
file(REMOVE_RECURSE ${OPENCL_OUTPUT_DIR})
file(MAKE_DIRECTORY ${OPENCL_OUTPUT_DIR})
file(GLOB OPENCL_SOURCES "${SP_PHYSICS_DIR}/resources/opencl/src/*.cl")
file(COPY ${OPENCL_SOURCES} DESTINATION ${OPENCL_OUTPUT_DIR})

# Copy Shaders sources to destination
set(SHADERS_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/shaders/opengl/")
file(REMOVE_RECURSE ${SHADERS_OUTPUT_DIR})
file(MAKE_DIRECTORY ${SHADERS_OUTPUT_DIR})
file(COPY "${PROJECT_DIR}/resources/shaders/opengl/" DESTINATION ${SHADERS_OUTPUT_DIR})

# Copy MeshModels to destination
set(MODELS_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/models/")
file(REMOVE_RECURSE ${MODELS_OUTPUT_DIR})
file(MAKE_DIRECTORY ${MODELS_OUTPUT_DIR})
file(COPY "${PROJECT_DIR}/resources/models/" DESTINATION ${MODELS_OUTPUT_DIR})

# Copy Fonts to destination
set(FONTS_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/fonts/")
file(REMOVE_RECURSE ${FONTS_OUTPUT_DIR})
file(MAKE_DIRECTORY ${FONTS_OUTPUT_DIR})
file(COPY "${PROJECT_DIR}/resources/fonts/" DESTINATION ${FONTS_OUTPUT_DIR})

# Copy Images to destination
set(IMAGES_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/images/")
file(REMOVE_RECURSE ${IMAGES_OUTPUT_DIR})
file(MAKE_DIRECTORY ${IMAGES_OUTPUT_DIR})
file(COPY "${PROJECT_DIR}/resources/images/" DESTINATION ${IMAGES_OUTPUT_DIR})