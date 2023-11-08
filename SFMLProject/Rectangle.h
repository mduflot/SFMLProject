#pragma once
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include "Scene.h"
#include "bmp.h"

class Rectangle : public Scene {
	public :
		Rectangle();
		~Rectangle();
		virtual void display(sf::Window& window) override;
		virtual void update(float elapsed, const Camera& camera) override;
	protected :
		GLuint ebo;
		GLuint texture;
		GLint uniModel;
		GLint uniProj;
		GLint uniView;
		glm::mat4 model;
		glm::mat4 proj;
		glm::mat4 view;
};