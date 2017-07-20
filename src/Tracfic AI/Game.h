#pragma once

#include <vector>
#include <memory>
#include <utility>
#include <ctime>

#include <SFML/System/Vector2.hpp>

namespace sf
{
	class RenderWindow;
	class Event;
	class View;
	class Font;
	class Text;
}

class Camera;
class MapEditor;
class AIController;

class Game
{
private:
	std::shared_ptr<sf::RenderWindow>						m_pWindow;
	
	std::shared_ptr<Camera>									m_pCamera;
	std::shared_ptr<sf::View>								m_pHud;
	bool													m_drawHud;

	std::shared_ptr<sf::Font>								m_pFont;
	std::shared_ptr<sf::Text>								m_pText;
	sf::Vector2f											m_cursorPos;

	std::shared_ptr<MapEditor>								m_pMapEditor;
	std::shared_ptr<AIController>							m_pAIController;

	// fps counter
	size_t													m_startTime;
	unsigned int											m_frames;
	std::vector<std::pair<unsigned int, uint64_t>>			m_fps;
	static bool s_debug;

	static Game *s_pInst;

public:
	 Game();
	~Game() { }

	bool Init();

	void Run();

	void Pulse();
	void Render();
	void RenderHud();
	void Event(sf::Event *e);

	static Game *Get() { return s_pInst; }
	static bool IsDebug() { return s_debug; }
};


