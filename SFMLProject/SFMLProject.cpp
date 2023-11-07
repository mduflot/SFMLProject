#include <iostream>
#include <SFML/Graphics.hpp>
#include <GL/glew.h>
#include "Scene.h"
#include "Triangle.h"
#include "Rectangle.h"

int main() {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	// settings.stencilBits = 8;
	sf::Window window(sf::VideoMode(1024, 768, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);
	sf::Clock clock;
	glewExperimental = GL_TRUE;
	glewInit();

	// Scene* scene = new Triangle;
	Scene* scene = new Rectangle;

	while (window.isOpen()) {

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		scene->update(clock.restart());
		scene->display(window);
		window.display();
	}

	return 0;
}