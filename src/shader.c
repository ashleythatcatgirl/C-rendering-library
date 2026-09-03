
#include "glad/glad.h"

#include <stdlib.h>
#include <stdio.h>

#include "shader.h"
#include "helper.h"

unsigned int createShaderProgram(const char *vertexShaderPath, const char *geometryShaderPath, const char *fragmentShaderPath) {
	unsigned int shaderProgram = glCreateProgram();

	if (vertexShaderPath) loadShader(shaderProgram, vertexShaderPath, GL_VERTEX_SHADER);
	if (geometryShaderPath) loadShader(shaderProgram, geometryShaderPath, GL_GEOMETRY_SHADER);
	if (fragmentShaderPath) loadShader(shaderProgram, fragmentShaderPath, GL_FRAGMENT_SHADER);

	return shaderProgram;
}

void loadShader(const unsigned int shaderProgram, const char *shaderPath, const int shaderType) {
	unsigned int shader;
	int success;

	const char *shaderContent = getShaderContent(shaderPath);
	if (!shaderContent) return;

	shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderContent, NULL);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		char infoLog[1024];
		glGetShaderInfoLog(shader, 1024, NULL, infoLog);
		printf("Shader compilation error\nError log\n%s\nEnd\n", infoLog);

		goto freeShaderMemory;
	}

	glAttachShader(shaderProgram, shader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    	if (!success) {
		char infoLog[1024];
        	glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
        	printf("Shader linking error\nError log\n%s\nEnd\n", infoLog);

		goto freeShaderMemory;
    	}

freeShaderMemory:

	freeShader(shaderContent, shader);
}

char* getShaderContent(const char *shaderFileName) {
	char buffer = 0;
	char* shaderContent = 0;
	int size = 1024;

	shaderContent = (char*)malloc(sizeof(char) * size);
	if (shaderContent == NULL) return 0;

	FILE *fPtr = fopen(shaderFileName, "r");
	if (fPtr == NULL) {
		free(shaderContent);
		return 0;
	}

	int i = 0;
	for (; (buffer = fgetc(fPtr)) != EOF; i++) {
		if (i >= size) {
			size *= 2;
			shaderContent = (char*)resizeArray(shaderContent, sizeof(char) * size);
		}
		
		shaderContent[i] = buffer;
	}

	shaderContent[i] = '\0';

	fclose(fPtr);

	return shaderContent;
}

void freeShader(const char *shaderSource, unsigned int shader) {
	free((char*)shaderSource);
	glDeleteShader(shader);
}

void shaderSetMat4(unsigned int shaderProgram, const char *name, int gl_bool, float *data) {	
	int location = glGetUniformLocation(shaderProgram, name);
	glUniformMatrix4fv(location, 1, gl_bool, data);
}
