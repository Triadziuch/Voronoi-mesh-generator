#pragma once
#include <SFML/Graphics.hpp>

class Cell {
    // Private variables
    sf::ConvexShape shape;

public:
    // Constructors / Destructors
    Cell();
    Cell(const sf::ConvexShape& obj);

    // Public member functions
    Cell& setPoints(const std::initializer_list<sf::Vector2f>& points);
    Cell& addPoint(const sf::Vector2f& point);
    bool contains(const sf::Vector2f& point);

    // Accessors
    const size_t getPointCount() const;
    const sf::Vector2f getPoint(size_t index) const;

    // Operators
    operator sf::ConvexShape& ();
};