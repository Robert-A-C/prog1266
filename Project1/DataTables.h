/**
@file    DataTables.h
@author  Robert Carll Robert-a-c@live.com
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt

Additions and modifications are my sole work for prog 1266

@section DESCRIPTION

Definition for the DataTables class

*/
#pragma once
#include "ResourceIdentifiers.h"
#include "Airplane.h"
#include "Projectile.h"
#include <map>

namespace GEX {
	
	struct Direction	// direction table
	{
		Direction(float a, float d);
		float	angle;
		float	distance;
	};

	struct AirplaneData			// table for projectile data
	{
		int			hitpoints;
		float		speed;
		TextureID	texture;
		sf::Time	fireInterval;
		std::vector<Direction> directions;
	};

	struct ProjectileData		// table for Projectile data
	{
		int damage;
		float speed;
		TextureID texture;
	};

	std::map<Airplane::Type, AirplaneData> initalizeAircraftData();			// initalize all the data required for the airplane table
	std::map<Projectile::Type, ProjectileData> initalizeProjectileData();	// initaliza all the data required for the projectile table
}
