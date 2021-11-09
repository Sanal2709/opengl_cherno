cmake_minimum_required(VERSION 3.0.0)
project(opengl VERSION 0.1.0)

find_package(OpenGL REQUIRED)



add_executable(${PROJECT_NAME} main.cpp renderer.cpp indexBuffer.cpp vertexBuffer.cpp vertexArray.cpp shader.cpp texture.cpp ${CMAKE_SOURCE_DIR}/vendor/stb_image/stb_image.cpp)
#target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/vendor/stb_image)
target_include_directories(${PROJECT_NAME} PUBLIC ".")
target_link_libraries(${PROJECT_NAME} PRIVATE glfw SOIL GLEW ${OPENGL_LIBRARIES})
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)
