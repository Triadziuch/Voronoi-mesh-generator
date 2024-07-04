#include "GUI.h"

void GUI::initFont()
{
	font.loadFromFile(font_path);
}

void GUI::initText()
{
    sf::Text* controls_text = new sf::Text{ "[C] - Controls", font, 20u };
    controls_text->setFillColor(sf::Color::Black);
    controls_text->setPosition(10.f, 10.f);
    v_GUI_text.push_back(controls_text);
}

void GUI::initSidePanel()
{
    side_panel = new SidePanel{ *window, font_path, sp_header_font_size, sp_font_size, sp_padding };
    side_panel->setBackgroundColor(sp_background_color);
    side_panel->setTitle("Controls");
    side_panel->addText("UP - Add point");
    side_panel->addText("DOWN - Remove point");
    side_panel->addText("Mouse wheel - Add / Remove point");
    side_panel->addText("LMB - Add point");
    side_panel->addText("RMB - Remove point");
    side_panel->addText("");
    side_panel->addText("I - Toggle interactive mode");
    side_panel->addText("");
    side_panel->addText("G - Toggle GUI visibility");
    side_panel->addText("B - Toggle borders visibility");
    side_panel->addText("P - Toggle points visibility");
    side_panel->addText("J - Toggle cells coloring");
    side_panel->addText("R - Randomize points");
    side_panel->addText("ESC - Close");
    side_panel->addText("");
    side_panel->addText("C - Show controls");
}

GUI::GUI(sf::RenderWindow* _window)
{
    this->window = _window;

    initFont();
    initText();
    initSidePanel();
}

GUI::~GUI()
{
	for (auto& text : v_GUI_text)
		delete text;
    delete side_panel;
	window = nullptr;
}

void GUI::update(float dt)
{
    side_panel->update(dt);
}

void GUI::toggleSidePanel()
{
    side_panel->toggle();
}

void GUI::render()
{
    for (const auto& text : v_GUI_text)
        window->draw(*text);

    side_panel->draw(*window);

    window->display();
}
