#include "MapEditor.h"

#include "Map.h"

#include "Street.h"
#include "Crossroad.h"
#include "Common.h"
#include "Camera.h"
#include "Path.h"

#include <SFML\Graphics.hpp>

#include <iostream>

MapEditor::MapEditor()
{ 
	m_pMap = std::make_shared<Map>();

	m_pCurrentStreet = nullptr;

	m_putType = NONE;
}

void MapEditor::Pulse()
{

}

void MapEditor::Render(sf::RenderWindow *window)
{
	if (m_pMap)
		m_pMap->Render(window);
}

void MapEditor::Event(sf::Event *e)
{
	if (e->type == sf::Event::KeyPressed)
	{
		switch (e->key.code)
		{
			case sf::Keyboard::Num1:
				printf("Selected STREET\n");
				m_putType = STREET;
				break;
			case sf::Keyboard::Num2:
				printf("Selected CROSSROAD\n");
				m_putType = CROSSROAD;
				break;
			case sf::Keyboard::Num3:
				m_pCurrentStreet = nullptr;
				printf("Selected NONE\n");
				m_putType = NONE;
				break;
		}
	}
	else if (e->type == sf::Event::MouseButtonPressed)
	{
		if (m_putType == STREET)
		{
			if (!m_pCurrentStreet)
			{
				m_pCurrentStreet = new Street();
				m_pMap->AddStreet(m_pCurrentStreet);
			}

			auto result = GetPointingLane(sf::Vector2f(float(e->mouseButton.x), float(e->mouseButton.y)));

			if (result.first)
			{
				bool empty = m_pCurrentStreet->IsEmpty();

				auto pos = result.first->GetLanePos(result.second);

				if (empty) // first point
				{
					float dir = PointDir(result.first->GetCenterPos(), pos);
					m_pCurrentStreet->AddPoint(pos, dir);

					// add to crossroad next path
					result.first->SetTargetToLane(result.second, m_pCurrentStreet, Street::RIGHT);

					// set target for street
					m_pCurrentStreet->SetTarget(Street::LEFT, result.first, result.second);
				}
				else // last point
				{
					float dir = PointDir(pos, result.first->GetCenterPos());
					m_pCurrentStreet->AddPoint(pos, -1.f, dir);

					// add to crossroad next path
					result.first->SetTargetToLane(result.second, m_pCurrentStreet, Street::LEFT);

					// set target for street
					m_pCurrentStreet->SetTarget(Street::RIGHT, result.first, result.second);

					m_pCurrentStreet = nullptr;
				}
			}
			else
			{
				if (!m_pCurrentStreet->IsEmpty())
				{
					sf::Vector2f cursorPos = sf::Vector2f(float(e->mouseButton.x), float(e->mouseButton.y)) * Camera::Get()->GetScale() - Camera::Get()->GetViewOffset();

					m_pCurrentStreet->AddPoint(cursorPos);
				}
				else
				{
					printf("First point must start with crossroad!\n");
				}
			}
		}
		else if (m_putType == CROSSROAD)
		{
			sf::Vector2f cursorPos = sf::Vector2f(float(e->mouseButton.x), float(e->mouseButton.y)) * Camera::Get()->GetScale() - Camera::Get()->GetViewOffset();

			m_pMap->AddCrossroad(new Crossroad(cursorPos));
		}
		else
		{
			printf("First select type road!\n");
		}
	}
}

std::pair<Crossroad*, Crossroad::LanePos> MapEditor::GetPointingLane(const sf::Vector2f &cursorPosOnWindow)
{
	for (auto& crossroad : *m_pMap->GetCrossroads())
	{
		auto leftLane = crossroad->GetLanePos(Crossroad::LEFT);
		auto rightLane = crossroad->GetLanePos(Crossroad::RIGHT);
		auto topLane = crossroad->GetLanePos(Crossroad::TOP);
		auto bottomLane = crossroad->GetLanePos(Crossroad::BOTTOM);

		sf::Vector2f cursorPos = cursorPosOnWindow * Camera::Get()->GetScale() - Camera::Get()->GetViewOffset();

		Crossroad::LanePos lanePos = Crossroad::NONE;

		bool isIn = true;


		// Left
		if (cursorPos.x > leftLane.x - Crossroad::ZONE_SIZE &&
			cursorPos.x < leftLane.x + Crossroad::ZONE_SIZE &&
			cursorPos.y > leftLane.y - Street::LANE_WIDTH &&
			cursorPos.y < leftLane.y + Street::LANE_WIDTH)
		{
			lanePos = Crossroad::LEFT;
		}

		// Right
		else if (cursorPos.x > rightLane.x - Crossroad::ZONE_SIZE &&
			cursorPos.x < rightLane.x + Crossroad::ZONE_SIZE &&
			cursorPos.y > rightLane.y - Street::LANE_WIDTH &&
			cursorPos.y < rightLane.y + Street::LANE_WIDTH)
		{
			lanePos = Crossroad::RIGHT;
		}

		// Top
		else if (cursorPos.x > topLane.x - Street::LANE_WIDTH  &&
			cursorPos.x < topLane.x + Street::LANE_WIDTH  &&
			cursorPos.y > topLane.y - Crossroad::ZONE_SIZE &&
			cursorPos.y < topLane.y + Crossroad::ZONE_SIZE)
		{
			lanePos = Crossroad::TOP;
		}

		// Bottom
		else if (cursorPos.x > bottomLane.x - Street::LANE_WIDTH  &&
			cursorPos.x < bottomLane.x + Street::LANE_WIDTH  &&
			cursorPos.y > bottomLane.y - Crossroad::ZONE_SIZE &&
			cursorPos.y < bottomLane.y + Crossroad::ZONE_SIZE)
		{
			lanePos = Crossroad::BOTTOM;
		}
		else
		{
			isIn = false;
		}

		if (isIn)
			return std::make_pair(crossroad.get(), lanePos);
		else
			continue;
	}

	return std::make_pair(nullptr, Crossroad::NONE);
}
