#pragma once
#include "Scene.h"

class Triangle : public Scene {
	public :
		Triangle();
		~Triangle();
		virtual void display(sf::Window& window) override;
		virtual void update(sf::Time elapsed) override;
};

