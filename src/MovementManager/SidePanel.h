#pragma once
#include <SFML/Graphics.hpp>
#include "MovementManager.h"
#include "easeFunctions.h"
#include <vector>
#include <string>

class SidePanel {
private:
	MovementManager* m_movementManager{};
	const sf::RenderTarget* m_window{};

	// Font variables
	std::string  m_fontPath{};
	sf::Font	 m_font{};
	unsigned int m_titleFontSize{};
	unsigned int m_textFontSize{};
	float		 m_padding{};
	
	// Movement variables
	float		m_duration{ 0.5f };
	bool		m_hidden{ true };
	const bool* m_stationary{};
	easeFunctions::Tmovement_function m_usedFunction{ easeFunctions::OUT_QUART };
	
	// Background variables
	float				m_width{};
	float				m_height{};
	sf::RectangleShape	m_background{};
	sf::Color			m_backgroundColor{ sf::Color{ 0, 0, 0, 200 } };
	std::string			m_backgroundMovementName{ "BackgroundMovement" };
	MovementRoutine*	m_backgroundMovementRoutine{};

	// Title variables
	sf::Text				m_title{};
	std::vector<sf::Text*>	m_textObjects{};
	std::string				m_titleMovementName{ "TitleMovement" };
	MovementRoutine*		m_titleMovementRoutine{};

	// Text variables
	std::string					  m_textMovementNames{ "TextMovement" };
	std::vector<MovementRoutine*> m_textMovementRoutines{};

	// Private functions
	void instantHide();
	void recalculateTextPositions();

public:
	// Constructors / Destructors
	SidePanel();
	SidePanel(const sf::RenderTarget& window, const std::string& fontPath, unsigned int  titleFontSize = 32u, unsigned int textFontSize = 16u, float padding = 12.f);
	SidePanel(const SidePanel& obj);
	~SidePanel();

	void update(const float dt);

	// Accessors / Mutators
	void setWindow(const sf::RenderTarget& window);
	void setFont(const std::string& fontPath);
	void setFont(const sf::Font& font);

	void setTitleFontSize(unsigned fontSize);
	void setTextFontSize(unsigned fontSize);
	void setPadding(float padding);
	void setWidth(float width);
	void setHeight(float height);
	void setBackgroundColor(const sf::Color& color);

	unsigned int getTitleFontSize() const;

	void setTitle(const std::string& title);
	void addText(const std::string& label);
	void clearText();

	void show();
	void hide();
	void toggle();

	// Draw functions
	void draw(sf::RenderTarget& window);
};