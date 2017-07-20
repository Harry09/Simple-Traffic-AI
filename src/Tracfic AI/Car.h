#pragma once

#include <memory>

#include <SFML\System\Vector2.hpp>

namespace sf
{
	class RenderWindow;
	class RectangleShape;
}

class Car
{
protected:
	std::shared_ptr<sf::RectangleShape>		m_pBody;

	float									m_wheel; // <-1,1>
	float									m_brakePower; // <0,1>
	float									m_enginePower; // <0,1>

	float									m_acceleration;
	float									m_friction;

	float									m_globalMaxSpeed;
	float									m_maxSpeed;

	float									m_speed;
	float									m_hSpeed;
	float									m_vSpeed;

public:
	static const int						BODY_LENGTH = 60;
	static const int						BODY_WIDTH = 30;

public:
	Car(float x, float y);
	~Car();

	void  Pulse();
	void  Render(sf::RenderWindow *window);

	// @power <0,1>
	void  SetEnginePower(float power)		{ m_enginePower = power; }
	float GetEnginePower()		const		{ return m_enginePower; }

	// @power <0,1>
	void  SetBrakePower(float power)		{ m_brakePower = power; }
	float GetBrakePower()		const		{ return m_brakePower; }

	void  SetWheel(float value)				{ m_wheel = value; }
	float GetWheel()			const		{ return m_wheel; }

	float GetSpeed()			const		{ return m_speed; }

	void  SetMaxSpeed(float maxSpeed)		{ m_globalMaxSpeed = maxSpeed; m_maxSpeed = m_globalMaxSpeed = maxSpeed; }
	float GetMaxSpeed()			const		{ return m_globalMaxSpeed; }


	void SetDirection(float deg);
	float GetDirection();

	void SetPosition(float x, float y);
	sf::Vector2f GetPosition();
};

