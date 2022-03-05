#include "GameObject.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "Texture.h"
#include "BoundingBox.h"
#include "Camera.h"

// creates a GameObject using file paths to the 3d model, texture, vertexShader and fragmentShader.
GameObject::GameObject(std::string _model, std::string _texture, std::string _vertShader, std::string _fragShader)
{
	m_collision = false;
	m_modelVertPositions = std::make_shared<std::vector<glm::vec3>>(); // creates a vector for the vertices of the model
	m_model = std::make_shared<VertexArray>(_model, m_modelVertPositions); // passes the vector to the constructor of VertexArray, which will fill it with
																		   // the positions of each triangle vertices of the model loaded.
	m_texture = std::make_shared<Texture>(_texture);
	m_shader = std::make_shared<ShaderProgram>(_vertShader, _fragShader);
	m_boundingBox = std::make_shared<BoundingBox>(m_modelVertPositions); // passes the model vertices to create the bounding box.
	m_position = m_boundingBox->getPosition(); // sets the position to the centre of the model.
	m_modelMatrix = glm::mat4(1.0f);
	m_velocity = -50.0f; // sets an initial velocity
}

GameObject::~GameObject()
{
}

// this is the update function for the crates
void GameObject::update(sf::Time _deltaTime)
{
	// using Euler to find the approximation of position while falling
	m_velocity += -9.81f * _deltaTime.asSeconds();
	m_position.y += m_velocity * _deltaTime.asSeconds();

	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);

	// move the bounding box as well. Since the models for the crates are 2x2 in size, 
	// I could predefine the size of the bounding box here.
	// This would need to be changed if more models were being used in the game with different bounding boxes.
	m_boundingBox->setMin(glm::vec3(m_position.x - 1.0f, m_position.y - 1.0f, m_position.z - 1.0f));
	m_boundingBox->setMax(glm::vec3(m_position.x + 1.0f, m_position.y + 1.0f, m_position.z + 1.0f));
}

// this is the update function for the floor
void GameObject::update()
{
	m_modelMatrix = glm::mat4(1.0f);
	m_modelMatrix = glm::translate(m_modelMatrix, m_position);
}

void GameObject::draw(std::shared_ptr<Camera> _camera)
{
	m_shader->setUniform("in_Projection", _camera->getProjectionMatrix());
	m_shader->setUniform("in_View", _camera->getViewMatrix());
	m_shader->setUniform("in_Model", m_modelMatrix);
	m_shader->setUniform("in_Texture", m_texture);

	// the floor and cubes share the same lighting properties,
	// as I decided that both the floor and boxes need to be properly lit
	// in a space setting so the player distinguishes them easily from the background.
	m_shader->setUniform("in_LightPos", glm::vec3(20.0f, 10.0f, 0.0f));
	m_shader->setUniform("in_Emissive", glm::vec3(0.6f, 0.6f, 0.6f));
	m_shader->setUniform("in_Ambient", glm::vec3(0.2f, 0.2f, 0.2f));
	m_shader->draw(m_model);
}