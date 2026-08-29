#version 450 core
in vec3 color;

out vec4 FragColor;

void main() {
	vec3 newColor = color + 0.5;
	FragColor = vec4(newColor, 1.0);
}
