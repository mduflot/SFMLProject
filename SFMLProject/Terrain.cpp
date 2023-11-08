#include "Terrain.h"
#include <iostream>

#define GLSL(shader) "#version 330 core\n" #shader

Terrain::Terrain() : Scene() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat* vertices;
	GLuint* elements;

	unsigned char* data = BMP::loadBMP_raw("heights.bmp", widthMap, heightMap);

	vertices =  new GLfloat[widthMap * heightMap * 5];
	elements = new GLuint[(widthMap -1) * (heightMap - 1) * 6];

	unsigned int indexVertices = 0;

	for (unsigned int y = 0; y < heightMap; y++) {
		for (unsigned int x = 0; x < widthMap; x++) {
			vertices[indexVertices] = (float) x;
			vertices[indexVertices + 1] = (float) y;
			vertices[indexVertices + 2] = 0;
			
			vertices[indexVertices + 3] = ((float)x / (float)(widthMap - 1));
			vertices[indexVertices + 4] = ((float)y / (float)(heightMap - 1));
			indexVertices += 5;
		}
	}

	unsigned int indexElements = 0;

	for (unsigned int y = 0; y < heightMap - 1; y++) {
		for (unsigned int x = 0; x < widthMap - 1; x++) {
			elements[indexElements] = (y * widthMap) + x;
			elements[indexElements + 1] = (y * widthMap) + x + 1;
			elements[indexElements + 2] = ((y + 1) * widthMap) + x + 1;

			elements[indexElements + 3] = (y * widthMap) + x;
			elements[indexElements + 4] = ((y + 1) * widthMap) + x + 1;
			elements[indexElements + 5] = ((y + 1) * widthMap) + x;
			indexElements += 6;
		}
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * widthMap * heightMap * 5, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * (widthMap - 1) * (heightMap - 1) * 6, elements, GL_STATIC_DRAW);

	const GLchar* vertexSource = GLSL(
		layout(location = 0) in vec3 position;
		layout(location = 1) in vec2 aTexCoord;

		out vec2 TexCoord;

		uniform mat4 model;
		uniform mat4 proj;
		uniform mat4 view;

		void main() {
			TexCoord = aTexCoord;
			gl_Position = proj * view * model * vec4(position, 1.0);
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

	texture = BMP::loadBMP_texture("map.bmp", widthMap, heightMap);
	GLint uniTexture = glGetUniformLocation(shaderProgram, "myTexture");
	glUniform1i(uniTexture, 0);

	model = glm::mat4(1.0f);
	proj = glm::perspective(glm::radians(45.0f), 1024.0f / 768.0f, 0.01f, 100.0f);
	view = glm::lookAt(
		glm::vec3(0.0f, 0.0f, 2.0f), // position
		glm::vec3(0.0f, 0.0f, 0.0f), // target
		glm::vec3(0.0f, 1.0f, 0.0f)  // up
	);

	uniModel = glGetUniformLocation(shaderProgram, "model");
	uniProj = glGetUniformLocation(shaderProgram, "proj");
	uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}

Terrain::~Terrain() {
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void Terrain::display(sf::Window& window) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glDrawElements(GL_TRIANGLES, (widthMap - 1) * (heightMap - 1) * 6, GL_UNSIGNED_INT, 0);
	glDisable(GL_BLEND);
}

void Terrain::update(float elapsed, const Camera& camera) {
	proj = glm::perspective(glm::radians(camera.Zoom), 1024.0f / 768.0f, 0.01f, 100.0f);
	view = camera.GetViewMatrix();
}