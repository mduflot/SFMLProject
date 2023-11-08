#include <iostream>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include "Scene.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Camera.h"

int main() {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8; 
	settings.antialiasingLevel = 2;
	sf::Window window(sf::VideoMode(1024, 768, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);
	sf::Clock clock;
	glewExperimental = GL_TRUE;
	glewInit();

	// Scene* scene = new Triangle;
	Scene* scene = new Rectangle;
	Camera camera(
		glm::vec3(0.0f, 0.0f, 1.0f), // position
		glm::vec3(0.0f, 1.0f, 0.0f) // up
	);
	sf::Vector2f oldPos = sf::Vector2f(window.getSize().x / 2, window.getSize().y / 2);

	while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseMoved) {
				sf::Vector2f newPos = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
				sf::Vector2f deltaPos = oldPos - newPos;
				camera.ProcessMouseMovement(deltaPos.x, deltaPos.y);
				oldPos = newPos;
			}
			if (event.type == sf::Event::MouseWheelScrolled) {
				camera.ProcessMouseScroll(event.mouseWheelScroll.delta);
			}
			if (event.type == sf::Event::KeyPressed) {
				
			}
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
			camera.ProcessKeyboard(LEFT, deltaTime);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
			camera.ProcessKeyboard(RIGHT, deltaTime);
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}

		scene->update(deltaTime, camera);
		scene->display(window);
		window.display();
	}

	return 0;
}