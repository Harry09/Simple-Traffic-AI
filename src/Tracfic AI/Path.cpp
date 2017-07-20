#define _USE_MATH_DEFINES

#include "Path.h"

#include <SFML\Graphics.hpp>

#include <cmath>
#include <fstream>

#include "Common.h"

Path::Path(Type lineType)
	: m_lineType(lineType)
{
	m_lineColor = sf::Color::Black;

	m_pShapeDirOfPath = std::make_shared<sf::CircleShape>(5.f, 3);
	m_pShapeDirOfPath->setOrigin(m_pShapeDirOfPath->getRadius(), m_pShapeDirOfPath->getRadius());
	m_pShapeDirOfPath->setFillColor(m_lineColor);

	m_drawDirection = true;
}

void Path::AddPoint(const sf::Vector2f &point)
{
	for (int i = 0; i < m_points.size(); ++i)
	{
		if (m_points[i] == point)
			return;
	}

	m_points.push_back(point);

	m_line.clear();

	if (m_lineType == STRAIGHT_LINES)
	{
		for (int i = 0; i < m_points.size(); ++i)
		{
			m_line.push_back(m_points[i]);
		}
	}
	else if (m_lineType == SMOOTH_CURVE)
	{
		int max = static_cast<int>(m_points.size()) * 10;

		for (int i = 0; i <= max; ++i)
		{
			float perc = float(i) / max;

			m_line.push_back(CalcBezierCurve(m_points, perc).at(0));
		}
	}
}

void Path::Reset()
{
	m_points.clear();
	m_line.clear();
}

bool Path::GetPoint(int index, Point &point) const
{
	if (index < 0 || index >= m_line.size())
	{
		return false;
	}

	point.pos = m_line[index];

	if (index == m_line.size() - 1)
		point.directionToNextPoint = PointDir(m_line[index - 1], m_line[index]);
	else
		point.directionToNextPoint = PointDir(m_line[index], m_line[index + 1]);

	return true;
}

void Path::SetLine(const std::vector<sf::Vector2f> &line)
{
	m_line = line;
	m_points.clear();
}

void Path::SetColor(sf::Color color)
{
	m_lineColor = color;
	m_pShapeDirOfPath->setFillColor(color);
}

void Path::Render(sf::RenderWindow *window)
{
	std::vector<sf::Vertex>	line;

	for (int i = 0; i < m_line.size(); ++i)
	{
		line.push_back(sf::Vertex(m_line[i], m_lineColor));
	}

	window->draw(line.data(), line.size(), sf::PrimitiveType::LineStrip);

	if (m_drawDirection)
	{
		int nPoints = 5;

		int a = static_cast<int>(m_line.size()) / nPoints;

		for (int i = 0; i <= nPoints; ++i)
		{
			Point point;

			if (GetPoint(i * a, point))
			{
				m_pShapeDirOfPath->setPosition(point.pos);
				m_pShapeDirOfPath->setRotation(DegreeToSFMLRotate(point.directionToNextPoint) + 90.f);
				window->draw(*m_pShapeDirOfPath);
			}
		}
	}
}

void Path::Load(std::ifstream & stream)
{
	// line type
	stream.read(DataToBytes(m_lineType), sizeof(m_lineType));

	size_t nPoints = 0;

	// number of points
	stream.read(DataToBytes(nPoints), sizeof(size_t));

	for (int i = 0; i < nPoints; ++i)
	{
		sf::Vector2f point;

		// points
		stream.read(DataToBytes(point), sizeof(sf::Vector2f));

		m_points.push_back(point);
	}

	nPoints = 0;

	// number of points on line
	stream.read(DataToBytes(nPoints), sizeof(size_t));

	for (int i = 0; i < nPoints; ++i)
	{
		sf::Vector2f point;

		// points
		stream.read(DataToBytes(point), sizeof(sf::Vector2f));

		m_line.push_back(point);
	}

	// draw direction
	stream.read(DataToBytes(m_drawDirection), sizeof(m_drawDirection));

	// line color
	stream.read(DataToBytes(m_lineColor), sizeof(sf::Color));

	m_pShapeDirOfPath->setFillColor(m_lineColor);
}

void Path::Save(std::ofstream& stream)
{
	// line type
	stream.write(DataToBytes(m_lineType), sizeof(m_lineType));


	// number of points
	size_t nPoint = m_points.size();
	stream.write(DataToBytes(nPoint), sizeof(nPoint));

	// points
	if (nPoint)
		stream.write(DataToBytes(m_points[0]), nPoint * sizeof(sf::Vector2f));


	// number of points on line
	nPoint = m_line.size();
	stream.write(DataToBytes(nPoint), sizeof(nPoint));

	// points
	if (nPoint)
		stream.write(DataToBytes(m_line[0]), nPoint * sizeof(sf::Vector2f));


	// draw direction
	stream.write(DataToBytes(m_drawDirection), sizeof(m_drawDirection));

	// line color
	stream.write(DataToBytes(m_lineColor), sizeof(sf::Color));
}
