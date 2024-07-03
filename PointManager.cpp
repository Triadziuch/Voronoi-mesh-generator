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
	pointShape.setRadius(radius);
	pointShape.setOrigin(sf::Vector2f{ radius, radius });
	pointShape.setPosition(position);
	pointShape.setFillColor(color);
}

PointManager::PointManager()
{
	pointCount = defaultPointCount;
	pointRadius = defaultPointRadius;
	pointColor = defaultPointColor;

	for (size_t i = 0; i < defaultPointCount; ++i) 
		v_points.push_back(new Point(util::randomPos(), defaultPointRadius, defaultPointColor));
}

PointManager::PointManager(const PointManager& obj)
{
	pointCount = obj.pointCount;
	pointRadius = obj.pointRadius;
	pointColor = obj.pointColor;

	for (const auto& point : obj.v_points)
		v_points.push_back(new Point{ *point });
}

PointManager::PointManager(const sf::Vector2u& window_size, const size_t& count, float radius, const sf::Color& color)
{
	pointCount = count;
	pointRadius = radius;
	pointColor = color;
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

const std::vector<Point*>& PointManager::getPoints() const
{
	return v_points;
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

void PointManager::addPoint(sf::Vector2f position)
{
	for (const auto& point : v_points)
		if (abs(point->pointShape.getPosition().x - position.x) < pointRadius && abs(point->pointShape.getPosition().y - position.y) < pointRadius)
			return;

	v_points.push_back(new Point{ position, pointRadius, pointColor });
	pointCount++;
}

void PointManager::removePoint(sf::Vector2f position)
{
	for (size_t i = 0; i < v_points.size(); ++i) 
		if (abs(v_points[i]->pointShape.getPosition().x - position.x) < pointRadius && abs(v_points[i]->pointShape.getPosition().y - position.y) < pointRadius) {
			v_points.erase(v_points.begin() + i);
			pointCount--;
			return;
		}
}

PointManager PointManager::operator++(void)
{
	v_points.push_back(new Point{ util::randomPos(windowSize), pointRadius, pointColor });
	++pointCount;
	return *this;
}

PointManager PointManager::operator--(void)
{
	if (!v_points.empty()) {
		v_points.pop_back();
		--pointCount;
	}
		
	return *this;
}
