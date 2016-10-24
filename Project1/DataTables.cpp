/**
@file    DataTables.cpp
@author  Robert Carll Robert-a-c@live.com
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt

Additions and modifications are my sole work for prog 1266

@section DESCRIPTION

Declaration for the DataTables class

*/
#include "DataTables.h"

namespace GEX {
	std::map<Airplane::Type, AirplaneData> GEX::initalizeAircraftData()		// initalizes data in the airplane table
	{
		 std::map<Airplane::Type, AirplaneData> data;

		 data[Airplane::Type::RAPTOR].hitpoints = 20;
		 data[Airplane::Type::RAPTOR].speed = 80.f;
		 data[Airplane::Type::RAPTOR].texture = TextureID::Raptor;
		 data[Airplane::Type::RAPTOR].fireInterval = sf::Time::Zero;

		 data[Airplane::Type::AVENGER].hitpoints = 40;
		 data[Airplane::Type::AVENGER].speed = 50.f;
		 data[Airplane::Type::AVENGER].texture = TextureID::Avenger;
		 data[Airplane::Type::AVENGER].fireInterval = sf::seconds(2);

		 data[Airplane::Type::EAGLE].hitpoints = 100;
		 data[Airplane::Type::EAGLE].speed = 200.f;
		 data[Airplane::Type::EAGLE].fireInterval = sf::seconds(1);
		 data[Airplane::Type::EAGLE].texture = TextureID::Eagle;

		 data[Airplane::Type::RAPTOR].directions.push_back(Direction(+30.f, 90.f));
		 data[Airplane::Type::RAPTOR].directions.push_back(Direction(+30.f, -90.f));
		 
		 

		 return data;
	}
	std::map<Projectile::Type, ProjectileData> GEX::initalizeProjectileData()	// initalizes projectile data in projectile table
	{
		std::map<Projectile::Type, ProjectileData> data;

		data[Projectile::Type::ALLIEDBULLET].damage = 10;
		data[Projectile::Type::ALLIEDBULLET].speed = 300;
		data[Projectile::Type::ALLIEDBULLET].texture = TextureID::Bullet;

		data[Projectile::Type::MISSILE].damage = 100;
		data[Projectile::Type::MISSILE].speed = 200.f;
		data[Projectile::Type::MISSILE].texture = TextureID::Missile;

		data[Projectile::Type::ENEMYBULLET].damage = 10;
		data[Projectile::Type::ENEMYBULLET].speed = 300;
		data[Projectile::Type::ENEMYBULLET].texture = TextureID::Bullet;

		return data;
	}

	Direction::Direction(float a, float d) : angle(a), distance(d)
	{
		
	}
}