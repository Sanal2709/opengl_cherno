#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <string>

struct ShaderProgramSource{
  std::string VertexSource;
  std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath){
  std::ifstream stream{filepath};

  enum class ShaderType{
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;

  while (std::getline(stream, line)){
    if(line.find("#shader") != std::string::npos){
      if(line.find("vertex") != std::string::npos){
        type = ShaderType::VERTEX;
      }
      else if(line.find("fragment") != std::string::npos){
        type = ShaderType::FRAGMENT;
      }
    }
    else{
      if(type != ShaderType::NONE){
        ss[(int)type] << line << '\n';
      }
    }
  }

  return { ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string& source){
  unsigned int id = glCreateShader(type);
  const char * src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result = 0;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if(result == GL_FALSE){
    int length = 0;
    std::unique_ptr<char> message{new char[length]};

    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    glGetShaderInfoLog(id, length, &length, message.get());

    std::cout << "Failed to compile "<<
                  ((type == GL_VERTEX_SHADER)? "vertex" : "fragment") <<
                  " shader"<< std::endl;
    std::cout << message.get() << std::endl;
    
    glDeleteShader(id);
    return 0;
  }

  return id;
}

static int CreateProgram(const std::string& vertexShader, const std::string& fragmentShader){
  unsigned int program = glCreateProgram();
  unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}

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

  if (glewInit() != GLEW_OK)
  {
    std::cout << "GLEW has not been initiated!" << std::endl;
    return -1;
  }

  std::cout << glGetString(GL_VERSION) << std::endl;

  float positions[] = {
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f,
  };

  unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3
  };

  unsigned int buffer = 0;
  glGenVertexArrays(1, &buffer);
  glBindVertexArray(buffer);
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);  //wtf: why size == 6*2

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2*sizeof(float), 0);

  unsigned int ibo = 0;
  glGenBuffers(1, &ibo);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

  ShaderProgramSource source = ParseShader("res/shaders/basic.shader");
  std::cout << "VERTEX" << std::endl;
  std::cout << source.VertexSource << std::endl;
  std::cout << "FRAGMENT" << std::endl;
  std::cout << source.FragmentSource << std::endl;
  unsigned int program = CreateProgram(source.VertexSource, source.FragmentSource);
  glUseProgram(program);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glDeleteProgram(program);

  glfwTerminate();
  return 0;
}