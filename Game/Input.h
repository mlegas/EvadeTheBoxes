#pragma once
#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

class Camera;

class Input
{
public:
	Input();
	~Input();
	void checkForInput(std::shared_ptr<sf::RenderWindow> _window, bool &_quitGame, std::shared_ptr<Camera> _camera);
	std::vector<bool> getPressedKeys() { return m_pressedKeys; }
private:
	std::vector<bool> m_pressedKeys;
};

