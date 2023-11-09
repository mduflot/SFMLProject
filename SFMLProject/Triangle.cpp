#include "Triangle.h"
#include <iostream>

// define shader
#define GLSL(shader) "#version 330 core\n" #shader

Triangle::Triangle() : Scene() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat vertices[] = {
		-0.5, 0.5, 0,     1.0, 0, 1.0, 0.5,
		0.5, 0.5, 0,      0, 1.0, 1.0, 0.2, 
		0.5, -0.5, 0,     1.0, 1.0, 0, 0.7
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Standard Shader without define
	/*
	const GLchar* vertexSource =
		"#version 330 core\n"
		"in vec2 position;"
		"void main()"
		"{"
		"	gl_Position = vec4(position, 0, 1.0);"
		"}";
	*/

	// With define GLSL Shader
	const GLchar* vertexSource = GLSL(
		layout(location = 0) in vec3 position;
		layout(location = 1) in vec4 color;

		out vec4 vColor;
		void main() { 
			vColor = color;
			gl_Position = vec4(position, 1.0); 
		}
	);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint status; 
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);

	char buffer[512];
	glGetShaderInfoLog(vertexShader, 512, NULL, buffer);
	std::cout << buffer << std::endl;

	const GLchar* fragmentSource = GLSL(
		in vec4 vColor;
		out vec4 outColor;
		void main() {
			outColor = vec4(vColor);
		}
	);

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	/*
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 2, 0);
	*/

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 7, (void*)(3 * sizeof(GL_FLOAT)));
}

Triangle::~Triangle() {
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Triangle::display(sf::Window& window) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDisable(GL_BLEND);
}

void Triangle::update(float elapsed, const Camera& camera, sf::Window& window) {

}
