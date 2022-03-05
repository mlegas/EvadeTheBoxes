#include "BoundingBox.h"


BoundingBox::BoundingBox(glm::vec3 _min, glm::vec3 _max)
{
	// this function is used for generating an imaginary bounding box for the player.
	m_min = _min;
	m_max = _max;

	m_position = glm::vec3((m_min.x + m_max.x) / 2.0f, (m_min.y + m_max.y) / 2.0f, (m_min.z + m_max.z) / 2.0f);
}

BoundingBox::BoundingBox(std::shared_ptr<std::vector<glm::vec3>> _modelPositions)
{
	std::vector<glm::vec3> temporaryModelPos = *_modelPositions;

		m_min.x = m_max.x = temporaryModelPos.at(0).x;
		m_min.y = m_max.y = temporaryModelPos.at(0).y;
		m_min.z = m_max.z = temporaryModelPos.at(0).z;

		// this loops reads the vertex positions from a vec3 vector after loading a model and
		// finds the max and min values for the bounding box.
		for (int i = 0; i < temporaryModelPos.size(); i++) 
		{
			if (temporaryModelPos.at(i).x < m_min.x) 
			{ 
				m_min.x = temporaryModelPos.at(i).x; 
			}

			if (temporaryModelPos.at(i).x > m_max.x) 
			{ 
				m_max.x = temporaryModelPos.at(i).x; 
			}

			if (temporaryModelPos.at(i).y < m_min.y) 
			{ 
				m_min.y = temporaryModelPos.at(i).y; 
			}

			if (temporaryModelPos.at(i).y > m_max.y) 
			{ 
				m_max.y = temporaryModelPos.at(i).y; 
			}

			if (temporaryModelPos.at(i).z < m_min.z) 
			{ 
				m_min.z = temporaryModelPos.at(i).z; 
			}

			if (temporaryModelPos.at(i).z > m_max.z) 
			{ 
				m_max.z = temporaryModelPos.at(i).z; 
			}
		}

	m_position = glm::vec3((m_min.x + m_max.x) / 2.0f, (m_min.y + m_max.y) / 2.0f, (m_min.z + m_max.z) / 2.0f); // sets the position to the centre
}


BoundingBox::~BoundingBox()
{
}
