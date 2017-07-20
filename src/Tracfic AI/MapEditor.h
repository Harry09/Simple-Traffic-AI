#pragma once

#include <SFML/System/Vector2.hpp>

#include "Crossroad.h"

#include <memory>
#include <utility>

namespace sf
{
	class RenderWindow;
	class Event;
	class Text;
	class Font;
}

class Map;

class Street;
class Crossroad;

class MapEditor
{
private:
	enum PutType
	{
		NONE,
		STREET,
		CROSSROAD
	};

private:
	std::shared_ptr<Map>				m_pMap;

	std::shared_ptr<sf::Font>			m_pFont;
	std::shared_ptr<sf::Text>			m_pText;

	PutType								m_putType;

	Street								*m_pCurrentStreet;

private:
	std::pair<Crossroad*, Crossroad::LanePos> GetPointingLane(const sf::Vector2f& cursorPosOnWindow);

public:
	MapEditor();
	~MapEditor() = default;

	void Pulse();
	void Render(sf::RenderWindow *window);
	void Event(sf::Event *e);

	Map *GetMap() { return m_pMap.get(); }
};

