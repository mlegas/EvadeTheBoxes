#include "Level.h"
#include "Diamond.h"
#include "Floor.h"
#include "End.h"
#include <SFML/OpenGL.hpp>

Level::Level()
{
}

Level::~Level()
{
}

void Level::AddFloor(std::shared_ptr<Floor> _floor)
{
	m_floors.push_back(_floor);
}

void Level::AddDiamond(std::shared_ptr<Diamond> _diamond)
{
	m_diamonds.push_back(_diamond);
}

void Level::SetEnd(std::shared_ptr<End> _end)
{
	m_end = _end;
}

void Level::Update(sf::Time &_deltaTime)
{
	for (int i = 0; i < m_floors.size(); i++)
	{
		m_floors.at(i)->Update();
	}

	for (int i = 0; i < m_diamonds.size(); i++)
	{
		m_diamonds.at(i)->Update(_deltaTime);
	}

	m_end->Update();
}

void Level::Draw(std::shared_ptr<Camera> _camera)
{
	for (int i = 0; i < m_floors.size(); i++)
	{
		m_floors.at(i)->Draw(_camera);
	}

	for (int i = 0; i < m_diamonds.size(); i++)
	{
		glDisable(GL_CULL_FACE);
		m_diamonds.at(i)->Draw(_camera); // Sadly, the model is a bit broken and cullfacing therefore needs to be disabled for drawing diamonds.
										// It's only a couple of faces, so it should be alright.
		glEnable(GL_CULL_FACE);
	}

	m_end->Draw(_camera);
}