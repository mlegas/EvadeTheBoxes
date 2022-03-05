#pragma once
#include <SFML/Graphics.hpp>
#include <memory>

// This class comes from my assignment from the first year. I decided to re-use and modify it, as it is still beneficial for this assignment.

class Button
{
	public:
		Button(int _action, sf::Texture &_texture, sf::Text &_text, sf::Vector2f _position);
		~Button();
		int getAction();
		sf::Sprite getSprite();
		bool isClicked(std::shared_ptr<sf::RenderWindow> _window);
		void isHovered(std::shared_ptr<sf::RenderWindow> _window);

	private:
		int m_action;
		sf::Sprite m_sprite;
};