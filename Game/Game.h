#include <SFML/Graphics.hpp>
#include <memory>
#pragma once

class Level;

class Game
{
public:
	Game();
	~Game();
	void initialize();
private:
	void menu();
	void gameLoop();
	void instructions();
	std::shared_ptr<sf::RenderWindow> m_window;
	int m_windowWidth;
	int m_windowHeight;
	bool m_quitGame;
	bool m_quitProgram;
	bool m_choiceMade;
};

