#pragma once

#include "Car.h"
#include "Path.h"
#include "Street.h"
#include "Crossroad.h"

class Street;
class Crossroad;

class TrafficCar : public Car
{
public:
	enum Status
	{
		PAUSE,
		PLAYING,
		FINISH
	};

private:
	Path::Point				m_currentPoint;
	int						m_currentPointIndex;

	int						m_ticksToFinishRotate;
	int						m_cr;

	bool					m_autoStop;

	const Path				*m_pCurrentPath;

	Status					m_pathStatus;

	// Street
	const Street			*m_pCurrentStreet;
	Street::LaneType		m_laneType;

	// Crossroad
	const Crossroad			*m_pCurrentCrossroad;
	Crossroad::LanePos		m_lanePos;


	static const int MAX_ROTATE_SPEED = 1;

private:
	bool IsCollideWithPoint(const sf::Vector2f &pos);
	void CalcMove();

	void Init();

public:
	TrafficCar(const sf::Vector2f &pos);
	TrafficCar(float x, float y);
	~TrafficCar() = default;

	void Pulse();
	void Render(sf::RenderWindow *window);

	void SetPathToTarget(const Path *path, const Street *street, Street::LaneType laneType);
	void SetPathToTarget(const Path *path, const Crossroad *crossroad, Crossroad::LanePos lanePos);
	const Path *GetPath() const { return m_pCurrentPath; }

	auto GetStreetLaneType() const { return m_laneType; }
	auto GetCrossroadLanePos() const { return m_lanePos; }

	auto GetCurrentStreet() const { return m_pCurrentStreet; }
	auto GetCurrentCrossroad() const { return m_pCurrentCrossroad; }

	void Start(bool autoStop = true);
	void Pause() { m_pathStatus = PAUSE; }
	void Stop();

	Status GetStatus() const { return m_pathStatus; }
};

