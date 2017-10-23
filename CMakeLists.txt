CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(base)

OPTION(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
OPTION(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
OPTION(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
OPTION(GLFW_INSTALL "Generate installation target" OFF)

ADD_SUBDIRECTORY(deps/glfw)

IF(UNIX)
  SET(GCC_CXX_FLAGS "-std=gnu++11 -m64")

  #X11 Dependencies
  SET(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXi")
  SET(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXxf86vm")
  SET(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXinerama")
  SET(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXcursor")
  SET(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXfixes")
  SET(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXrandr")
  SET(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXext")
  SET(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXrender")
  SET(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lX11")
  SET(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lpthread")
  SET(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lxcb")
  SET(GCC_CXX_FLAGS "${GCC_CXX_FLAGS} -lXau")

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_CXX_FLAGS}")
ENDIF()

#Required packages
FIND_PACKAGE(OpenGL REQUIRED)

INCLUDE_DIRECTORIES(
  deps/
  deps/glm
  include/
  ${GLFW_INCLUDE_DIRS}
 )

link_directories(${GLFW_LIBRARY_DIRS})

ADD_EXECUTABLE(basic_cube src/basic_cube/cube.cpp)
ADD_EXECUTABLE(textured_cube src/textured_cube/textured_cube.cpp)
ADD_EXECUTABLE(obj_loading src/obj_loading/obj_loading.cpp)
ADD_EXECUTABLE(camera_controls src/camera_controls/camera_controls.cpp)

TARGET_LINK_LIBRARIES(basic_cube glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(textured_cube glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(obj_loading glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
TARGET_LINK_LIBRARIES(camera_controls glfw ${GLFW_LIBRARIES} ${OPENGL_LIBRARIES})
