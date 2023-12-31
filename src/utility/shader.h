#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <stdio.h>
#include <stdlib.h>

// setup functions
void set_shaders(const char* vertexPath, const char* fragmentPath);
void use_shaders();

// utility uniform functions
void set_int(const char* name, int value);
void set_float(const char* name, float value);

extern unsigned int shaderID;

#endif
