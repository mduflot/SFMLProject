#pragma once
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include "Camera.h"

class Scene {
	public :
		Scene();
		~Scene();
		virtual void display(sf::Window& window) = 0;
		virtual void update(float elapsed, const Camera& camera) = 0;
	protected :
		GLuint vao;
		GLuint vbo;
		GLuint vertexShader;
		GLuint fragmentShader;
		GLuint shaderProgram;
};