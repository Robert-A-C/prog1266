/**
@file	 Airplane.cpp
@author  Robert Carll Robert-a-c@live.com
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt

Additions and modifications are my sole work for prog 1266

@section DESCRIPTION

Declaration for the airplane class

*/
#include "Airplane.h"
#include "Category.h"
#include "TextNode.h"
#include "TextureHolder.h"
#include <memory>
#include <cassert>
#include "Utility.h"
#include "DataTables.h"
#include <iostream>
namespace GEX {


	const std::map<Airplane::Type, AirplaneData> table = initalizeAircraftData();

	Airplane::Airplane(Type type) :
		Entity(table.at(type).hitpoints),
		_type(type),
		_player(TextureHolder::getInstance().get(table.at(type).texture)),
		_travelDistance(0),
		_missile_ammo(10)

	{
		centerOrigin(_player);

		// build fire and launch commands
		_fireCommand.category = Category::SceneAirLayer;
		_fireCommand.action = [this](SceneNode& node, sf::Time dt)
		{
			this->createBullet(node);
		};
		_launchMissileCommand.category = Category::SceneAirLayer;
		_launchMissileCommand.action = [this](SceneNode& node, sf::Time dt)
		{
			this->createProjectile(node, Projectile::Type::MISSILE, 0.f, 0.f);
		};

		std::unique_ptr<TextNode> healthDisplay(new TextNode(std::to_string(getHitpoints()) + " HP"));
		_healthDisplay = healthDisplay.get();
		healthDisplay->setPosition(0.f, 50.f);
		attachChild(std::move(healthDisplay));


	}	

	unsigned int Airplane::getCategory() const	// Gets the category based on the type
	{
		switch (_type)
		{
		case Type::EAGLE:
			return Category::PlayerAircraft;

		case Type::RAPTOR:
			return Category::EnemyAircraft;

		case Type::AVENGER:
			return Category::EnemyAircraft;
			

		default:
			assert(0); // missing type in switch
		}
	}

	void Airplane::updateCurrent(sf::Time dt, CommandQueue& command)		// updates health display and movement
	{
		sf::Vector2f velocity = getVelocity();
		if (velocity.x != 0.f && velocity.y != 0.f)
			setVelocity(velocity / std::sqrt(2.f));

		_healthDisplay->setText(std::to_string(getHitpoints()) + " HP");
		_healthDisplay->setPosition(0.f, 50.f);
		_healthDisplay->setRotation(-getRotation());

		updateTexts();
		checkProjectileLaunch(dt, command);

		movementUpdate(dt);
		Entity::updateCurrent(dt, command);
	}

	void Airplane::updateTexts()		// updates the Health display texts
	{
		_healthDisplay->setText(std::to_string(getHitpoints()) + " HP");
		_healthDisplay->setPosition(0.f, 50.f);
		_healthDisplay->setRotation(-getRotation());
	}

	void Airplane::movementUpdate(sf::Time dt)				// updates movement based on directions otherwise autopilots
	{
		const std::vector<Direction>& directions = table.at(_type).directions;

		if (!directions.empty()) //only auto pilot plane if I have directions
		{
			float distanceToTravel = directions.at(_directionIndex).distance;
			if (_travelDistance > distanceToTravel)
			{
				_directionIndex = (_directionIndex + 1) % directions.size();
				_travelDistance = 0;
			}
			_travelDistance += getMaxSpeed() * dt.asSeconds();

			float dirAngle = directions.at(_directionIndex).angle;
			float vx = getMaxSpeed() * GEX::cos(dirAngle);
			float vy = getMaxSpeed() * GEX::sin(dirAngle);
			setVelocity(vx, vy);
		}
	}

	bool Airplane::isAllied() const	// checks if Airplane object is of type Eagle
	{
		return _type == Type::EAGLE;		
	}

	float Airplane::getMaxSpeed() const // returns speed from table
	{
		return table.at(_type).speed;
	}

	sf::FloatRect Airplane::getBoundingRect() const	// get the rectangle of the sprites shape
	{
		return getWorldTransform().transformRect(_player.getGlobalBounds());
	}

	void Airplane::fire()		// shoots bullet
	{
		//std::cout << "Fire" << std::endl;
		//only shios with none zero fire rate fire bullets
		if (table.at(_type).fireInterval != sf::Time::Zero)
			_isFiring = true;
	}

	void Airplane::launchMissile()	// launches missile
	{
		//std::cout << "Missile" << std::endl;
		// only ships with missles can fire
		if (_missile_ammo > 0)
		{
			--_missile_ammo;
			_isLaunchingMissile = true;
		}
	}

	void Airplane::checkProjectileLaunch(sf::Time dt, CommandQueue & command)	// sends fire command and then sets _isFiring/_isLaunchingMissile to false 
	{
		// Enemies always fire
		if (!isAllied())
			fire();
		
		if (_isFiring)
		{
			command.push(_fireCommand);
			_isFiring = false;
		}

		if (_isLaunchingMissile)
		{
			command.push(_launchMissileCommand);
			_isLaunchingMissile = false;
		}

	}

	void Airplane::createBullet(SceneNode& node) const		// creates a bullet
	{	
		Projectile::Type type = isAllied() ? Projectile::Type::ALLIEDBULLET : Projectile::Type::ENEMYBULLET;
		createProjectile(node, type, 0, 0.5f);
	}

	void Airplane::createProjectile(SceneNode& node, Projectile::Type type, float xOffSet, float yOffSet) const		// creates a projectile 
	{
		// node is the SceneAirLayer node that the command was targeted to
		std::unique_ptr<Projectile> projectile(new Projectile(type));

		sf::Vector2f offSet(xOffSet * _player.getGlobalBounds().width, yOffSet * _player.getGlobalBounds().height);
		sf::Vector2f velocity(0, projectile->getMaxSpeed());

		float sign = isAllied() ? -1.f : 1.f;
		projectile->setPosition(getWorldPosition() + offSet * sign);
		projectile->setVelocity(velocity * sign);
		node.attachChild(std::move(projectile));
	}

	void Airplane::drawCurrent(sf::RenderTarget & target, sf::RenderStates state) const		// draws the sprite _player
	{
		target.draw(_player, state);
	}

}