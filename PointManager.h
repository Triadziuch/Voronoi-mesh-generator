#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <random>
#include <math.h>
#include <limits>

class Point
{
public:
	sf::Vector2f position;
	sf::CircleShape pointShape;

	Point();
	Point(const Point& obj);
	Point(const sf::Vector2f& pos, float radius, const sf::Color& color);
};

class PointManager
{
public:
	// Configuration
	static constexpr size_t defaultPointCount = 5;
	static constexpr float defaultPointRadius = 2.5f;
	const sf::Color defaultPointColor{ sf::Color::Black };

private:
	// Private variables
	size_t pointCount;
	sf::Vector2u windowSize{ 1920u, 1080u };

	// Points
	std::vector<Point*> v_points;

public:
	PointManager();
	PointManager(const PointManager &obj);
	PointManager(const sf::Vector2u& window_size = sf::Vector2u{ 1920u, 1080u }, const size_t & count = defaultPointCount, float radius = defaultPointRadius, const sf::Color & color = sf::Color::Black);
	~PointManager();

	const size_t getClosestPointID(const sf::Vector2f& pos) const;
	void reset();
	void render(sf::RenderWindow &window);
};

namespace util {
	inline sf::Vector2f randomPos(sf::Vector2u window_size = { 1920u, 1080u }, float pointRadius = PointManager::defaultPointRadius) {
		return { static_cast<float>((rand() % (window_size.x - 4u * static_cast<unsigned>(pointRadius))) + 2u * pointRadius),
				 static_cast<float>((rand() % (window_size.y - 4u * static_cast<unsigned>(pointRadius))) + 2u * pointRadius) };
	}

	inline float distance(const sf::Vector2f& p1, const sf::Vector2f& p2) {
		double x1x2 = static_cast<double>(p1.x - p2.x), y1y2 = static_cast<double>(p1.y - p2.y);

		return sqrt((x1x2 * x1x2) + (y1y2 * y1y2));
	}
}