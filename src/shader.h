
#ifndef SHADER_H 
#define SHADER_H

#include "stdlib.h"
#include "stdio.h"

uint createShaderProgram(const char *vertexShaderPath, const char *geometryShaderPath, const char *fragmentShaderPath);
void loadShader(const uint shaderProgram, const char *shaderPath, const int shaderType);
char *getShaderContent(const char *shaderFileName);
char *readFileToArray(FILE *fPtr);
void freeShader(const char *shaderSource, uint shader);

void shaderSetMat4(uint shaderProgram, const char *name, int gl_bool, float *data);

#endif

