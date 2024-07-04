#include "Cell.h"

// Constructors / Destructors
Cell::Cell() {};

Cell::Cell(const sf::ConvexShape& obj) : shape{ obj } {}

// Public member functions
Cell& Cell::setPoints(const std::initializer_list<sf::Vector2f>& points)
{
    shape.setPointCount(points.size());

    for (size_t i = 0; i < points.size(); ++i)
        shape.setPoint(i, *(points.begin() + i));

    return *this;
}

Cell& Cell::addPoint(const sf::Vector2f& point)
{
    shape.setPointCount(shape.getPointCount() + 1);
    shape.setPoint(shape.getPointCount() - 1, point);

    return *this;
}

bool Cell::contains(const sf::Vector2f& point)
{
    size_t numPoints = shape.getPointCount();
    bool inside = false;

    for (size_t i = 0, j = numPoints - 1; i < numPoints; j = i++) {
        sf::Vector2f pi = shape.getPoint(i);
        sf::Vector2f pj = shape.getPoint(j);

        if (((pi.y > point.y) != (pj.y > point.y)) &&
            (point.x < (pj.x - pi.x) * (point.y - pi.y) / (pj.y - pi.y) + pi.x)) {
            inside = !inside;
        }
    }

    return inside;
}

// Accessors
const size_t Cell::getPointCount() const
{
    return shape.getPointCount();
}

const sf::Vector2f Cell::getPoint(size_t index) const
{
    return shape.getPoint(index);
}

// Operators
Cell::operator sf::ConvexShape& ()
{
    return shape;
}


