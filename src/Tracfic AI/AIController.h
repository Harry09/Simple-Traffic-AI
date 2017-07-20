#pragma once

#include <vector>
#include <map>
#include <memory>
#include <utility>
#include <random>

namespace sf
{
	class RenderWindow;
	class Event;
}

class Map;
class TrafficCar;
class Street;
class Crossroad;
class Path;

class AIController
{
private:
	//class 

private:
	Map																		*m_pMap;

	std::vector<std::shared_ptr<TrafficCar>>								m_trafficCars;
	std::vector<std::pair<TrafficCar*, std::shared_ptr<Path>>>				m_carCrossroadPath;
	std::mt19937															m_gen;


public:
	AIController();
	~AIController();

	void SetMap(Map *map) { m_pMap = map; }

	void Pulse();
	void Render(sf::RenderWindow *window);
	void Event(sf::Event *e);
};

