cmake_minimum_required(VERSION 3.2)

#################################
#C / Cxx standard
#   Install clang: sudo apt-get install clang-6.0
#   SetVariables: https://stackoverflow.com/questions/7031126/switching-between-gcc-and-clang-llvm-using-cmake
#   Compile: https://stackoverflow.com/questions/9148488/how-do-i-compile-c-with-clang
#################################
#For Tangram: -Wno-register
#SET (CMAKE_CXX_FLAGS                "-Wno-reorder -Wno-documentation -Wno-conversion -pthread") 

#################################
#Project
#################################
project(opengl_kitti)
message(STATUS "*** PROJECT_NAME: ${PROJECT_NAME}")

#################################
#Cxx standard
#################################
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message(STATUS "C++14 support has been enabled by default.")

#################################
#Set modulepath for find_package
#################################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
message(STATUS "*** CMAKE_CURRENT_SOURCE_DIR: ${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "*** CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/stbimage)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/tinyxml2)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/contrib/imgui/include)


#################################
#Shader path
#################################
ADD_DEFINITIONS( -D_SHADER_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}\" )

#################################
#Executable
#################################
add_executable(opengl_kitti
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/shader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/simpleGeometryCollection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/videoPlayer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/tracklets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/car.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/pointCloud.cpp

    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/imgui/imgui_demo.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/imgui/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/imgui/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/imgui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contrib/tinyxml2/tinyxml2.cpp

    # ${CMAKE_CURRENT_SOURCE_DIR}/submodule/tangram-es/platforms/common/urlClient.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/submodule/tangram-es/platforms/common/linuxSystemFontHelper.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/submodule/tangram-es/platforms/linux/src/linuxPlatform.cpp
    # ${CMAKE_CURRENT_SOURCE_DIR}/submodule/tangram-es/platforms/common/platform_gl.cpp
)

#################################
#Find opengl
#################################
find_package(OpenGL REQUIRED)
if(OpenGL_FOUND)
    message(STATUS "*** Found OpenGl")
   	target_include_directories(${PROJECT_NAME} PUBLIC ${OPENGL_INCLUDE_DIR})
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARY})
endif(OpenGL_FOUND)

#################################
#   Submodule glew (static)
#################################
add_definitions(-DGLEW_STATIC)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodule/glew-cmake EXCLUDE_FROM_ALL)
target_link_libraries(${PROJECT_NAME} libglew_static)

#################################
#   Submodule glfw
#################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodule/glfw)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/submodule/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

#################################
#Add subdirectory glm
#################################
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodule/glm)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/submodule/glm)
target_link_libraries(${PROJECT_NAME} glm)

# #################################
# #Add Fontconfig
# #################################
# include(FindPkgConfig)
# pkg_check_modules(FONTCONFIG REQUIRED "fontconfig")
# target_include_directories(${PROJECT_NAME} PRIVATE ${FONTCONFIG_INCLUDE_DIRS})
# target_link_libraries(${PROJECT_NAME} ${FONTCONFIG_LDFLAGS})

# #################################
# #Add subdirectory curl
# #################################
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/submodule/curl)
# target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/submodule/curl)
# target_link_libraries(${PROJECT_NAME} libcurl)

# #################################
# #Add subdirectory tangram
# #Usage https://medium.com/postzen/delicious-3d-maps-baked-into-a-raspberrypi-cb80150853b4
# #https://github.com/tangrams/PI-GPS
# #https://medium.com/@patricklacz/behind-our-maps-mapzen-tangram-7f2c17fc0154
# #################################
# add_subdirectory(${PROJECT_SOURCE_DIR}/submodule/tangram-es/core)

# target_include_directories(${PROJECT_NAME} 
#                             PRIVATE 
#                             ${CMAKE_CURRENT_SOURCE_DIR}/submodule/tangram-es/core/include
#                             ${CMAKE_CURRENT_SOURCE_DIR}/submodule/tangram-es/core/include/tangram
#                             ${CMAKE_CURRENT_SOURCE_DIR}/submodule/tangram-es/core/deps
#                             ${CMAKE_CURRENT_SOURCE_DIR}/submodule/tangram-es/core/src
#                             ${CMAKE_CURRENT_SOURCE_DIR}/submodule/tangram-es/platforms/common
#                             ${CMAKE_CURRENT_SOURCE_DIR}/submodule/tangram-es/platforms/linux/src
#                             )

# target_link_libraries(${PROJECT_NAME} tangram-core)