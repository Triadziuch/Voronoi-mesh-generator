#pragma once
#include "MovementManager/SidePanel.h"
#include "VoronoiMesh.h"
#include "GUI.h"

class Application {
private:
	// Window variables
	sf::RenderWindow* window;

	constexpr static unsigned int window_size_x = 800U, window_size_y = 800U;
	constexpr static unsigned int depth = 0U, stencil = 0U, antialiasing = 16U, major = 1U, minor = 1U, attributes = 0U;
	constexpr static bool sRgb = false;
	
	const sf::Color background_color{ sf::Color::White };
	
	// Point manager variables
	PointManager* pManager;
	const size_t defaultPointCount{ 16U };

	// Clock variables
	sf::Clock dt_clock;
	float dt;

	// Mesh update optimization variables
	const float max_time_between_updates{ 0.03f };
	float		time_since_last_update{};
	bool		update_needed{};

	// GUI Variables
	GUI* gui;

	// Application configuration variables
	bool is_gui_visible = true;
	bool is_interactive = false;
	bool is_points_visible = true;

	// Voronoi mesh
	VoronoiMesh* voronoi;

	// Initialization functions
	void initWindow();
	void initPointManager();
	void initGUI();
	void initVoronoiMesh();

public:
	// Constructors / Destructors
	Application();
	~Application();

	void run();

	// Update functions
	void update();
	void updatePollEvents();
	void updateVoronoiMesh();

	// Render functions
	void render();
};