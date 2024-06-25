#include "PointManager.h"

Point::Point()
{
	pointShape.setRadius(1.f);
	pointShape.setFillColor(sf::Color::Green);
}

Point::Point(const Point& obj)
{
	position = obj.position;
	pointShape = obj.pointShape;
}

Point::Point(const sf::Vector2f& pos, float radius, const sf::Color& color)
{
	position = pos;
	pointShape.setPosition(position);
	pointShape.setFillColor(color);
	pointShape.setRadius(radius);
}

PointManager::PointManager()
{
	pointCount = defaultPointCount;
	for (size_t i = 0; i < defaultPointCount; ++i) 
		v_points.push_back(new Point(util::randomPos(), defaultPointRadius, defaultPointColor));
}

PointManager::PointManager(const PointManager& obj)
{
	pointCount = obj.pointCount;
	for (const auto& point : obj.v_points)
		v_points.push_back(new Point{ *point });
}

PointManager::PointManager(const sf::Vector2u& window_size, const size_t& count, float radius, const sf::Color& color)
{
	pointCount = count;
	windowSize = window_size;

	for (size_t i = 0; i < pointCount; ++i)
		v_points.push_back(new Point{ util::randomPos(windowSize, radius), radius, color });
}

PointManager::~PointManager()
{
	for (auto& point : v_points)
		delete point;
	v_points.clear();
}

const size_t PointManager::getClosestPointID(const sf::Vector2f& pos) const
{
	size_t id = 0;
	float min_distance = 9999999999.f;

	for (size_t i = 0; i < v_points.size(); ++i) {
		const float distance = util::distance(v_points[i]->position, pos);
		if (distance < min_distance) {
			min_distance = distance;
			id = i;
		}
	}

	return id;
}

void PointManager::reset()
{
	if (v_points.empty()) return;

	sf::Color pointColor = v_points.front()->pointShape.getFillColor();
	float radius = v_points.front()->pointShape.getRadius();

	for (auto& point : v_points)
		delete point;
	v_points.clear();

	for (size_t i = 0; i < pointCount; ++i)
		v_points.push_back(new Point{ util::randomPos(windowSize), radius, pointColor });
}

void PointManager::render(sf::RenderWindow& window)
{
	for (const auto& point : v_points)
		window.draw(point->pointShape);
}