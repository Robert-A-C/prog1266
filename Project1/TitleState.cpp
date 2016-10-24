/**
@file    TitleState.cpp
@author  Robert Carll Robert-a-c@live.com
@version 1.0

@section LICENSE

This software is based on the material accompanying the book "SFML Game Development" see License.txt

Additions and modifications are my sole work for prog 1266

@section DESCRIPTION

Declaration for the TitleState class

*/
#include "TitleState.h"
#include "TextureHolder.h"
#include "FontHolder.h"
#include "Utility.h"



namespace GEX
{

	TitleState::TitleState(StateStack& stack, Context context) :
		State(stack, context),
		_text(),
		_showText(true),
		_textEffectTime(sf::Time::Zero)
	{
		_backgroundSprite.setTexture(TextureHolder::getInstance().get(TextureID::TitleScreen));

		_text.setFont(FontHolder::getInstance().get(FontID::Main));
		_text.setString("Press any key to start");
		centerOrigin(_text);
		_text.setPosition(context.window->getView().getSize() / 2.f);
	}

	void TitleState::draw()		// draws the window
	{
		sf::RenderWindow& window = *getContext().window;
		window.draw(_backgroundSprite);

		if (_showText)
			window.draw(_text);
	}

	bool TitleState::update(sf::Time dt)				// updates title state
	{
		_textEffectTime += dt;

		if (_textEffectTime >= sf::seconds(0.5f))
		{
			_showText = !_showText;
			_textEffectTime = sf::Time::Zero;
		}

		return true;
	}

	bool TitleState::handleEvent(const sf::Event& event)		// handles events
	{
		// If any key is pressed, trigger the next screen
		if (event.type == sf::Event::KeyPressed)
		{
			requestStackPop();
			requestStackPush(StateID::Menu);
		}

		return true;
	}

}