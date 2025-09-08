#pragma once
#include "pch.h"

std::string readFile(const char* path);

GLuint CompileShader(const char *source, GLenum type);

GLuint CreateShaderProgram(const char* vertexSource, const char* fragmentSource);