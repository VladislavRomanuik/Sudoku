#pragma once
#include <SFML/Graphics.hpp>

class Collider
{
public:
	sf::Vector2f position;
	sf::Vector2f size;

	bool IsPointInside(sf::Vector2f checkPos);
};

