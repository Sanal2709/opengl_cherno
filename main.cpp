#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "renderer.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "indexBuffer.h"
#include "vertexArray.h"
#include "shader.h"
#include "texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>


int main(void)
{
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
  GLFWwindow * window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

  VertexArray va;
  VertexBuffer vb(positions, 4 * 4 * sizeof(float));

  VertexBufferLayout layout;
  layout.Push<float>(2);
  layout.Push<float>(2);
  va.AddBuffer(vb, layout);

  IndexBuffer ib(indices, 6);

  glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

  Shader shader("res/shaders/basic.shader");
  shader.Bind();
  //shader.SetUniform4f("u_Color", 1.0f, 0.4f, 0.5f, 1.0f);

  Texture texture("res/texture/logo.png");
  texture.Bind();
  shader.SetUniform1i("u_Texture", 0);
  shader.SetUniformMat4f("u_MVP", proj);

  va.UnBind();
  vb.Unbind();
  ib.Unbind();
  shader.UnBind();

  Renderer renderer;

  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  const char* glsl_version = "#version 330";
  ImGui_ImplOpenGL3_Init(glsl_version);

  float r = 0.0f;
  float increment = 0.05f;

  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    renderer.Clear();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    shader.Bind();
    //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

    renderer.Draw(va, ib, shader);

    if(r > 1.0f) {
      increment = -0.05f;
    } else if(r < 0.0f){
      increment = 0.05f;
    }
    r += increment;

    {
      static float f = 0.0f;
      static int counter = 0;

      ImGui::Begin("Hello, world!"); // Create a window called "Hello, world!" and append into it.

      ImGui::Text("This is some useful text.");          // Display some text (you can use a format strings too)
      ImGui::Checkbox("Demo Window", &show_demo_window); // Edit bools storing our window open/close state
      ImGui::Checkbox("Another Window", &show_another_window);

      ImGui::SliderFloat("float", &f, 0.0f, 1.0f);             // Edit 1 float using a slider from 0.0f to 1.0f
      ImGui::ColorEdit3("clear color", (float *)&clear_color); // Edit 3 floats representing a color

      if (ImGui::Button("Button")) // Buttons return true when clicked (most widgets return true when edited/activated)
        counter++;
      ImGui::SameLine();
      ImGui::Text("counter = %d", counter);

      ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
      ImGui::End();
    }

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    /* Swap front and back buffers */
    GLCall(glfwSwapBuffers(window));

    /* Poll for and process events */
    GLCall(glfwPollEvents());
  }

  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();
  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}