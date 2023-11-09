#include "Skybox.h"
#include <iostream>

#define GLSL(shader) "#version 330 core\n" #shader

#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A

Skybox::Skybox() : Scene() {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLfloat vertices[] = {
		-1.0f,  1.0f,  -1.0f,
		 1.0f,  1.0f,  -1.0f,
		 1.0f, -1.0f,  -1.0f,
		-1.0f, -1.0f,  -1.0f,

		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f
	};

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0,

		5, 4, 7,
		7, 6, 5,

		1, 0, 4,
		4, 5, 1,

		1, 5, 6,
		6, 2, 1,

		4, 0, 3,
		3, 7, 4,

		3, 2, 6,
		6, 7, 3
	};

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	const GLchar* vertexSource = GLSL(
		layout(location = 0) in vec3 position;

		out vec3 TexCoord;

		uniform mat4 model;
		uniform mat4 proj;
		uniform mat4 view;

		void main() {
			gl_Position = proj * view * model * vec4(position, 1.0);
			TexCoord = vec3(position.x, position.y, -position.z);
		}
	);

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	const GLchar* fragmentSource = GLSL(
		out vec4 outColor;

		in vec3 TexCoord;

		uniform samplerCube skybox;

		void main() {
			outColor = texture(skybox, TexCoord);
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 3, 0);

	const char* facesCubemap[6] = {
		"Data/sky_0.bmp",
		"Data/sky_1.bmp",
		"Data/sky_2.bmp",
		"Data/sky_3.bmp",
		"Data/sky_4.bmp",
		"Data/sky_5.bmp"
	};

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (unsigned int i = 0; i < 6; i++)
	{
		unsigned char* data = BMP::loadBMP_raw(facesCubemap[i], widthMap, heightMap);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, widthMap, heightMap, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
		delete[] data;
	}
	
	GLint uniTexture = glGetUniformLocation(shaderProgram, "skybox");
	glUniform1i(uniTexture, 0);

	model = glm::mat4(1.0f);
	proj = glm::mat4(1.0f);
	view = glm::mat4(1.0f);

	uniModel = glGetUniformLocation(shaderProgram, "model");
	uniProj = glGetUniformLocation(shaderProgram, "proj");
	uniView = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}

Skybox::~Skybox() {
	glDeleteProgram(shaderProgram);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteBuffers(1, &vbo);
	glDeleteBuffers(1, &ebo);
	glDeleteVertexArrays(1, &vao);
}

void Skybox::display(sf::Window& window) {
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glBindVertexArray(vao);
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Skybox::update(float elapsed, const Camera& camera, sf::Window& window) {
	model = glm::rotate(model, glm::radians(elapsed), glm::vec3(0.0f, 1.0f, 0.0f));
	proj = glm::perspective(glm::radians(camera.Zoom), (float)window.getSize().x / (float)window.getSize().y, 0.01f, 100.0f);
	view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
}