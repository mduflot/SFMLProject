#include <iostream>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include "Scene.h"
#include "Triangle.h"
#include "Rectangle.h"
#include "Terrain.h"
#include "Camera.h"

int main() {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 2;
	sf::RenderWindow window(sf::VideoMode(1024, 768, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);
	sf::Clock clock;
	glewExperimental = GL_TRUE;
	glewInit();

	window.setMouseCursorGrabbed(true);
	window.setMouseCursorVisible(false);

	// Scene* triangle = new Triangle;
	// Scene* rectangle = new Rectangle;
	Terrain* terrain = new Terrain;
	// Scene* skybox = new Skybox;
	Camera camera(
		glm::vec3(terrain->widthTexture / 2, 20.0f, terrain->heightTexture / 2), // position
		glm::vec3(0.0f, 1.0f, 0.0f) // up
	);

	sf::Vector2i oldPos = sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2);

	while (window.isOpen()) {
		float deltaTime = clock.restart().asSeconds();

		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed) {
				window.close();
			}
			if (event.type == sf::Event::MouseWheelScrolled) {
				camera.ProcessMouseScroll(event.mouseWheelScroll.delta);
			}
		}

		sf::Vector2i newPos = sf::Vector2i(sf::Mouse::getPosition().x, sf::Mouse::getPosition().y);
		sf::Vector2i deltaPos = oldPos - newPos;
		camera.ProcessMouseMovement(deltaPos.x, deltaPos.y);
		oldPos = sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2);
		sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2));

		if (terrain->heightMap[(((int)camera.Position.z * terrain->widthTexture) + (int)camera.Position.x) * 3 + 1] > camera.Position.y) {
			camera.Position.y = terrain->heightMap[(((int)camera.Position.z * terrain->widthTexture) + (int)camera.Position.x) * 3 + 1] + 10.0f;
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

		// triangle->update(deltaTime, camera);
		// triangle->display(window);
		// rectangle->update(deltaTime, camera);
		// rectangle->display(window);
		terrain->update(deltaTime, camera, window);
		terrain->display(window);
		// skybox->update(deltaTime, camera);
		// skybox->display(window);
		window.display();
	}

	return 0;
}