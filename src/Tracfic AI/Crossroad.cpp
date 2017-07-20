#include "Crossroad.h"

#include "Game.h"
#include "Common.h"
#include "Path.h"
#include "Street.h"

#include <SFML\Graphics.hpp> 

#include <fstream>

Crossroad::Crossroad()
{
	m_excludedLane = NONE;
	m_lineColor = sf::Color::Black;
	m_centerPos = sf::Vector2f(0.f, 0.f);

	m_pClickZone = std::make_shared<sf::RectangleShape>(sf::Vector2f(ZONE_SIZE * 2, Street::LANE_WIDTH * 2));
	m_pClickZone->setOrigin(float(ZONE_SIZE), float(Street::LANE_WIDTH));
	m_pClickZone->setFillColor(sf::Color(0, 0, 0, 50));
}

Crossroad::Crossroad(const sf::Vector2f &pos, LanePos without)
	: m_centerPos(pos)
{
	m_excludedLane = without;

	m_lineColor = sf::Color::Black;

	printf("Pos: (%f, %f)\n", pos.x, pos.y);

	bool leftLane = (without != LEFT);
	bool rightLane = (without != RIGHT);
	bool topLane = (without != TOP);
	bool bottomLane = (without != BOTTOM);

	CalcLanePoses();

	if (!leftLane)
		m_curbs.push_back(CalcSimpleCurb(m_lane[TOP], m_lane[BOTTOM], false));
	else if (!topLane)
		m_curbs.push_back(CalcSimpleCurb(m_lane[LEFT], m_lane[RIGHT], false));
	else if (!rightLane)
		m_curbs.push_back(CalcSimpleCurb(m_lane[TOP], m_lane[BOTTOM], true));
	else if (!bottomLane)
		m_curbs.push_back(CalcSimpleCurb(m_lane[LEFT], m_lane[RIGHT], true));

	// LEFT TOP
	if (leftLane && topLane)
		m_curbs.push_back(CalcCurveCurb(m_lane[LEFT], m_lane[TOP]));

	// RIGHT TOP
	if (topLane && rightLane)
		m_curbs.push_back(CalcCurveCurb(m_lane[RIGHT], m_lane[TOP]));

	// RIGHT BOTTOM
	if (rightLane && bottomLane)
		m_curbs.push_back(CalcCurveCurb(m_lane[RIGHT], m_lane[BOTTOM]));

	// LEFT BOTTOM
	if (leftLane && bottomLane)
		m_curbs.push_back(CalcCurveCurb(m_lane[LEFT], m_lane[BOTTOM]));

	m_pClickZone = std::make_shared<sf::RectangleShape>(sf::Vector2f(ZONE_SIZE * 2, Street::LANE_WIDTH * 2));
	m_pClickZone->setOrigin(float(ZONE_SIZE), float(Street::LANE_WIDTH));
	m_pClickZone->setFillColor(sf::Color(0,0,0, 50));
}

Crossroad::Crossroad(Crossroad *crossroad)
{
	m_centerPos = crossroad->m_centerPos;

	m_lane = crossroad->m_lane;
	m_curbs = crossroad->m_curbs;

	m_excludedLane = crossroad->m_excludedLane;

	m_lineColor = crossroad->m_lineColor;
}

void Crossroad::Render(sf::RenderWindow *window)
{
	for (auto &line : m_curbs)
	{
		DrawLine(line, window);
	}

	if (Game::IsDebug())
	{
		if (IsKeyInMap(LEFT))
		{
			m_pClickZone->setPosition(m_lane[LEFT]);
			window->draw(*m_pClickZone);
		}

		if (IsKeyInMap(RIGHT))
		{
			m_pClickZone->setPosition(m_lane[RIGHT]);
			window->draw(*m_pClickZone);
		}

		if (IsKeyInMap(TOP))
		{
			m_pClickZone->setPosition(m_lane[TOP]);
			m_pClickZone->setRotation(90);
			window->draw(*m_pClickZone);
			m_pClickZone->setRotation(0);
		}

		if (IsKeyInMap(BOTTOM))
		{
			m_pClickZone->setPosition(m_lane[BOTTOM]);
			m_pClickZone->setRotation(90);
			window->draw(*m_pClickZone);
			m_pClickZone->setRotation(0);
		}
	}
}

void Crossroad::SetTargetToLane(LanePos lanePos, const Street *street, int laneType)
{
	m_laneTarget[lanePos] = Crossroad::Target{ street, laneType };
}

const Crossroad::Target *Crossroad::GetTargetToLane(LanePos lanePos) const
{
	if (IsKeyInMap(lanePos))
		return &m_laneTarget.at(lanePos);
	else
		return nullptr;
}

void Crossroad::DrawLine(const std::vector<sf::Vector2f>& line, sf::RenderWindow *window)
{
	std::vector<sf::Vertex>	vertexLine;

	for (int i = 0; i < line.size(); ++i)
	{
		vertexLine.push_back(sf::Vertex(line[i], m_lineColor));
	}

	window->draw(vertexLine.data(), vertexLine.size(), sf::PrimitiveType::LineStrip);
}

bool Crossroad::IsKeyInMap(LanePos key) const
{
	if (m_lane.find(key) == m_lane.end())
		return false;

	return true;
}

void Crossroad::CalcLanePoses()
{
	m_lane[LEFT] = m_centerPos - sf::Vector2f(static_cast<float>(Street::LANE_WIDTH + OFFSET), 0.f);
	m_lane[RIGHT] = m_centerPos + sf::Vector2f(static_cast<float>(Street::LANE_WIDTH + OFFSET), 0.f);
	m_lane[TOP] = m_centerPos - sf::Vector2f(0.f, static_cast<float>(Street::LANE_WIDTH + OFFSET));
	m_lane[BOTTOM] = m_centerPos + sf::Vector2f(0.f, static_cast<float>(Street::LANE_WIDTH + OFFSET));
}

const std::vector<sf::Vector2f> Crossroad::CalcCurveCurb(const sf::Vector2f &horizontalPos, const sf::Vector2f &verticalPos)
{
	std::vector<sf::Vector2f> points;

	if (verticalPos.y > m_centerPos.y)
		points.push_back(horizontalPos + sf::Vector2f(0.f, float(Street::LANE_WIDTH)));
	else if (verticalPos.y < m_centerPos.y)
		points.push_back(horizontalPos + sf::Vector2f(0.f, -float(Street::LANE_WIDTH)));

	if (points.size() == 0)
		return std::vector<sf::Vector2f>();

	if (horizontalPos.x < m_centerPos.x)
	{
		points.push_back(points[0] + sf::Vector2f(float(OFFSET), 0.f));
		points.push_back(verticalPos + sf::Vector2f(-float(Street::LANE_WIDTH), 0.f));
	}
	else if (horizontalPos.x > m_centerPos.x)
	{
		points.push_back(points[0] - sf::Vector2f(float(OFFSET), 0.f));
		points.push_back(verticalPos + sf::Vector2f(float(Street::LANE_WIDTH), 0.f));
	}

	std::vector<sf::Vector2f> line;

	int max = static_cast<int>(points.size()) * 3;

	for (int i = 0; i <= max; ++i)
	{
		float perc = float(i) / max;

		line.push_back(CalcBezierCurve(points, perc).at(0));
	}

	return line;
}

const std::vector<sf::Vector2f> Crossroad::CalcSimpleCurb(const sf::Vector2f &pos1, const sf::Vector2f &pos2, bool positive)
{
	std::vector<sf::Vector2f> curb;

	float laneWidth = static_cast<float>(Street::LANE_WIDTH);

	if (!positive)
		laneWidth = -laneWidth;

	if (pos1.x < m_centerPos.x && pos2.x > m_centerPos.x)
	{
		curb.push_back(pos1 + sf::Vector2f(0.f, laneWidth));
		curb.push_back(pos2 + sf::Vector2f(0.f, laneWidth));
	}
	else if (pos1.y < m_centerPos.x && pos2.y > m_centerPos.y)
	{
		curb.push_back(pos1 + sf::Vector2f(laneWidth, 0.f));
		curb.push_back(pos2 + sf::Vector2f(laneWidth, 0.f));
	}

	return curb;
}
