#pragma once
#include <memory>
#include <vector>
#include <glm/ext.hpp>

class BoundingBox
{
public:
	BoundingBox(glm::vec3 _min, glm::vec3 _max);
	BoundingBox(std::shared_ptr<std::vector<glm::vec3>> _modelPositions);
	~BoundingBox();
	glm::vec3 getPosition() { return m_position; }
	glm::vec3 getMax() { return m_max; }
	glm::vec3 getMin() { return m_min; }
	void setMax(glm::vec3 _max) { m_max = _max; }
	void setMin(glm::vec3 _min) { m_min = _min; }

private:
	glm::vec3 m_position;
	glm::vec3 m_max;
	glm::vec3 m_min;
};

