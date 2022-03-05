#include "Camera.h"
#include "Player.h"

Camera::Camera(float _speed, float _windowWidth, float _windowHeight)
{
	m_position = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::vec3 initialDirection = glm::vec3(0.0, 0.0f, -1.0f);

	m_midX = _windowWidth / 2.0f;
	m_midY = _windowHeight / 2.0f;

	m_cameraAngleX = 3.14f;
	m_cameraAngleY = 0.0f;

	m_cameraSpeed = _speed;
	m_mouseSpeed = 0.005f;

	m_mousePos = glm::vec2(m_midX, m_midY);
	m_oldMousePos = glm::vec2(m_midX, m_midY);
	m_up = glm::vec3(0, 1, 0);

	m_viewMatrix = glm::inverse(glm::lookAt(m_position, initialDirection, m_up));
	m_projectionMatrix = glm::perspective(glm::radians(45.0f), (float)_windowWidth / (float)_windowHeight, 0.1f, 300.f);
}


Camera::~Camera()
{
}

void Camera::update(std::vector<bool> _pressedKeys, std::shared_ptr<sf::RenderWindow> _window, sf::Time _deltaTime)
{
	float diffX = m_mousePos.x - m_oldMousePos.x;
	float diffY = m_mousePos.y - m_oldMousePos.y;

	// calculate the angles from the mouse movement
	m_cameraAngleX += -(m_mouseSpeed * ((float) m_mousePos.x - (float)m_oldMousePos.x + diffX));
	m_cameraAngleY += -(m_mouseSpeed * ((float) m_mousePos.y - (float)m_oldMousePos.y + diffY));

	if (m_cameraAngleY > 1.55334f) // limit the camera to 180 degrees Y rotation
	{
		m_cameraAngleY = 1.55334f;
	}

	else if (m_cameraAngleY < -1.55334f) // limit the camera to 180 degrees Y rotation
	{
		m_cameraAngleY = -1.55334f;
	}

	m_oldMousePos = m_mousePos;

	// reference used for camera: https://learnopengl.com/Getting-started/Camera

	// calculate the direction the player is pointing at for the view matrix, and movement
	m_direction = glm::vec3(cos(m_cameraAngleY) * sin(m_cameraAngleX), sin(m_cameraAngleY), cos(m_cameraAngleY) * cos(m_cameraAngleX));

	// this vec3 is used for movement, discarding the Y rotation of the camera, so the player moves with the same speed with any Y angle of the camera.
	// Without this, a player looking straight at the sky would not move, since the forward vector would be pointing the same way as the view.
	m_movementDirection = glm::vec3(sin(m_cameraAngleX), sin(m_cameraAngleY), cos(m_cameraAngleX));

	m_right = glm::vec3(sin(m_cameraAngleX - 3.14f / 2.0f), 0, cos(m_cameraAngleX - 3.14f / 2.0f)); // obtain the right vector from angle calculation
	m_up = glm::cross(m_right, m_direction); // obtain the up vector from a cross product of the forward vector and right vector

	if (_pressedKeys.at(0)) // set the new position using Euler
	{
		m_translation = (m_movementDirection * m_cameraSpeed) * _deltaTime.asSeconds();
		m_position += m_translation;
	}
	else if (_pressedKeys.at(1))
	{
		m_translation = -(m_movementDirection * m_cameraSpeed) * _deltaTime.asSeconds();
		m_position += m_translation;
	}

	if (_pressedKeys.at(2))
	{
		m_translation = -(m_right * m_cameraSpeed) * _deltaTime.asSeconds();
		m_position += m_translation;
	}
	else if (_pressedKeys.at(3))
	{
		m_translation = (m_right * m_cameraSpeed) * _deltaTime.asSeconds();
		m_position += m_translation;
	}

	if (m_position.x > 10.0f) // disallow the player from falling off the plane
	{
		m_position.x = 10.0f;
	}

	else if (m_position.x < -10.0f)
	{
		m_position.x = -10.0f;
	}

	if (m_position.z > 10.0f)
	{
		m_position.z = 10.0f;
	}

	else if (m_position.z < -10.0f)
	{
		m_position.z = -10.0f;
	}

	m_position.y = 1.0f; // lock the player's Y axis of movement
	
	m_viewMatrix = glm::lookAt(m_position, m_position + m_direction, m_up); // update the view matrix
}