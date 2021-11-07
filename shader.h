#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

struct ShaderProgramSource{
  std::string VertexSource;
  std::string FragmentSource;
};

class Shader{
private:
  std::string m_FilePath;
  unsigned int m_RendererID;
  std::unordered_map<std::string, int> m_UniformLocationCache;
public:
  Shader(const std::string& filepath);
  ~Shader();

  void Bind() const;
  void UnBind() const;

  //set uniforms
  void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

private:
  ShaderProgramSource ParseShader(const std::string& filepath);  
  unsigned int CompileShader(unsigned int type, const std::string& source);
  int CreateProgram(const std::string& vertexShader, const std::string& fragmentShader);
  int GetUniformLocation(const std::string& name);
};

#endif /*SHADER_H*/