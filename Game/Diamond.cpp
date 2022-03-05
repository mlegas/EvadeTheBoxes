#include "Diamond.h"
#include "VertexArray.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "BoundingBox.h"
#include <glm/ext.hpp>


Diamond::Diamond(std::string path)
{
	m_model = std::make_shared<VertexArray>(path, m_modelVertPositions);
	m_shader = std::make_shared<ShaderProgram>("Shaders/diamondShader.vert", "Shaders/diamondShader.frag");

	m_boundingBox = std::make_shared<BoundingBox>(m_modelVertPositions, false);
	m_modelMatrix = glm::mat4(1.0f);
	m_angle = 0.0f;
}


Diamond::~Diamond()
{
}

void Diamond::Update(sf::Time &_deltaTime)
{
	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::rotate(m_modelMatrix, glm::radians(m_angle * _deltaTime.asSeconds()), glm::vec3(0, 1, 0));

	m_shader->setUniform("in_Model", m_modelMatrix);
	m_shader->setUniform("in_Color", glm::vec4(0, 0, 0.695, 1.0));
	m_angle += 0.1f;

	if (m_angle >= 360.0f)
	{
		m_angle = 0.0f;
	}
}