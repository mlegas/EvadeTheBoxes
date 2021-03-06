#include <GL/glew.h>
#include <glm/glm.hpp>

#include <memory>
#include <string>
#include <vector>

class VertexArray;
class Texture;

struct Sampler
{
  GLint id;
  std::shared_ptr<Texture> texture;
};

class ShaderProgram
{
  GLuint id;
  std::vector<Sampler> samplers;

public:
  ShaderProgram(std::string vert, std::string frag);
  void draw(std::shared_ptr<VertexArray> vertexArray);
  void setUniform(std::string uniform, glm::vec3 value);
  void setUniform(std::string uniform, glm::vec4 value);
  void setUniform(std::string uniform, float value);
  void setUniform(std::string uniform, glm::mat4 value);
  void setUniform(std::string uniform, std::shared_ptr<Texture> texture);
  void printShaderInfoLog(GLuint obj);
  GLuint getId();

};
