#include "Rectangle.h"
#include <iostream>

#define GLSL(shader) "#version 330 core\n" #shader

Rectangle::Rectangle() : Scene() {
	rotation = 90.0f;
	scale = 0.5f;
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat vertices[] = {
		 /* FORWARD FACE */
		-0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 0
		 0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // 1
		 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 0.0f, // 2
		-0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // 3

		 /* RIGHT FACE */
		 0.5f,  0.5f,  0.0f,    0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 4
		 0.5f,  0.5f, -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f, // 5
		 0.5f, -0.5f, -1.0f,    0.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 6
		 0.5f, -0.5f,  0.0f,    1.0f, 1.0f, 0.0f, 1.0f,   0.0f, 0.0f, // 7

		 /* LEFT FACE */
		-0.5f,  0.5f,  0.0f,    1.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f, // 8
		-0.5f,  0.5f, -1.0f,    0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 9
		-0.5f, -0.5f, -1.0f,    0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // 10
		-0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 11

		/* BACKWARD FACE */
		-0.5f, -0.5f, -1.0f,    0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // 12
		-0.5f,  0.5f, -1.0f,    0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 13
		 0.5f, -0.5f, -1.0f,    0.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 14
		 0.5f,  0.5f, -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f, // 15

		/* UPWARD FACE */
		-0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f, // 16
		 0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 17
		 0.5f,  0.5f, -1.0f,    1.0f, 0.0f, 1.0f, 1.0f,   1.0f, 1.0f, // 18
		-0.5f,  0.5f, -1.0f,    0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 19

		/* DOWNWARD FACE */
		-0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f, // 20
		 0.5f, -0.5f, 0.0f,     1.0f, 1.0f, 0.0f, 1.0f,   1.0f, 1.0f, // 21
		 0.5f, -0.5f, -1.0f,    0.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // 22
		-0.5f, -0.5f, -1.0f,    0.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // 23
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0,

		4, 5, 6,
		6, 7, 4,

		8, 9, 10,
		10, 11, 8,

		12, 13, 14,
		14, 15, 13,

		16, 17, 18,
		18, 19, 16,

		20, 21, 22,
		22, 23, 20
	};

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

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
		layout(location = 2) in vec2 aTexCoord;

		out vec4 vColor;
		out vec2 TexCoord;

		uniform mat4 model;
		uniform mat4 proj;
		uniform mat4 view;

		void main() {
			vColor = color;
			TexCoord = aTexCoord;
			gl_Position = proj * view * model * vec4(position, 1.0);
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

	/*
	const GLchar* fragmentSource = GLSL(
		out vec4 outColor;

		in vec4 vColor;
		void main() {
			outColor = vec4(vColor);
		}
	);
	*/

	const GLchar* fragmentSource = GLSL(
		out vec4 outColor;

		in vec4 vColor;
		in vec2 TexCoord;

		uniform sampler2D myTexture;

		void main() {
			outColor = texture(myTexture, TexCoord) * vec4(vColor);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 9, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 9, (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 9, (void*)(7 * sizeof(GL_FLOAT)));

	glActiveTexture(GL_TEXTURE0);

	unsigned int width;
	unsigned int height;

	texture = BMP::loadBMP_texture("Kermit-the-Frog.bmp", width, height);
	GLint uniTexture = glGetUniformLocation(shaderProgram, "myTexture");
	glUniform1i(uniTexture, 0);

	uniModel = glGetUniformLocation(shaderProgram, "model");
	uniProj = glGetUniformLocation(shaderProgram, "proj");
	uniView = glGetUniformLocation(shaderProgram, "view");
	model = glm::mat4(1.0f);
	proj = glm::mat4(1.0f);
	view = glm::mat4(1.0f);
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}

Rectangle::~Rectangle() {
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}

void Rectangle::display(sf::Window& window) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// glDrawArrays(GL_TRIANGLES, 0, 6);
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);
}

void Rectangle::update(sf::Time elapsed) {
	model = glm::rotate(model, elapsed.asSeconds(), glm::vec3(0.0, 0.0, 1.0));
	model = glm::rotate(model, elapsed.asSeconds(), glm::vec3(0.0, 1.0, 0.0));
}
