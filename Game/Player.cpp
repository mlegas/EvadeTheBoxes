#include "Player.h"
#include "BoundingBox.h"
#include "GameObject.h"
#include <glm/ext.hpp>

Player::Player()
{
	m_speed = 10.0f; // set the movement speed of the player
	m_position = glm::vec3(0.0f, 1.0f, 0.0f);
	
	// this line creates an imaginary 1.5x1.0x1.5 bounding box, as the player is represented as a small point in-game.
	// Possibly, if the window size changed, the bounding box would have to be resized as well to fit the new window dimensions properly.
	m_boundingBox = std::make_shared<BoundingBox>( glm::vec3(-0.75f, 0.0f, -0.75f), glm::vec3(0.75f, 1.0f, 0.75f) );
	m_crashAmount = 0;
}


Player::~Player()
{
}

void Player::update(glm::vec3 _position)
{
	m_position = _position;

	// update the position of the bounding box
	m_boundingBox->setMin(glm::vec3(m_position.x - 0.75f, m_position.y - 1.0f, m_position.z - 0.75f));
	m_boundingBox->setMax(glm::vec3(m_position.x + 0.75f, m_position.y, m_position.z + 0.75f));
}

void Player::checkCollisions(std::vector<std::shared_ptr<GameObject>> _boxes)
{
	std::vector<std::shared_ptr<GameObject>>::iterator boxIterator;

	for (boxIterator = _boxes.begin(); boxIterator != _boxes.end();)
	{
		if ((*boxIterator)->getPosition().y < 5.0f) // firstly check if the box is close enough to collide, broad phase test
		{
			// perform the AABB collision test.
			// reference: http://www.miguelcasillas.com/?p=30

			if (m_boundingBox->getMax().x > (*boxIterator)->getBoundingBox()->getMin().x &&
				m_boundingBox->getMin().x < (*boxIterator)->getBoundingBox()->getMax().x &&
				m_boundingBox->getMax().y >(*boxIterator)->getBoundingBox()->getMin().y &&
				m_boundingBox->getMin().y < (*boxIterator)->getBoundingBox()->getMax().y &&
				m_boundingBox->getMax().z >(*boxIterator)->getBoundingBox()->getMin().z &&
				m_boundingBox->getMin().z < (*boxIterator)->getBoundingBox()->getMax().z)
			{
				m_crashAmount++; // increment the amount of crashes
				(*boxIterator)->setCollision(true); // set the crate's collision status to true
			}
			else
			{
				(*boxIterator)->setCollision(false); // set the crate's collision status to false
			}
		}
		else
		{
			(*boxIterator)->setCollision(false); // set the crate's collision status to false
		}
		++boxIterator;
	}
}