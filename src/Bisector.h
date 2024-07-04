#pragma once
#include <SFML/Graphics.hpp>
#include "Edge.h"

class Bisector {
    // Private variables
    int a, b, c;

public:
    // Constructors
    Bisector() = delete;
    Bisector(const sf::Vector2f& p1, const sf::Vector2f& p2);

    // Public member functions
    const std::pair<bool, sf::Vector2f> &intersection(const sf::Vector2f& p1, const sf::Vector2f& p2) const;
    const std::pair<bool, sf::Vector2f> &intersection(const Edge& edge) const;

    const void draw(sf::RenderWindow& window) const;
};