/**
@file    World.h
@author  Robert Carll Robert-a-c@live.com
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt

Additions and modifications are my sole work for prog 1266

@section DESCRIPTION

Definition for the World class

*/
#pragma once
#include <SFML\Graphics.hpp>
#include "SceneNode.h"
#include <array>
#include <vector>
#include "CommandQueue.h"
#include "Airplane.h"

namespace GEX 
{
	
	class Airplane;
	
	class World
	{
	private:
		struct SpawnPoint
		{
			SpawnPoint(Airplane::Type type, float _x, float _y) :
				type(type),
				x(_x),
				y(_y)
			{}

			Airplane::Type	type;
			float			x;
			float			y;
		};

	public:
		explicit	World(sf::RenderWindow& window);

		void update(sf::Time);	// updates everything
		void guideMissiles();
		void draw();			// draws the window
		
		CommandQueue&		getCommandQueue();



	private:
		
		void			buildScene();						// builds the scene
		sf::FloatRect	getViewBounds() const;				// gets view bounds
		sf::FloatRect	getBattleFieldBounds() const;		// gets battlefield bounds

		void	spawnEnemies();												// spawns enemies
		void	addEnemies();												//
		void	addEnemy(Airplane::Type type, float relX, float relY);		//
		void	addEnemy(SpawnPoint point);									//

	private:
		enum Layer
		{
			Background,
			Air,
			LayerCount			
		};

		

	private:
		sf::RenderWindow&						_window;
		sf::View								_worldView;
		sf::FloatRect							_worldBounds;
		
		SceneNode								_sceneGraph;
		std::array<SceneNode*, LayerCount>		_sceneLayers;
		CommandQueue							_commandQueue;
		
		sf::Vector2f							_spawnPosition;
		float									_scrollSpeed;
		Airplane*								_playerAirplane;
		std::vector<SpawnPoint>					_enemieSpawnPoints;
		std::vector<Airplane*>					_activeEnemies;
	};
}