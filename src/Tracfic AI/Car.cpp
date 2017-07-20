#define _USE_MATH_DEFINES

#include "Car.h"

#include <SFML\Graphics.hpp>

#include <cmath>

Car::Car(float x, float y)
{
	m_pBody = std::make_shared<sf::RectangleShape>(sf::Vector2f(float(BODY_LENGTH), float(BODY_WIDTH)));
	m_pBody->setFillColor(sf::Color::Black);
	m_pBody->setOrigin(BODY_LENGTH / 3.f, BODY_WIDTH / 2.f);
	m_pBody->setPosition(x, y);

	m_wheel = 0.f;
	m_brakePower = 0.f;
	m_enginePower = 0.f;

	m_globalMaxSpeed = 3.f;
	m_maxSpeed = 3.f;
	m_acceleration = 0.1f;
	m_friction = 0.01f;

	m_speed = 0.f;
	m_hSpeed = 0.f;
	m_vSpeed = 0.f;
}

Car::~Car()
{
}

void Car::Pulse()
{
	if (m_enginePower > 0.f)
	{
		if (m_speed < m_maxSpeed)
			m_speed += m_acceleration * m_enginePower;
		else if (m_speed > m_maxSpeed)
			m_speed -= 0.2f;
	}
	else
	{
		if (m_speed - m_friction > 0.f)
			m_speed -= m_friction;
		else if (m_speed - m_friction < 0.f)
			m_speed = 0.f;
	}
	
	if (m_speed > 0.f)
		m_speed -= m_brakePower / 15;
	else if (m_speed < 0.f)
		m_speed = 0.f;

	float direction = m_pBody->getRotation();

	direction += m_wheel;

	m_pBody->setRotation(direction);

	m_hSpeed = static_cast<float>(cos(direction * M_PI / 180)) * m_speed;
	m_vSpeed = static_cast<float>(sin(direction * M_PI / 180)) * m_speed;

	auto pos = m_pBody->getPosition();
	pos += sf::Vector2f(m_hSpeed, m_vSpeed);
	m_pBody->setPosition(pos);
}

void Car::Render(sf::RenderWindow *window)
{
	window->draw(*m_pBody);

	// draw center point of car
	sf::Vertex point(m_pBody->getPosition(), sf::Color::White);
	window->draw(&point, 1, sf::PrimitiveType::Points);
}

void Car::SetDirection(float deg)
{
	m_pBody->setRotation(deg);
}

float Car::GetDirection()
{
	return m_pBody->getRotation();
}

void Car::SetPosition(float x, float y)
{
	m_pBody->setPosition(x, y);
}

sf::Vector2f Car::GetPosition()
{
	return m_pBody->getPosition();
}
