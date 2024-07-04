#pragma once
#include "MovementManager/SidePanel.h"

class GUI {
private:
	sf::Font font;
	const std::string font_path{ "assets/Fonts/ClearSans-Bold.ttf" };
	std::vector<sf::Text*> v_GUI_text;

	SidePanel* side_panel;
	const unsigned sp_header_font_size{ 32U }, sp_font_size{ 16U };
	const float sp_padding{ 20.f };
	const sf::Color sp_background_color{ sf::Color{ 50, 50, 50, 245 } };

	sf::RenderWindow* window;

	void initFont();
	void initText();
	void initSidePanel();
public:
	GUI() = delete;
	GUI(sf::RenderWindow* _window);
	~GUI();

	void update(float dt);
	void toggleSidePanel();

	void render();
};