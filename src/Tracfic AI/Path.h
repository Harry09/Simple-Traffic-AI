#pragma once

#include <memory>
#include <vector>

#include <SFML\System\Vector2.hpp>
#include <SFML\Graphics\Color.hpp>

namespace sf
{
	class RenderWindow;
	class CircleShape;
}

class Street;
class Crossroad;

class Path
{
public:
	enum Type
	{
		STRAIGHT_LINES,
		SMOOTH_CURVE
	};

	class Point
	{
	public:
		sf::Vector2f		pos;
		float				directionToNextPoint;

	public:
		Point() : pos(), directionToNextPoint(0) { }
	};

private:
	Type								m_lineType;

	std::vector<sf::Vector2f>			m_points;

	std::vector<sf::Vector2f>			m_line;

	bool								m_drawDirection;
	std::shared_ptr<sf::CircleShape>	m_pShapeDirOfPath;

	sf::Color							m_lineColor;

private:
	friend Street;
	friend Crossroad;
	void Load(std::ifstream& stream);
	void Save(std::ofstream& stream);

public:
	Path(Type lineType = SMOOTH_CURVE);
	~Path() = default;

	void AddPoint(const sf::Vector2f &point);
	void Reset();

	bool GetPoint(int index, Point &point) const;
	int GetSize() const { return static_cast<int>(m_line.size()); }

	void SetLine(const std::vector<sf::Vector2f> &line);

	void SetColor(sf::Color color);

	void Render(sf::RenderWindow *window);

	void DrawDirection(bool draw) { m_drawDirection = draw; }
};

