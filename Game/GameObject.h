#pragma once
#include <glm/ext.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <memory>
#include <string>

class VertexArray;
class BoundingBox;
class Texture;
class Camera;
class ShaderProgram;

class GameObject
{
public:
	GameObject(std::string _model, std::string _texture, std::string _vertShader, std::string _fragShader);
	~GameObject();
	std::shared_ptr<BoundingBox> getBoundingBox() { return m_boundingBox; }
	glm::vec3 getPosition() { return m_position; }
	void setPosition(glm::vec3 _position) { m_position = _position; }
	void setCollision(bool _collision) { m_collision = _collision; }
	bool getCollision() { return m_collision; }
	void draw(std::shared_ptr<Camera> _camera);
	void update(sf::Time _deltaTime);
	void update();
private:
	bool m_collision;
	bool m_falling;
	float m_velocity;
	glm::mat4 m_modelMatrix;
	glm::vec3 m_position;
	std::shared_ptr<VertexArray> m_model;
	std::shared_ptr<BoundingBox> m_boundingBox;
	std::shared_ptr<Texture> m_texture;
	std::shared_ptr<ShaderProgram> m_shader;
	std::shared_ptr<std::vector<glm::vec3>> m_modelVertPositions;
};

