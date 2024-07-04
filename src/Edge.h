#pragma once
#include <SFML/Graphics.hpp>

struct Edge {
    sf::Vector2f p1{}, p2{};

    Edge() = delete;
    Edge(const sf::Vector2f& p, const sf::Vector2f& q);
};