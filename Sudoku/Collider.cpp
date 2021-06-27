#include "Collider.h"

bool Collider::IsPointInside(sf::Vector2f checkPos)
{
	return (checkPos.x > position.x && checkPos.x < position.x + size.x &&
		checkPos.y > position.y && checkPos.y < position.y + size.y);
}