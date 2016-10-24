/**
@file    Projectile.cpp
@author  Robert Carll Robert-a-c@live.com
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt

Additions and modifications are my sole work for prog 1266

@section DESCRIPTION

Declaration for the Projectile class

*/
#include "Projectile.h"
#include "Category.h"
#include "TextNode.h"
#include "TextureHolder.h"
#include <cassert>
#include "Utility.h"
#include "DataTables.h"
#include <iostream>
namespace GEX {


	const std::map<Projectile::Type, ProjectileData> table = initalizeProjectileData(); 

	Projectile::Projectile(Type type) :
		Entity(1),
		_type(type),
		_sprite(TextureHolder::getInstance().get(table.at(type).texture))
	{
			 centerOrigin(_sprite);
	}

	

	unsigned int Projectile::getCategory() const	// gets catergory
	{
		
		switch (_type)
		{
		case Type::ENEMYBULLET:
			return Category::EnemyProjectile;
			break;

		case Type::ALLIEDBULLET:
		case Type::MISSILE:
			return Category::AlliedProjectile;
			break;

		default:
			assert(0);
			return Category::None;
			break;
		}
	
	}

	void Projectile::updateCurrent(sf::Time dt, CommandQueue& command)	// updates entity
	{				
		if (isGuided())
		{
			const float approachRate = 200.f;
			sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * _targetDirection + getVelocity());
			newVelocity *= getMaxSpeed();

			setVelocity(newVelocity);
		}

		Entity::updateCurrent(dt, command);
	}

	

	float Projectile::getMaxSpeed() const	// gets speed
	{
		return table.at(_type).speed;
	}

	bool Projectile::isGuided() const
	{
		return _type == Type::MISSILE;
	}

	void Projectile::guideTowards(sf::Vector2f position)
	{
		assert(isGuided());
		_targetDirection = unitVector(position - getWorldPosition());
	}
	
	void Projectile::drawCurrent(sf::RenderTarget & target, sf::RenderStates state) const	// draws the sprite
	{
		target.draw(_sprite, state);

	}



}