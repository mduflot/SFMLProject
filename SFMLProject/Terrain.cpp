#include "Terrain.h"
#include <iostream>

#define GLSL(shader) "#version 330 core\n" #shader

Terrain::Terrain() : Scene() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat* vertices;
	GLuint* elements;

	unsigned char* data = BMP::loadBMP_raw("Data/heights.bmp", widthTexture, heightTexture);

	vertices =  new GLfloat[widthTexture * heightTexture * 5];
	heightMap = new GLfloat[widthTexture * heightTexture * 3];
	elements = new GLuint[(widthTexture -1) * (heightTexture - 1) * 6];

	unsigned int indexVertices = 0;
	unsigned int indexVerticesHeight = 0;

	for (unsigned int y = 0; y < heightTexture; y++) {
		for (unsigned int x = 0; x < widthTexture; x++) {
			vertices[indexVertices] = (float) x;
			heightMap[indexVerticesHeight] = (float)x;
			vertices[indexVertices + 1] = (float)data[((y * widthTexture) + x) * 3] / 255.0f * 100.0f;
			heightMap[indexVerticesHeight + 1] = (float)data[((y * widthTexture) + x) * 3] / 255.0f * 100.0f;
			vertices[indexVertices + 2] = (float)y;
			heightMap[indexVerticesHeight + 2] = (float)y;

			vertices[indexVertices + 3] = ((float)x / (float)(widthTexture - 1));
			vertices[indexVertices + 4] = ((float)y / (float)(heightTexture - 1));
			indexVertices += 5;
			indexVerticesHeight += 3;
		}
	}

	delete[] data;

	unsigned int indexElements = 0;

	for (unsigned int y = 0; y < heightTexture - 1; y++) {
		for (unsigned int x = 0; x < widthTexture - 1; x++) {
			elements[indexElements] = (y * widthTexture) + x;
			elements[indexElements + 1] = (y * widthTexture) + x + 1;
			elements[indexElements + 2] = ((y + 1) * widthTexture) + x + 1;

			elements[indexElements + 3] = (y * widthTexture) + x;
			elements[indexElements + 4] = ((y + 1) * widthTexture) + x + 1;
			elements[indexElements + 5] = ((y + 1) * widthTexture) + x;
			indexElements += 6;
		}
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * widthTexture * heightTexture * 5, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * (widthTexture - 1) * (heightTexture - 1) * 6, elements, GL_STATIC_DRAW);

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

	model = glm::mat4(1.0f);

	glActiveTexture(GL_TEXTURE0);
	texture = BMP::loadBMP_texture("Data/map.bmp", widthTexture, heightTexture);
	GLint uniTexture = glGetUniformLocation(shaderProgram, "myTexture");
	glUniform1i(uniTexture, 0);

	uniModel = glGetUniformLocation(shaderProgram, "model");
	uniProj = glGetUniformLocation(shaderProgram, "proj");
	uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

	delete[] vertices;
	delete[] elements;

	skybox = new Skybox;
	minimap = new Minimap;
}

Terrain::~Terrain() {
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
	delete skybox;
	delete[] heightMap;
	delete minimap;
}

void Terrain::display(sf::Window& window) {
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
	skybox->display(window);
	glEnable(GL_DEPTH_TEST);
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(vao);
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glDrawElements(GL_TRIANGLES, (widthTexture - 1) * (heightTexture - 1) * 6, GL_UNSIGNED_INT, 0);
	minimap->display(window);
}

void Terrain::update(float elapsed, const Camera& camera, sf::Window& window) {
	proj = glm::perspective(glm::radians(camera.Zoom), (float)window.getSize().x / (float)window.getSize().y , 0.01f, 100.0f);
	view = camera.GetViewMatrix();
	skybox->update(elapsed, camera, window);
}