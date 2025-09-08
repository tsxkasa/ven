#include "shadercompile.h"
#include "pch.h"

std::string readFile(const char *path) {
  std::ifstream file(path);
  std::stringstream fileStream;

  if (file.is_open()) {
    fileStream << file.rdbuf();
    file.close();
  } else {
    std::cerr << "Failed to open file: " << path;
  }

  return fileStream.str();
}

GLuint CompileShader(const char *source, GLenum type) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

  if (!success) {
    char infoLog[512];
    glGetShaderInfoLog(shader, 512, nullptr, infoLog);
    std::cerr << "Shader compilation failed: " << infoLog;
  }

  return shader;
}

GLuint CreateShaderProgram(const char *vertexSource,
                           const char *fragmentSource) {
  GLuint vertex = CompileShader(vertexSource, GL_VERTEX_SHADER);
  GLuint fragment = CompileShader(fragmentSource, GL_FRAGMENT_SHADER);

  GLuint shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertex);
  glAttachShader(shaderProgram, fragment);
  glLinkProgram(shaderProgram);

  GLint success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    char infoLog[512];
    glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
    std::cerr << "Shader program creation failed: " << infoLog;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  return shaderProgram;
}