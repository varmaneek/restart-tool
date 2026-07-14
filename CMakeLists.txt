cmake_minimum_required(VERSION 3.15)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/glfw)

set(IMGUI_DIR external/imgui)
set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

set(GLAD_DIR external/glad)

add_executable(MyProject
    src/main.cpp
    ${IMGUI_SOURCES}
    ${GLAD_DIR}/src/glad.c
 "src/interface.cpp" "include/interface.h")

target_include_directories(MyProject PRIVATE
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    external/glfw/include
    ${GLAD_DIR}/include
    include
)

target_link_libraries(MyProject PRIVATE
    glfw
    opengl32
)