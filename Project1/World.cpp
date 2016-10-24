/**
@file    World.cpp
@author  Robert Carll Robert-a-c@live.com
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt

Additions and modifications are my sole work for prog 1266

@section DESCRIPTION

Declaration for the World class

*/
#include "World.h"
#include "Airplane.h"
#include "TextureHolder.h"
#include "SpriteNode.h"
#include <algorithm>

namespace GEX {
	World::World(sf::RenderWindow& window) : 
	_window(window),
	_worldView(window.getDefaultView()),

	_sceneGraph(),
	_sceneLayers(),
	_commandQueue(),


	_worldBounds(0.f,0.f, _worldView.getSize().x, 2000.f),
	_spawnPosition( (_worldView.getSize().x / 2.f), (_worldBounds.height - _worldView.getSize().y / 2.f) ),
	_scrollSpeed(-50.0f),
	_playerAirplane(nullptr)
	{

		
		buildScene();

		// start the view at the spawn
		_worldView.setCenter(_spawnPosition);
	}

	void World::update(sf::Time dt)		// updates everything
	{
		//_worldView.zoom(1.0001);
		//_worldView.rotate(0.001);


		_worldView.move(0.f, _scrollSpeed * dt.asSeconds());
		_playerAirplane->setVelocity(0.f, 0.f);

		guideMissiles();

		while (!_commandQueue.isEmpty())
			_sceneGraph.onCommand(_commandQueue.pop(), dt);		

		spawnEnemies();

		//apply  movements 
		_sceneGraph.update(dt, getCommandQueue());
	}

	void World::guideMissiles()
	{
		// set up command to get a list of all enemy planes
		Command enemyCollector;
		enemyCollector.category = Category::EnemyAircraft;
		enemyCollector.action = derivedAction<Airplane>([this](Airplane& enemy, sf::Time dt)
		{
			if (!enemy.isDestroyed())
				this->_activeEnemies.push_back(&enemy);
		});

		Command missileGuider;
		missileGuider.category = Category::AlliedProjectile;
		missileGuider.action = derivedAction<Projectile>([this](Projectile& missile, sf::Time dt)
		{
			if (!missile.isGuided())
				return;

			float minDistance = std::numeric_limits<float>::max();
			Airplane* closestEnemy = nullptr;

			for (Airplane* enemy : this->_activeEnemies)
			{
				float enemyDistance = distance(missile, *enemy);

				if (enemyDistance < minDistance)
				{
					closestEnemy = enemy;
					minDistance = enemyDistance;
				}
			}
			if (closestEnemy)
			{
				missile.guideTowards(closestEnemy->getWorldPosition());

			}
		});
		// clear enemy list
		_activeEnemies.clear();

		// push commands
		_commandQueue.push(enemyCollector);
		_commandQueue.push(missileGuider);
		
	}

	

	void World::draw()				// draws the window
	{
		_window.setView(_worldView);
		_window.draw(_sceneGraph);
	}

	CommandQueue & World::getCommandQueue()		// returns command queue
	{
		return _commandQueue;
	}

	sf::FloatRect World::getViewBounds() const	// gets view bounds
	{
		return sf::FloatRect(_worldView.getCenter() - _worldView.getSize() / 2.f, _worldView.getSize());
	}
	
	sf::FloatRect World::getBattleFieldBounds() const		// gets battlefield bounds 
	{
		sf::FloatRect bounds = getViewBounds();
		bounds.top -= 100;
		bounds.height += 100;
		return bounds;
	}

	void World::spawnEnemies()	// spawns enemies
	{
		while (!_enemieSpawnPoints.empty() && _enemieSpawnPoints.back().y > getBattleFieldBounds().top)
		{
			auto spawn = _enemieSpawnPoints.back();
			std::unique_ptr<Airplane> enemy(new Airplane(spawn.type));
			enemy->setPosition(spawn.x, spawn.y);
			enemy->setRotation(180.f);
			_sceneLayers[Air]->attachChild(std::move(enemy));
			_enemieSpawnPoints.pop_back();
		}
	}

	void World::addEnemies()		// adds the enemies at certain points
	{
		addEnemy(Airplane::Type::RAPTOR, -250.f, 200.f);
		addEnemy(Airplane::Type::RAPTOR, 250.f, 200.f);
		addEnemy(Airplane::Type::RAPTOR, -350.f, 400.f);
		addEnemy(Airplane::Type::RAPTOR, 350.f, 400.f);
		
		addEnemy(Airplane::Type::RAPTOR, -250.f, 800.f);
		addEnemy(Airplane::Type::RAPTOR, 250.f, 800.f);
		addEnemy(Airplane::Type::RAPTOR, -350.f, 1000.f);
		addEnemy(Airplane::Type::RAPTOR, 350.f, 1000.f);
		
		std::sort(_enemieSpawnPoints.begin(), _enemieSpawnPoints.end(), [](SpawnPoint lhs, SpawnPoint rhs) {return lhs.y < rhs.y; });
		
	}

	void World::addEnemy(Airplane::Type type, float relX, float relY)	// spawn enemy function
	{
		addEnemy(SpawnPoint(type, relX, relY));
	}
		
	void World::addEnemy(SpawnPoint point)		// spawn enemy to spawn points 
	{
		point.x = _spawnPosition.x + point.x;
		point.y = _spawnPosition.y - point.y;
		_enemieSpawnPoints.push_back(point);
	}

	void World::buildScene()		// builds the scene
	{
		// build layer nodes for scene graph
		for (std::size_t i = 0; i < LayerCount; ++i)
		{
			Category::Type category = (i == Air) ? Category::SceneAirLayer : Category::None;
			SceneNode::Ptr layer(new SceneNode(category));
			_sceneLayers[i] = layer.get();
			_sceneGraph.attachChild(std::move(layer));
		}

		//prepare tiled background
		sf::Texture& texture = TextureHolder::getInstance().get(TextureID::Desert);
		sf::IntRect textureRect(_worldBounds);
		texture.setRepeated(true);

		//add background to sceneGraph
		std::unique_ptr<SpriteNode> background(new SpriteNode(texture, textureRect));
		background->setPosition(_worldBounds.left, _worldBounds.top);
		_sceneLayers[Background]->attachChild(std::move(background));

		// add planes
		std::unique_ptr<Airplane> plane(new Airplane(Airplane::Type::EAGLE));
		plane->setPosition(_spawnPosition);
		plane->setVelocity(40.f, _scrollSpeed);
		_playerAirplane = plane.get();
		_sceneLayers[Air]->attachChild(std::move(plane));


		addEnemies();

		
	}

}