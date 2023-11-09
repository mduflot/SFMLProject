#include "Minimap.h"
#include <iostream>

#define GLSL(shader) "#version 330 core\n" #shader

Minimap::Minimap() : Scene() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat vertices[] = {
	    0.5f,  0.9f,  0.0f,  0.0f, 1.0f,
		0.9f,  0.9f,  0.0f,  1.0f, 1.0f,
		0.9f,  0.5f,  0.0f,  1.0f, 0.0f,
	    0.5f,  0.5f,  0.0f,  0.0f, 0.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0,
	};

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	const GLchar* vertexSource = GLSL(
		layout(location = 0) in vec3 position;
		layout(location = 1) in vec2 aTexCoord;

		out vec2 TexCoord;

		void main() {
			TexCoord = aTexCoord;
			gl_Position = vec4(position, 1.0);
		}
	);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	const GLchar* fragmentSource = GLSL(
		out vec4 outColor;

		in vec2 TexCoord;

		uniform sampler2D myTexture;

		void main() {
			outColor = texture(myTexture, TexCoord);
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

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(3 * sizeof(GL_FLOAT)));

	glActiveTexture(GL_TEXTURE0);

	unsigned int width;
	unsigned int height;

	texture = BMP::loadBMP_texture("Data/map.bmp", width, height);
	GLint uniTexture = glGetUniformLocation(shaderProgram, "myTexture");
	glUniform1i(uniTexture, 0);
}

Minimap::~Minimap() {
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void Minimap::display(sf::Window& window) {
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Minimap::update(float elapsed, const Camera& camera, sf::Window& window) {

}