#include "Street.h"

#include "Game.h"
#include "Common.h"
#include "Path.h"

#include <SFML\Graphics.hpp>

#include <list>
#include <fstream>

Street::Street()
{
	m_lineColor = sf::Color::Black;

	m_pLeftLanePath = std::make_shared<Path>();
	m_pRightLanePath = std::make_shared<Path>();
}

Street::Street(Street *street)
{
	m_pLeftLanePath = street->m_pLeftLanePath;
	m_pRightLanePath = street->m_pRightLanePath;

	m_centerLinePoints = street->m_centerLinePoints;

	m_leftLine = street->m_leftLine;
	m_centerLine = street->m_centerLine;
	m_rightLine = street->m_rightLine;

	m_lineColor = street->m_lineColor;
}

void Street::Reset()
{
	m_centerLinePoints.clear();

	m_centerLine.clear();
	m_leftLine.clear();
	m_rightLine.clear();

	m_pLeftLanePath->Reset();
	m_pRightLanePath->Reset();
}

void Street::AddPoint(const sf::Vector2f &point, float firstPointDir, float lastPointDir)
{
	m_centerLinePoints.push_back(point);

	int linePrec = 6;

	m_centerLine = CalcLine(m_centerLinePoints, linePrec);

	m_leftLine.clear();
	m_rightLine.clear();

	m_pLeftLanePath->Reset();
	m_pRightLanePath->Reset();

	std::list<sf::Vector2f> leftLaneList;
	std::vector<sf::Vector2f> rightLane;

	for (int i = 0; i < m_centerLine.size(); ++i)
	{
		float dirToNextPoint = 0.f;

		if (firstPointDir >= 0.f && firstPointDir <= 360.f && i == 0) // first point
			dirToNextPoint = firstPointDir;
		if (lastPointDir >= 0.f && lastPointDir <= 360.f && i == m_centerLine.size() - 1) // last point
			dirToNextPoint = lastPointDir;
		else
		{
			if (i == m_centerLine.size() - 1)
				dirToNextPoint = PointDir(m_centerLine[i - 1], m_centerLine[i]);
			else
				dirToNextPoint = PointDir(m_centerLine[i], m_centerLine[i + 1]);
		}

		m_leftLine.push_back(sf::Vector2f(m_centerLine[i] + LenDir(static_cast<float>(LANE_WIDTH), dirToNextPoint + 90)));
		m_rightLine.push_back(sf::Vector2f(m_centerLine[i] + LenDir(static_cast<float>(LANE_WIDTH), dirToNextPoint - 90)));

		leftLaneList.push_front(sf::Vector2f(m_centerLine[i] + LenDir(static_cast<float>(LANE_WIDTH) / 2, dirToNextPoint + 90)));
		rightLane.push_back(sf::Vector2f(m_centerLine[i] + LenDir(static_cast<float>(LANE_WIDTH) / 2, dirToNextPoint - 90)));
	}

	std::vector<sf::Vector2f> leftLaneVector{ std::make_move_iterator(std::begin(leftLaneList)),
		std::make_move_iterator(std::end(leftLaneList)) };

	m_pLeftLanePath->SetLine(leftLaneVector);
	m_pRightLanePath->SetLine(rightLane);
}

void Street::Render(sf::RenderWindow *window)
{
	DrawLine(m_centerLine, window);
	DrawLine(m_leftLine, window);
	DrawLine(m_rightLine, window);

	if (Game::IsDebug())
	{
		if (m_pLeftLanePath)
			m_pLeftLanePath->Render(window);

		if (m_pRightLanePath)
			m_pRightLanePath->Render(window);
	}
}

void Street::SetTarget(LaneType laneType, const Crossroad *crossroad, int lanePos)
{
	if (laneType == LEFT)
		m_leftLaneTarget = { crossroad, lanePos };
	else if (laneType == RIGHT)
		m_rightLaneTarget = { crossroad, lanePos };
}

const Street::Target *Street::GetTarget(LaneType laneType) const
{
	if (laneType == LEFT)
		return &m_leftLaneTarget;
	else if (laneType == RIGHT)
		return &m_rightLaneTarget;

	return nullptr;
}

const Path *Street::GetLanePath(LaneType laneType) const
{
	if (laneType == LEFT)
		return m_pLeftLanePath.get();
	else if (laneType == RIGHT)
		return m_pRightLanePath.get();

	return nullptr;
}


std::vector<sf::Vector2f> Street::CalcLine(const std::vector<sf::Vector2f> &points, int precision)
{
	std::vector<sf::Vector2f> line;

	int max = static_cast<int>(points.size()) * precision;

	for (int i = 0; i <= max; ++i)
	{
		float perc = float(i) / max;

		line.push_back(CalcBezierCurve(points, perc).at(0));
	}

	return line;
}

void Street::DrawLine(const std::vector<sf::Vector2f>& line, sf::RenderWindow *window)
{
	std::vector<sf::Vertex>	vertexLine;

	for (int i = 0; i < line.size(); ++i)
	{
		vertexLine.push_back(sf::Vertex(line[i], m_lineColor));
	}

	window->draw(vertexLine.data(), vertexLine.size(), sf::PrimitiveType::LineStrip);
}
