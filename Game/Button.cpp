#include "Button.h"
#include "SetOriginToCenter.h"

// These functions come from my assignment from the first year. I decided to re-use and modify it, as it is still beneficial for this assignment.

Button::Button(int _action, sf::Texture &_texture, sf::Text &_text, sf::Vector2f _position)
{
	m_action = _action;

	setOriginToCenter(_text);

	_text.setOutlineThickness(3);
	
	sf::Sprite button(_texture);
	setOriginToCenter(button);
	
	button.setPosition(_position);
	_text.setPosition(_position);
	
	m_sprite = button;
}

Button::~Button()
{

}

int Button::getAction()
{
	return m_action;
}

sf::Sprite Button::getSprite()
{
	return m_sprite;
}

bool Button::isClicked(std::shared_ptr<sf::RenderWindow> _window)
{
	if (m_sprite.getGlobalBounds().contains(_window->mapPixelToCoords(sf::Mouse::getPosition(*_window))))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Button::isHovered(std::shared_ptr<sf::RenderWindow> _window)
{
	if (m_sprite.getGlobalBounds().contains(_window->mapPixelToCoords(sf::Mouse::getPosition(*_window))))
	{
		m_sprite.setColor(sf::Color(255, 255, 255, 128)); // make the buttons transparent if the player places his mouse on the button
	}
	else
	{
		m_sprite.setColor(sf::Color(255, 255, 255, 200));
	}
}