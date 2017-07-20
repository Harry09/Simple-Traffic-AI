#include "Camera.h"

#include <SFML/Window/Event.hpp>

Camera *Camera::s_pInst;

Camera::Camera(const sf::View &view)
{
	setCenter(view.getCenter());
	setRotation(view.getRotation());
	setSize(view.getSize());
	setViewport(view.getViewport());

	Init();
}

Camera::Camera(const sf::FloatRect &rectangle)
	: sf::View(rectangle)
{
	Init();
}

Camera::Camera(const sf::Vector2f &center, const sf::Vector2f &size)
	: sf::View(center, size)
{
	Init();
}

void Camera::Init()
{
	s_pInst = this;

	m_horizontalDir = NONE_HOR;
	m_verticalDir = NONE_VER;

	m_baseViewPos = getCenter();
	m_baseViewSize = getSize();
	m_scale = getSize().x / m_baseViewSize.x;
}

void Camera::Pulse()
{
	sf::Vector2f pos = getCenter();

	if (m_horizontalDir == LEFT)
		pos.x -= CAMERA_MOVE_SPEED * m_scale;
	else if (m_horizontalDir == RIGHT)
		pos.x += CAMERA_MOVE_SPEED * m_scale;

	if (m_verticalDir == TOP)
		pos.y -= CAMERA_MOVE_SPEED * m_scale;
	else if (m_verticalDir == BOTTOM)
		pos.y += CAMERA_MOVE_SPEED * m_scale;

	setCenter(pos);
}

void Camera::Event(const sf::Event *e)
{
	if (e->type == sf::Event::KeyPressed)
	{
		switch (e->key.code)
		{
			case sf::Keyboard::A:
			m_horizontalDir = LEFT;
			break;
			case sf::Keyboard::D:
			m_horizontalDir = RIGHT;
			break;
			case sf::Keyboard::W:
			m_verticalDir = TOP;
			break;
			case sf::Keyboard::S:
			m_verticalDir = BOTTOM;
			break;
		}
	}
	else if (e->type == sf::Event::KeyReleased)
	{
		switch (e->key.code)
		{
			case sf::Keyboard::A:
			if (m_horizontalDir == LEFT)
				m_horizontalDir = NONE_HOR;
			break;
			case sf::Keyboard::D:
			if (m_horizontalDir == RIGHT)
				m_horizontalDir = NONE_HOR;
			break;
			case sf::Keyboard::W:
			if (m_verticalDir == TOP)
				m_verticalDir = NONE_VER;
			break;
			case sf::Keyboard::S:
			if (m_verticalDir == BOTTOM)
				m_verticalDir = NONE_VER;
			break;
		}
	}
	else if (e->type == sf::Event::MouseWheelMoved)
	{
		if (e->mouseWheel.delta > 0 && m_scale > 0.75f)
			zoom(0.9f);
		else if (e->mouseWheel.delta < 0 && m_scale < 4.f)
			zoom(1.1f);

		m_scale = getSize().x / m_baseViewSize.x;
	}
}

sf::Vector2f Camera::GetViewOffset()
{
	return m_baseViewPos - getCenter() + (getSize() - m_baseViewSize) / 2.f;
}
