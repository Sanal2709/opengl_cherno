cmake_minimum_required(VERSION 3.0.0)
project(opengl VERSION 0.1.0)

find_package(OpenGL REQUIRED)

set(IMGUI_SRC ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_demo.cpp
              ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_draw.cpp
              ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_tables.cpp
              ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_widgets.cpp
              ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui.cpp
              ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_impl_glfw.cpp
              ${CMAKE_SOURCE_DIR}/vendor/imgui/imgui_impl_opengl3.cpp)

set(STBI_SRC ${CMAKE_SOURCE_DIR}/vendor/stb_image/stb_image.cpp)

set(APP_SRC main.cpp
            renderer.cpp
            indexBuffer.cpp
            vertexBuffer.cpp
            vertexArray.cpp
            shader.cpp
            texture.cpp)

add_executable(${PROJECT_NAME} ${APP_SRC} ${STBI_SRC} ${IMGUI_SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/vendor)
target_include_directories(${PROJECT_NAME} PUBLIC ".")
target_link_libraries(${PROJECT_NAME} PRIVATE glfw SOIL GLEW dl ${OPENGL_LIBRARIES})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
