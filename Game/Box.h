#pragma once
#include <glm/ext.hpp>
#include <vector>
#include <memory>

class VertexArray;
class BoundingBox;
class Texture;
class Camera;
class ShaderProgram;

class Box
{
public:
	Box();
	~Box();
	std::shared_ptr<BoundingBox> GetBoundingBox() { return m_boundingBox; }
	void SetScale(float _scale) { m_scale = glm::vec3(_scale, _scale, _scale); }
	void Draw(std::shared_ptr<Camera> _camera);
	void Update();
protected:
	glm::mat4 m_modelMatrix;
	glm::vec3 m_position;
	glm::vec3 m_scale;
	std::shared_ptr<VertexArray> m_model;
	std::shared_ptr<BoundingBox> m_boundingBox;
	std::shared_ptr<Texture> m_texture;
	std::shared_ptr<ShaderProgram> m_shader;
	std::shared_ptr<std::vector<glm::vec3>> m_modelVertPositions;
};

