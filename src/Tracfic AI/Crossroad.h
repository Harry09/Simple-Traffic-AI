#pragma once

#include <vector>
#include <map>
#include <memory>
#include <utility>

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Color.hpp>

namespace sf
{
	class RenderWindow;
	class RectangleShape;
}

class Path;
class Map;
class Street;

class Crossroad
{
public:
	enum LanePos
	{
		NONE = 0,
		LEFT,		// 1
		RIGHT,		// 2
		TOP,		// 3
		BOTTOM		// 4
	};

	struct Target
	{
		const Street		*street;
		int					laneType; // Street::LaneType
	};

private:
	sf::Vector2f												m_centerPos;

	std::shared_ptr<sf::RectangleShape>							m_pClickZone;

	std::map<LanePos, sf::Vector2f>								m_lane;
	std::map<LanePos, Target>									m_laneTarget;
	std::vector<std::vector<sf::Vector2f>>						m_curbs;

	LanePos														m_excludedLane;

	sf::Color													m_lineColor;

	static const int OFFSET = 50;

public:
	static const int ZONE_SIZE = 30;

private:
	bool IsKeyInMap(LanePos key) const;
	void CalcLanePoses();
	const std::vector<sf::Vector2f> CalcCurveCurb(const sf::Vector2f &horizontalPos, const sf::Vector2f &verticalPos);
	const std::vector<sf::Vector2f> CalcSimpleCurb(const sf::Vector2f &pos1, const sf::Vector2f &pos2, bool positive);

public:
	Crossroad();
	Crossroad(const sf::Vector2f &pos, LanePos without = NONE);
	Crossroad(Crossroad *crossroad);
	~Crossroad() = default;

	void Render(sf::RenderWindow *window);

	const sf::Vector2f& GetLanePos(LanePos lanePos) const { return m_lane.at(lanePos); }
	void SetTargetToLane(LanePos lanePos, const Street *street, int laneType);
	const Target *GetTargetToLane(LanePos lanePos) const;

	const sf::Vector2f& GetCenterPos() const { return m_centerPos; }

	void DrawLine(const std::vector<sf::Vector2f>& line, sf::RenderWindow *window);
};

