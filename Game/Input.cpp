#include "Input.h"
#include "Camera.h"
#include <memory>
#include <SFML/Graphics.hpp>

Input::Input()
{
	m_pressedKeys.resize(5, false); // initialize 5 booleans in the vector
}

Input::~Input()
{
}

void Input::checkForInput(std::shared_ptr<sf::RenderWindow> _window, bool &_quitGame, std::shared_ptr<Camera> _camera)
{
	sf::Event event;

	while (_window->pollEvent(event))
	{
		if (event.type == sf::Event::KeyPressed) // check which keys were pressed
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_pressedKeys.at(0) = true;
				break;
			case sf::Keyboard::S:
				m_pressedKeys.at(1) = true;
				break;
			case sf::Keyboard::A:
				m_pressedKeys.at(2) = true;
				break;
			case sf::Keyboard::D:
				m_pressedKeys.at(3) = true;
				break;
			case sf::Keyboard::Q:
				_quitGame = true;
				break;
			case sf::Keyboard::K:
				m_pressedKeys.at(4) = true;
				break;
			default:
				break;
			}
		}
		else if (event.type == sf::Event::KeyReleased) // check which keys were released
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W:
				m_pressedKeys.at(0) = false;
				break;
			case sf::Keyboard::S:
				m_pressedKeys.at(1) = false;
				break;
			case sf::Keyboard::A:
				m_pressedKeys.at(2) = false;
				break;
			case sf::Keyboard::D:
				m_pressedKeys.at(3) = false;
				break;
			case sf::Keyboard::K:
				m_pressedKeys.at(4) = true;
				break;
			default:
				break;
			}
		}

		if (event.type == sf::Event::Closed)
		{
			_quitGame = true;
		}

		if (event.type == sf::Event::MouseMoved)
		{
			glm::vec2 mousePos(event.mouseMove.x, event.mouseMove.y); // store the movement of the mouse, so we can move the camera in its update() function.

			_camera->setNewMousePos(mousePos); // set the new mousepos to the camera.
		}
	}
}