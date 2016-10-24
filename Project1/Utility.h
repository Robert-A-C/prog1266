/**
@file    Utility.h
@author  Robert Carll Robert-a-c@live.com
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt

Additions and modifications are my sole work for prog 1266

@section DESCRIPTION

Definition for the Utility class

*/
#pragma once
#include <cmath>
#include <cassert>
#include <SFML/System/Vector2.hpp>

namespace sf
{
	class Sprite;
	class Text;
}

namespace GEX
{
	void centerOrigin(sf::Sprite& sprite);	// centers sprite
	void centerOrigin(sf::Text& text);		// centers text

	//trig helper functions
	const float M_PI = 3.14159265f;
	inline float degreesToRadians(float degrees) { return degrees * M_PI / 180.f; }
	inline float radiansToDegrees(float rads) { return rads * 180.f / M_PI; }
	inline float sin(float deg) { return std::sin(degreesToRadians(deg)); }
	inline float cos(float deg) { return std::sin(degreesToRadians(deg)); }
	inline float arctan2(float opp, float adj) { return 0.F; }
	float length(sf::Vector2f vector);
	sf::Vector2f unitVector(sf::Vector2f vector);
}