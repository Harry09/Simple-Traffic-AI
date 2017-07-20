#pragma once

#include <SFML/Graphics/View.hpp>

namespace sf
{
	class Event;
}

class Camera : public sf::View
{
private:
	enum HorizontalDirection
	{
		NONE_HOR = 0,
		LEFT,
		RIGHT,
	};

	enum VerticalDirection
	{
		NONE_VER = 0,
		TOP,
		BOTTOM
	};

private:
	HorizontalDirection		m_horizontalDir;
	VerticalDirection		m_verticalDir;

	float					m_scale;

	sf::Vector2f			m_baseViewPos;
	sf::Vector2f			m_baseViewSize;

	static const int CAMERA_MOVE_SPEED = 10;

	static Camera *s_pInst;

private:
	void Init();

public:
	Camera(const sf::View &view);	// copy
	explicit Camera(const sf::FloatRect &rectangle);
	Camera(const sf::Vector2f &center, const sf::Vector2f &size);

	~Camera() = default;

	void Pulse();
	void Event(const sf::Event *e);

	sf::Vector2f GetViewOffset();
	float GetScale() const { return m_scale; }

	static Camera *Get() { return s_pInst; }
};

