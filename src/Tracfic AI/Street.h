#pragma once

#include <memory>
#include <vector>
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
class Crossroad;


class Street
{
public:
	enum LaneType
	{
		LEFT = 0,
		RIGHT
	};

	struct Target
	{
		const Crossroad		*crossroad;
		int					lanePos; // src Crossroad::LanePos
	};

private:
	std::shared_ptr<Path>									m_pLeftLanePath;
	std::shared_ptr<Path>									m_pRightLanePath;

	Target													m_leftLaneTarget;
	Target													m_rightLaneTarget;

	std::vector<sf::Vector2f>								m_centerLinePoints;

	std::vector<sf::Vector2f>								m_leftLine;
	std::vector<sf::Vector2f>								m_centerLine;
	std::vector<sf::Vector2f>								m_rightLine;

	sf::Color												m_lineColor;

public:
	static const int LANE_WIDTH = 70;

private:
	std::vector<sf::Vector2f> CalcLine(const std::vector<sf::Vector2f> &points, int precision);
	void DrawLine(const std::vector<sf::Vector2f> &line, sf::RenderWindow *window);

public:
	Street();
	Street(Street *street);
	~Street() = default;

	void Reset();

	bool IsEmpty() const { return m_centerLinePoints.empty(); }

	// @direction = <0,360>
	// if direction == -1.f than it is ignored
	void AddPoint(const sf::Vector2f &point, float firstPointDir = -1.f, float lastPointDir = -1.f);

	void Render(sf::RenderWindow *window);

	const std::vector<sf::Vector2f> *GetCenterLine() const { return &m_centerLine; }

	void SetTarget(LaneType laneType, const Crossroad *crossroad, int lanePos);
	const Target *GetTarget(LaneType laneType) const;

	const Path *GetLanePath(LaneType laneType) const;
};
