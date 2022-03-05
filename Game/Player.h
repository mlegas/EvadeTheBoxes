#pragma once
#include <vector>
#include <memory>
#include "BoundingBox.h"
#include "GameObject.h"
#include <SFML/System.hpp>

class BoundingBox;
class GameObject;

class Player
{
public:
	Player();
	~Player();
	void checkCollisions(std::vector<std::shared_ptr<GameObject>> _boxes);
	glm::vec3 getPosition() { return m_position; }
	void update(glm::vec3 _position);
	float getSpeed() { return m_speed; }
	int getCrashAmount() { return m_crashAmount; }
private:
	int m_crashAmount;
	float m_speed;
	glm::vec3 m_position;
	std::shared_ptr<BoundingBox> m_boundingBox;
};

