#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "shader.h"
#include "texture.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>


int main(void)
{
  GLFWwindow *window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;

  //Set minimal version required OpenGL.
  //major
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //minor
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //Set profile for context
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);
  glViewport(0, 0, 640, 480);

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK)
  {
    std::cout << "GLEW has not been initiated!" << std::endl;
    return -1;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;

  float positions[] = {
    -0.5f, -0.5f, 0.0f, 0.0f, //0
     0.5f, -0.5f, 1.0f, 0.0f, //1
     0.5f,  0.5f, 1.0f, 1.0f, //2
    -0.5f,  0.5f, 0.0f, 1.0f//3
  };

  unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3
  };

  VertexArray va;
  VertexBuffer vb(positions, 4 * 4 * sizeof(float));

  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indices, 6);

  Shader shader("res/shaders/basic.shader");
  shader.Bind();
  shader.SetUniform4f("u_Color", 1.0f, 0.4f, 0.5f, 1.0f);

  Texture texture("res/texture/logo.png");
  texture.Bind();
  shader.SetUniform1i("u_Texture", 0);

  va.UnBind();
  vb.Unbind();
  ib.Unbind();
  shader.UnBind();

  Renderer renderer;

  float r = 0.0f;
  float increment = 0.05f;
  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    renderer.Clear();

    shader.Bind();
    shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

    renderer.Draw(va, ib, shader);

    if(r > 1.0f) {
      increment = -0.05f;
    } else if(r < 0.0f){
      increment = 0.05f;
    }
    r += increment;

    /* Swap front and back buffers */
    GLCall(glfwSwapBuffers(window));

    /* Poll for and process events */
    GLCall(glfwPollEvents());
  }

  glfwTerminate();
  return 0;
}