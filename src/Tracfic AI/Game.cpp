#include "Game.h"

#include <SFML/Graphics.hpp>

#include "Camera.h"
#include "MapEditor.h"
#include "Common.h"
#include "AIController.h"

#include <Windows.h>
#include <string>
#include <sstream>

bool Game::s_debug;

Game *Game::s_pInst;

Game::Game()
{
	m_frames = 0;
	m_startTime = GetTickCount();
	m_drawHud = true;

	s_pInst = this;
}

bool Game::Init()
{
	m_pWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(1280, 720), "AI Traffic");
	
	if (!m_pWindow->isOpen())
		return false;

	m_pCamera = std::make_shared<Camera>(m_pWindow->getView());
	m_pHud = std::make_shared<sf::View>(m_pWindow->getView());

	m_pMapEditor = std::make_shared<MapEditor>();
	m_pAIController = std::make_shared<AIController>();
	m_pAIController->SetMap(m_pMapEditor->GetMap());

	m_pFont = std::make_shared<sf::Font>();
	if (!m_pFont->loadFromFile("arial.ttf"))
		return false;

	m_pText = std::make_shared<sf::Text>();
	m_pText->setFont(*m_pFont);
	m_pText->setString("FPS: 0");
	m_pText->setPosition(0.f, 0.f);
	m_pText->setFillColor(sf::Color::Black);
	m_pText->setCharacterSize(20);

	s_debug = true;

	return true;
}

void Game::Run()
{
	while (m_pWindow->isOpen())
	{
		sf::Event e;

		while (m_pWindow->pollEvent(e))
		{
			if (e.type == sf::Event::Closed ||
				e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape)
			{
				m_pWindow->close();
			}

			Event(&e);
		}

		Pulse();

		m_pWindow->clear(sf::Color::White);
		Render();
		if (m_drawHud)
			RenderHud();
		m_pWindow->display();

		if (GetTickCount() - m_startTime > 100)
		{
			std::stringstream strstream;

			if (m_fps.size() == 9)
				m_fps.erase(m_fps.begin());

			m_fps.push_back(std::make_pair(m_frames, GetTickCount()));

			unsigned int fps = 0;

			for (auto &_fps : m_fps)
			{
				if (GetTickCount() - _fps.second <= 1000)
					fps += _fps.first;
			}

			strstream << "FPS: ";
			strstream << fps;

			m_pText->setString(strstream.str());

			m_frames = 0;
			m_startTime = GetTickCount();
		}

		m_frames++;

		sf::sleep(sf::milliseconds(10));
	}
}

void Game::Pulse()
{
	if (m_pCamera)
		m_pCamera->Pulse();

	if (m_pMapEditor)
		m_pMapEditor->Pulse();

	if (m_pAIController)
		m_pAIController->Pulse();
}

void Game::Render()
{
	// set view
	if (m_pCamera)
		m_pWindow->setView(*m_pCamera);

	// draw stuff
	if (m_pMapEditor)
		m_pMapEditor->Render(m_pWindow.get());

	if (m_pAIController)
		m_pAIController->Render(m_pWindow.get());
}

void Game::RenderHud()
{
	// set view
	if (m_pHud)
		m_pWindow->setView(*m_pHud);

	// draw stuff
	if (m_pText)
		m_pWindow->draw(*m_pText);
}

void Game::Event(sf::Event *e)
{
	if (e->type == sf::Event::KeyPressed)
	{
		switch (e->key.code)
		{
			case sf::Keyboard::X:
				s_debug = !s_debug;
				break;
			case sf::Keyboard::F8:
				m_drawHud = !m_drawHud;
				break;
			case sf::Keyboard::Space:
			{
				while (true)
				{
					if (GetAsyncKeyState(VK_F5))
						break;
				}
			} break;
		}

	}

	if (m_pCamera)
		m_pCamera->Event(e);

	if (m_pMapEditor)
		m_pMapEditor->Event(e);

	if (m_pAIController)
		m_pAIController->Event(e);
}
