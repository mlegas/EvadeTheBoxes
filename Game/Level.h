#pragma once

#include <SFML/System.hpp>
#include <glm/ext.hpp>
#include <vector>
#include <memory>

class Camera;
class Floor;
class End;
class Diamond;

class Level
{
public:
	Level();
	~Level();
	void AddFloor(std::shared_ptr<Floor> _floor);
	void AddDiamond(std::shared_ptr<Diamond> _diamond);
	void SetEnd(std::shared_ptr<End> _end);
	std::vector<std::shared_ptr<Diamond>> GetDiamonds() { return m_diamonds; }
	std::vector<std::shared_ptr<Floor>> GetFloors() { return m_floors; }
	std::shared_ptr<End> GetEnd() { return m_end; }
	void Draw(std::shared_ptr<Camera> _camera);
	void Update(sf::Time &_deltaTime);
private:
	std::vector<std::shared_ptr<Diamond>> m_diamonds;
	std::vector<std::shared_ptr<Floor>> m_floors;
	std::shared_ptr<End> m_end;
};

