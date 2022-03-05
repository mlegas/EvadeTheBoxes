#pragma once
#include "Box.h"
#include <SFML/System.hpp>
class Diamond :
	public Box
{
public:
	Diamond(std::string path);
	~Diamond();
	void Update(sf::Time &_deltaTime);
private:
	float m_angle;
};

