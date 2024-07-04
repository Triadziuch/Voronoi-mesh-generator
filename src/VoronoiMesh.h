#pragma once
#include <vector>
#include "Bisector.h"
#include "PointManager.h"
#include "Cell.h"

class VoronoiMesh {
private:
	sf::ConvexShape box;
	std::vector<sf::ConvexShape> cells;

	bool is_border_visible = true;
	bool is_colored = false;

public:
	// Constructors
	VoronoiMesh() = delete;
	VoronoiMesh(const sf::RenderWindow& window);

	void calculate(const std::vector<Point*>& v_points);

	void toggleBorderVisibility();
	void toggleCellsColoring();

	void render(sf::RenderWindow* window);
};