
#include "glad/glad.h"

#include <stdlib.h>
#include <stdio.h>

#include "shader.h"
#include "helper.h"

uint createShaderProgram(const char *vertexShaderPath, const char *geometryShaderPath, const char *fragmentShaderPath) {
	uint shaderProgram = glCreateProgram();

	if (vertexShaderPath) loadShader(shaderProgram, vertexShaderPath, GL_VERTEX_SHADER);
	if (geometryShaderPath) loadShader(shaderProgram, geometryShaderPath, GL_GEOMETRY_SHADER);
	if (fragmentShaderPath) loadShader(shaderProgram, fragmentShaderPath, GL_FRAGMENT_SHADER);

	return shaderProgram;
}

void loadShader(const uint shaderProgram, const char *shaderPath, const int shaderType) {
	uint shader;
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

char *getShaderContent(const char *shaderFileName) {
	FILE *fPtr = fopen(shaderFileName, "r");
	if (!fPtr)
		return NULL;

	char *shaderContent = readFileToArray(fPtr);

	fclose(fPtr);

	return shaderContent;
}

char *readFileToArray(FILE *fPtr) {
	int size = 1024;
	char *array = malloc(sizeof(char) * size);
	if (!array)
		return NULL;

	char buffer = 0;
	int i = 0;
	for (; (buffer = fgetc(fPtr)) != EOF; i++) {
		if (i >= size) {
			size *= 2;
			array = (char*)resizeArray(array, sizeof(char) * size);
		}
		
		array[i] = buffer;
	}

	array[i] = '\0';

	return array;
}


void freeShader(const char *shaderSource, uint shader) {
	free((char*)shaderSource);
	glDeleteShader(shader);
}

void shaderSetMat4(uint shaderProgram, const char *name, int gl_bool, float *data) {	
	int location = glGetUniformLocation(shaderProgram, name);
	glUniformMatrix4fv(location, 1, gl_bool, data);
}
