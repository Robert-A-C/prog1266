/**
@file	 Application.cpp
@author  Robert Carll Robert-a-c@live.com
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt

Additions and modifications are my sole work for prog 1266

@section DESCRIPTION

Declaration for the application class

*/
#include "Application.h"
#include "TextureHolder.h"
#include "Utility.h"
#include "State.h"
#include "StateIdentifiers.h"
#include "TitleState.h"
#include "GameState.h"
#include "MenuState.h"
#include "PauseState.h"
#include "FontHolder.h"
#include "GexState.h"

namespace GEX 
{
	const sf::Time Application::TimePerFrame = sf::seconds(1.f / 60.f);
	
	Application::Application() :
		_window(sf::VideoMode(1024, 768), "States", sf::Style::Close),
		_player(),
		_stateStack(State::Context(_window, _player)),
		_statsText()
	{
		_window.setKeyRepeatEnabled(false);

		FontHolder::getInstance().load(FontID::Main, "../Media/Sansation.ttf");								//
		TextureHolder::getInstance().load(TextureID::TitleScreen, "../Media/Textures/TitleScreen.png");		//
		TextureHolder::getInstance().load(TextureID::MenuScreen, "../Media/Textures/Title2.png");			//
		TextureHolder::getInstance().load(TextureID::GexScreen, "../Media/Textures/face.png");				//
		TextureHolder::getInstance().load(TextureID::Eagle, "../Media/Textures/Eagle.png");					//	Loads all textures needed
		TextureHolder::getInstance().load(TextureID::Raptor, "../Media/Textures/Raptor.png");				//
		TextureHolder::getInstance().load(TextureID::Desert, "../Media/Textures/Desert.png");				//
		TextureHolder::getInstance().load(TextureID::Bullet, "../Media/Textures/Bullet.png");				//
		TextureHolder::getInstance().load(TextureID::Missile, "../Media/Textures/Missile.png");				//

		_statsText.setFont(FontHolder::getInstance().get(FontID::Main));	//
		_statsText.setPosition(5.f, 5.f);									// sets text for fps
		_statsText.setCharacterSize(10u);									//

		registerStates();
		_stateStack.pushState(StateID::Title);
	}

	void Application::run()				// runs the game loop
	{
		sf::Clock clock;
		sf::Time timeSinceLastUpdate = sf::Time::Zero;

		while (_window.isOpen())
		{
			sf::Time dt = clock.restart();
			timeSinceLastUpdate += dt;
			while (timeSinceLastUpdate > TimePerFrame)
			{
				timeSinceLastUpdate -= TimePerFrame;

				processInput();;
				update(TimePerFrame);

				if (_stateStack.isEmpty())
					_window.close();
			}
			updateStatistics(dt);
			render();

		}
	}

	void Application::processInput()		// processes event input
	{
		sf::Event event;
		while (_window.pollEvent(event))
		{
			_stateStack.handleEvent(event);

			if (event.type == sf::Event::Closed)
				_window.close();
		}
	}

	void Application::update(sf::Time dt)	// updates the stateStack
	{
		_stateStack.update(dt);
	}

	void Application::render()			// clears and redraws everthing
	{
		_window.clear();
		_stateStack.draw();
		_window.setView(_window.getDefaultView());
		_window.draw(_statsText);
		_window.display();
	}

	void Application::updateStatistics(sf::Time dt)		// updates the fps and time per update
	{
		static sf::Time updateTime = sf::Time::Zero;
		static int numFrames = 0;

		updateTime += dt;
		numFrames += 1;
		if (updateTime >= sf::seconds(1.0f))
		{
			_statsText.setString("Frames / Second = " + std::to_string(numFrames) + "\n" + "Time / Update = " + std::to_string(updateTime.asMicroseconds() / numFrames) + "us");

			updateTime -= sf::seconds(1.0f);
			numFrames = 0;
		}
	}
	
	void Application::registerStates()		// creates the games states
	{
		_stateStack.registerState<TitleState>(StateID::Title);
		_stateStack.registerState<MenuState>(StateID::Menu);
		_stateStack.registerState<GameState>(StateID::Game);
		_stateStack.registerState<PauseState>(StateID::Pause);
		_stateStack.registerState<GexState>(StateID::Gex);
	}
}