
#ifndef SHADER_H 
#define SHADER_H

unsigned int createShaderProgram(const char *vertexShaderPath, const char *geometryShaderPath, const char *fragmentShaderPath);
void loadShader(const unsigned int shaderProgram, const char *shaderPath, const int shaderType);
char* getShaderContent(const char *shaderFileName);
void freeShader(const char *shaderSource, unsigned int shader);

void shaderSetMat4(unsigned int shaderProgram, const char *name, int gl_bool, float *data);

#endif

