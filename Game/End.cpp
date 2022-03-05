#include "End.h"
#include "VertexArray.h"
#include "Texture.h"
#include "ShaderProgram.h"
#include "BoundingBox.h"

End::End(std::string path)
{
	m_model = std::make_shared<VertexArray>(path, m_modelVertPositions);
	m_texture = std::make_shared<Texture>("Textures/button.png");
	m_shader = std::make_shared<ShaderProgram>("Shaders/Tex.vert", "Shaders/LightingTex.frag");
	m_boundingBox = std::make_shared<BoundingBox>(m_modelVertPositions, false);
	m_modelMatrix = glm::mat4(1.0f);
}

End::~End()
{
}