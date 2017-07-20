#include "TrafficCar.h"

#include "Path.h"
#include "Common.h"
#include "Game.h"

#include <SFML\Graphics.hpp>


TrafficCar::TrafficCar(const sf::Vector2f &pos)
	: Car(pos.x, pos.y)
{
	Init();
}

TrafficCar::TrafficCar(float x, float y)
	: Car(x,y)
{
	Init();
}

void TrafficCar::Init()
{
	m_pCurrentPath = nullptr;
	m_currentPointIndex = 0;
	m_pathStatus = PAUSE;
	m_ticksToFinishRotate = 0;
	m_autoStop = true;

	m_pCurrentCrossroad = nullptr;
	m_pCurrentStreet = nullptr;
}

void TrafficCar::Pulse()
{
	if (m_pCurrentPath && m_pathStatus == PLAYING)
	{
		if (m_ticksToFinishRotate > 0)
		{
			if (m_cr > 0)
				m_wheel = MAX_ROTATE_SPEED;
			else if (m_cr < 0)
				m_wheel = -MAX_ROTATE_SPEED;

			m_ticksToFinishRotate--;
		}
		else
			m_wheel = 0.f;

		if (IsCollideWithPoint(m_currentPoint.pos))
		{
			m_currentPointIndex++;

			CalcMove();
			//printf("Current: %d Size: %d\n", m_currentPointIndex, m_pCurrentPath->GetSize());

			// end of path
			if (m_currentPointIndex == m_pCurrentPath->GetSize())
			{
				if (m_autoStop)
					Stop();
				
				m_pathStatus = FINISH;
			}
		}
	}
	else
	{
		m_enginePower = 0.f;
		m_brakePower = 1.f;
		m_wheel = 0.f;
	}

	Car::Pulse();
}

void TrafficCar::Render(sf::RenderWindow *window)
{
	Car::Render(window);

	auto carPosition = m_pBody->getPosition();

	if (Game::IsDebug())
	{	
		// direction line
		{
			sf::Vertex line[2];
			line[0] = sf::Vertex(carPosition, sf::Color::Blue);
			line[1] = sf::Vertex(carPosition + LenDir(200.f, -m_pBody->getRotation()), sf::Color::Blue);
			window->draw(&line[0], 2, sf::PrimitiveType::LineStrip);
		}

		// line to point
		if (m_pCurrentPath && m_pathStatus == PLAYING)
		{
			float directionToPoint = PointDir(carPosition, m_currentPoint.pos);
			float lengthToPoint = LengthToPoint(carPosition, m_currentPoint.pos);

			sf::Vertex line[2];
			line[0] = sf::Vertex(carPosition, sf::Color::Red);
			line[1] = sf::Vertex(carPosition + LenDir(lengthToPoint, directionToPoint), sf::Color::Red);

			window->draw(&line[0], 2, sf::PrimitiveType::LineStrip);
		}
	}
}

void TrafficCar::SetPathToTarget(const Path *path, const Street *street, Street::LaneType laneType)
{
	m_pCurrentCrossroad = nullptr;

	m_pCurrentPath = path;
	m_pCurrentStreet = street;

	m_laneType = laneType;
}

void TrafficCar::SetPathToTarget(const Path *path, const Crossroad *crossroad, Crossroad::LanePos lanePos)
{
	m_pCurrentStreet = nullptr;

	m_pCurrentPath = path;
	m_pCurrentCrossroad = crossroad;

	m_lanePos = lanePos;
}

void TrafficCar::Start(bool autoStop)
{
	if (m_pCurrentPath)
	{
		m_autoStop = autoStop;
		m_brakePower = 0.f;
		CalcMove();
		m_pathStatus = PLAYING;
	}
}

void TrafficCar::Stop()
{
	m_brakePower = 1.f;
	m_pathStatus = PAUSE;
	m_pCurrentPath = nullptr;
	m_currentPointIndex = 0;
}

bool TrafficCar::IsCollideWithPoint(const sf::Vector2f &pointPos)
{
	sf::Vector2f carPos = m_pBody->getPosition() - m_pBody->getOrigin();
	sf::Vector2f size = m_pBody->getSize();

	if (pointPos.x >= carPos.x &&
		pointPos.x <= carPos.x + size.x &&
		pointPos.y >= carPos.y &&
		pointPos.y <= carPos.y + size.y)
	{
		return true;
	}

	return false;
}

void TrafficCar::CalcMove()
{
	m_pCurrentPath->GetPoint(m_currentPointIndex, m_currentPoint);

	auto carPosition = m_pBody->getPosition();

	float directionToPoint = PointDir(carPosition, m_currentPoint.pos);
	float lengthToPoint = LengthToPoint(carPosition, m_currentPoint.pos);

	float bodyRotation = SFMLRotateToDegree(m_pBody->getRotation());

	if ((directionToPoint < 90.f || directionToPoint > 270.f) &&
		(bodyRotation < 90.f || bodyRotation > 270.f))
	{
		if (directionToPoint > 270.f)
			directionToPoint = -360.f + directionToPoint;
		
		if (bodyRotation > 270.f)
			bodyRotation = -360.f + bodyRotation;
	}

	m_cr = static_cast<int>(bodyRotation - directionToPoint);

	//printf("%f - %f = %f\n", bodyRotation, directionToPoint, m_cr);

	m_ticksToFinishRotate = static_cast<int>(ceil(abs(m_cr / MAX_ROTATE_SPEED)));

	float speedToFinish = 0.f;

	if (m_ticksToFinishRotate == 0)
		speedToFinish = m_globalMaxSpeed;
	else
		speedToFinish = lengthToPoint / m_ticksToFinishRotate;

	if (speedToFinish < m_globalMaxSpeed)
		m_maxSpeed = speedToFinish;

	m_enginePower = 1.f;
}
