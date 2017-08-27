cmake_minimum_required(VERSION 2.8)

project(base)

set(GCC_CXX_FLAGS "-std=gnu++11 -m64")

# X11 Dependencies
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXi")
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXxf86vm")
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXinerama")
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXcursor")
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXfixes")
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXrandr")
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXext")
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXrender")
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lX11")
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lpthread")
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lxcb")
set(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXau")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXX_FLAGS}")

# Required packages
find_package(OpenGL REQUIRED)
find_package(PkgConfig)
if(PKGCONFIG_FOUND)
    pkg_search_module(GLFW glfw3)
else(PKGCONFIG_FOUND)
    find_package(GLFW)
endif()

# Application source
set(APPLICATION_SOURCE src/cube_textured.cpp)

include_directories(
    include/glLoadGen 
    include/stb 
    include/tinyobjloader
    ${GLFW_INCLUDE_DIRS}
    )

link_directories(
    ${GLFW_LIBRARY_DIRS}
    )

add_executable(base ${APPLICATION_SOURCE})

target_link_libraries(
    base 
    ${GLFW_STATIC_LIBRARIES} 
    ${OPENGL_LIBRARIES} 
    )

# Put executable in build directory root
#set(EXECUTABLE_OUTPUT_PATH)

# Install to project root
#install(TARGETS base DESTINATION ${base_SOURCE_DIR})

# Install settings
#set(CMAKE_INSTALL_PREFIX "${base_SOURCE_DIR}/")

