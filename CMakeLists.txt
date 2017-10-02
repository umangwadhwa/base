cmake_minimum_required(VERSION 2.8)

project(base)

set(GCC_CXX_FLAGS "-std=gnu++11 -m64")

#X11 Dependencies
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

#Required packages
find_package(OpenGL REQUIRED)
find_package(PkgConfig)
if(PKGCONFIG_FOUND)
    pkg_search_module(GLFW glfw3)
    pkg_search_module(GLM glm)
else(PKGCONFIG_FOUND)
    find_package(GLFW)
endif()

#Application source
#set(APPLICATION_SOURCE src/textured_cube/textured_cube.cpp)

include_directories(
    include/glLoadGen 
    include/stb 
    include/tinyobjloader
    ${GLFW_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    )

link_directories(${GLFW_LIBRARY_DIRS})

#add_executable(base ${APPLICATION_SOURCE})
add_executable(basic_cube src/basic_cube/cube.cpp)
add_executable(textured_cube src/textured_cube/textured_cube.cpp)
add_executable(obj_loading src/obj_loading/obj_loading.cpp)
add_executable(camera_controls src/camera_controls/camera_controls.cpp)

target_link_libraries(basic_cube ${GLFW_STATIC_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries(textured_cube ${GLFW_STATIC_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries(obj_loading ${GLFW_STATIC_LIBRARIES} ${OPENGL_LIBRARIES})
target_link_libraries(camera_controls ${GLFW_STATIC_LIBRARIES} ${OPENGL_LIBRARIES})
