#include "Box.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Texture.h"
#include "BoundingBox.h"
#include "Camera.h"


Box::Box()
{
	m_modelVertPositions = std::make_shared<std::vector<glm::vec3>>();
	m_model = std::make_shared<VertexArray>("Models/wooden_crate.obj", m_modelVertPositions);
	m_texture = std::make_shared<Texture>("Textures/Specular_map.png");
	m_shader = std::make_shared<ShaderProgram>("Shaders/boxShader.vert", "Shaders/boxShader.frag");
	m_boundingBox = std::make_shared<BoundingBox>(m_modelVertPositions);
	m_modelMatrix = glm::mat4(1.0f);
}



Box::~Box()
{
}

void Box::Update()
{
	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
	m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation.x, glm::vec3(1, 0, 0));
	m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation.y, glm::vec3(0, 1, 0));
	m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation.z, glm::vec3(0, 0, 1));
}

void Box::Draw(std::shared_ptr<Camera> _camera)
{
	m_shader->setUniform("in_Projection", _camera->GetProjectionMatrix());
	m_shader->setUniform("in_View", _camera->GetViewMatrix());
	m_shader->setUniform("in_Model", m_modelMatrix);

	if (m_texture != nullptr)
	{
		m_shader->setUniform("in_Texture", m_texture);
	}
	m_shader->draw(m_model);
}