#include <GL/glew.h>
#include <glm/glm.hpp>

#include <vector>
#include <memory>
#include <string>

class VertexBuffer;
class Level;
class GameObject;

class VertexArray
{
  GLuint id;
  bool dirty;
  std::vector<VertexBuffer *> buffers;

  void splitStringWhitespace(std::string& input, std::vector<std::string>& output);
  void splitString(std::string& input, char splitter, std::vector<std::string>& output);

public:
  VertexArray();
  VertexArray(std::string path);
  VertexArray(std::string path, std::shared_ptr<std::vector<glm::vec3>> _modelPositions);
  VertexArray(std::vector<glm::vec3> _positions, std::vector<glm::vec2> _texCoords);
  void setBuffer(std::string attribute, VertexBuffer *buffer);
  int getVertexCount();
  GLuint getId();

};
