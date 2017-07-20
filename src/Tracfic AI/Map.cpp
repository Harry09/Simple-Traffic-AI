#include "Map.h"

#include "Street.h"
#include "Crossroad.h"

#include "Common.h"

#include <fstream>

void Map::Render(sf::RenderWindow *window)
{
	for (auto& street : m_streets)
	{
		if (street)
			street->Render(window);
	}

	for (auto& crossroad : m_crossroads)
	{
		if (crossroad)
			crossroad->Render(window);
	}
}

void Map::AddStreet(Street *street)
{
	m_streets.push_back(std::shared_ptr<Street>(street));
}

void Map::AddStreet(std::shared_ptr<Street>& street)
{
	m_streets.push_back(street);
}

void Map::AddCrossroad(Crossroad *crossroad)
{
	m_crossroads.push_back(std::shared_ptr<Crossroad>(crossroad));
}

void Map::AddCrossroad(std::shared_ptr<Crossroad>& crossroad)
{
	m_crossroads.push_back(crossroad);
}
