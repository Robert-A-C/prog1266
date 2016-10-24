/**
@file    Utility.cpp
@author  Robert Carll Robert-a-c@live.com
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt

Additions and modifications are my sole work for prog 1266

@section DESCRIPTION

Declaration for the Utility class

*/
#include "Utility.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>

#include <cmath>


namespace GEX
{
	void centerOrigin(sf::Sprite& sprite)	// centers sprite
	{
		sf::FloatRect bounds = sprite.getLocalBounds();
		sprite.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
	}

	void centerOrigin(sf::Text& text)	// centers text
	{
		sf::FloatRect bounds = text.getLocalBounds();
		text.setOrigin(std::floor(bounds.left + bounds.width / 2.f), std::floor(bounds.top + bounds.height / 2.f));
	}

	float length(sf::Vector2f vector)
	{
		return sqrt(vector.x * vector.x + vector.y * vector.y);
	}

	sf::Vector2f unitVector(sf::Vector2f vector)
	{
		assert(vector != sf::Vector2f(0.f, 0.f));
		return vector / length(vector);
	}

}
