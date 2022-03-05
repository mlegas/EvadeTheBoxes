#pragma once
#include <glm/ext.hpp>
#include <memory>
#include <SFML/Graphics.hpp>

class Player;

class Camera
{
public:
	glm::mat4 getViewMatrix() { return m_viewMatrix; }
	glm::mat4 getProjectionMatrix() { return m_projectionMatrix; }
	glm::vec3 getPosition() { return m_position; }
	void setNewMousePos(glm::vec2 _mousePos) { m_mousePos = _mousePos; }
	Camera(float _speed, float _windowWidth, float _windowHeight);
	~Camera();
	void update(std::vector<bool> _pressedKeys, std::shared_ptr<sf::RenderWindow> _window, sf::Time _deltaTime);
private:
	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;
	float m_midX;
	float m_midY;
	float m_cameraAngleX;
	float m_cameraAngleY;
	float m_mouseSpeed;
	float m_cameraSpeed;
	glm::vec2 m_mousePos;
	glm::vec2 m_oldMousePos;
	glm::vec3 m_direction;
	glm::vec3 m_movementDirection;
	glm::vec3 m_right;
	glm::vec3 m_up;
	glm::vec3 m_position;
	glm::vec3 m_translation;
};

