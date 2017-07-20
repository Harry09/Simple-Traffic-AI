#include "AIController.h"

#include <SFML\Window\Event.hpp>
#include <SFML\System\Sleep.hpp>

#include "TrafficCar.h"
#include "Street.h"
#include "Crossroad.h"
#include "Map.h"
#include "Path.h"
#include "Common.h"
#include "Game.h"

#include <random>
#include <Windows.h>

AIController::AIController()
{
	m_gen = std::mt19937{ std::random_device{}() };
}

AIController::~AIController()
{
}

void AIController::Pulse()
{
	for (int i = 0; i < m_trafficCars.size(); ++i)
	{
		auto car = m_trafficCars.at(i);

		if (car)
		{
			car->Pulse();

			if (car->GetStatus() == TrafficCar::FINISH)
			{
				if (car->GetCurrentStreet()) // if street ends
				{
					auto street = car->GetCurrentStreet();

					while (true)
					{
						Crossroad::LanePos lanePos = static_cast<Crossroad::LanePos>(std::uniform_int_distribution<size_t>{ 1, 4 }(m_gen));

						auto streetTarget = street->GetTarget(car->GetStreetLaneType());
						Street::LaneType currentLanePos = static_cast<Street::LaneType>(streetTarget->lanePos);

						if (streetTarget->lanePos == lanePos)
						{
							continue;
						}

						if (streetTarget->crossroad)
						{
							auto path = std::make_shared<Path>();
							path->SetColor(sf::Color::Black);

							auto crossroadTarget = streetTarget->crossroad->GetTargetToLane(lanePos);

							Path::Point startPoint, endPoint;

							// get last point of current street
							auto currentStreetLanePath = street->GetLanePath(car->GetStreetLaneType());
							currentStreetLanePath->GetPoint(currentStreetLanePath->GetSize() - 1, startPoint);

							// get first point target street
							auto targetStreetLanePath = crossroadTarget->street->GetLanePath(static_cast<Street::LaneType>(crossroadTarget->laneType));
							targetStreetLanePath->GetPoint(0, endPoint);

							path->AddPoint(startPoint.pos);

							if (currentLanePos == Crossroad::LEFT || currentLanePos == Crossroad::RIGHT)
								path->AddPoint(sf::Vector2f(endPoint.pos.x, startPoint.pos.y));
							else if (currentLanePos == Crossroad::TOP || currentLanePos == Crossroad::BOTTOM)
								path->AddPoint(sf::Vector2f(startPoint.pos.x, endPoint.pos.y));

							path->AddPoint(endPoint.pos);

							m_carCrossroadPath.push_back(std::make_pair(car.get(), path));

							car->SetPathToTarget(path.get(), streetTarget->crossroad, lanePos);
							car->Start();

							break;
						}

						sf::sleep(sf::milliseconds(10));
					}
				}
				else if (car->GetCurrentCrossroad()) // if crossroad ends
				{
					auto crossroad = car->GetCurrentCrossroad();

					auto lanePos = car->GetCrossroadLanePos();

					auto crossroadTarget = crossroad->GetTargetToLane(lanePos);

					for (int i = 0; i < m_carCrossroadPath.size(); ++i)
					{
						if (m_carCrossroadPath[i].first == car.get())
						{
							m_carCrossroadPath.erase(m_carCrossroadPath.begin() + i);
							break;
						}
					}

					if (crossroadTarget->street)
					{
						Street::LaneType laneType = static_cast<Street::LaneType>(crossroadTarget->laneType);
						car->SetPathToTarget(crossroadTarget->street->GetLanePath(laneType), crossroadTarget->street, laneType);
						car->Start();
					}
					else
					{
						car->Stop();
					}
				}
			}
		}
	}
}

void AIController::Render(sf::RenderWindow *window)
{
	for (auto& car : m_trafficCars)
	{
		if (car)
		{
			car->Render(window);
		}
	}

	if (Game::IsDebug())
	{
		for (auto &path : m_carCrossroadPath)
		{
			if (path.second)
			{
				path.second->Render(window);
			}
		}
	}
}

void AIController::Event(sf::Event *e)
{
	if (e->type == sf::Event::KeyPressed)
	{
		switch (e->key.code)
		{
			case sf::Keyboard::R:
			{
				auto streets = m_pMap->GetStreets();
				auto street = streets->at(std::uniform_int_distribution<size_t>{0, streets->size() - 1}(m_gen));

				Street::LaneType laneType = static_cast<Street::LaneType>(std::uniform_int_distribution<int>{ 0, 1 }(m_gen));
				
				auto path = street->GetLanePath(laneType);

				Path::Point point;

				if (path->GetSize() > 0)
				{
					path->GetPoint(0, point);

					TrafficCar *trafficCar = new TrafficCar(point.pos + LenDir(-50, point.directionToNextPoint));
					trafficCar->SetDirection(-point.directionToNextPoint);
					trafficCar->SetPathToTarget(path, street.get(), laneType);
					trafficCar->Start();
					//trafficCar->SetMaxSpeed(1.f);

					m_trafficCars.push_back(std::shared_ptr<TrafficCar>(trafficCar));

					printf("Created car on pos: (%f, %f)\n", point.pos.x, point.pos.y);
				}
			} break;
		}
	}

}
