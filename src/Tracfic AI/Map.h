#pragma once

#include <vector>
#include <memory>

class Street;
class Crossroad;

namespace sf
{
	class RenderWindow;
}

class Map
{
private:
	std::vector<std::shared_ptr<Street>>		m_streets;
	std::vector<std::shared_ptr<Crossroad>>		m_crossroads;

public:
	Map() = default;
	~Map() = default;

	void Render(sf::RenderWindow *window);

	void AddStreet(Street *street);
	void AddStreet(std::shared_ptr<Street> &street);
	const auto GetStreets() const { return &m_streets; }

	void AddCrossroad(Crossroad *crossroad);
	void AddCrossroad(std::shared_ptr<Crossroad> &crossroad);
	const auto GetCrossroads() const { return &m_crossroads; }
};

