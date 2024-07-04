#include "Bisector.h"

// Constructors
Bisector::Bisector(const sf::Vector2f& p1, const sf::Vector2f& p2)
{
    float x0 = (p1.x + p2.x) / 2.f;
    float y0 = (p1.y + p2.y) / 2.f;

    a = p2.x - p1.x;
    b = p2.y - p1.y;
    c = -(a * x0) - (b * y0);
}

// Public member functions
const std::pair<bool, sf::Vector2f> &Bisector::intersection(const sf::Vector2f& p1, const sf::Vector2f& p2) const
{
    const float t = (-(a * p1.x + b * p1.y + c) / (a * (p2.x - p1.x) + b * (p2.y - p1.y)));

    if (t < 0 || t > 1) return std::pair{ false, sf::Vector2f{} };

    const float x = p1.x + (t * (p2.x - p1.x));
    const float y = p1.y + (t * (p2.y - p1.y));

    return std::pair{ true, sf::Vector2f{x, y} };
}

const std::pair<bool, sf::Vector2f> &Bisector::intersection(const Edge& edge) const
{
    return intersection(edge.p1, edge.p2);
}

const void Bisector::draw(sf::RenderWindow& window) const
{
    sf::RectangleShape box{ static_cast<sf::Vector2f>(window.getSize()) };
    sf::VertexArray line{ sf::LineStrip, 2U };
    bool first = true;

    for (size_t i = 0; i < 4; ++i) {
        std::pair<bool, sf::Vector2f> inter = intersection(Edge{ box.getPoint(i % 4), box.getPoint((i + 1) % 4) });

        if (inter.first == true) {
            if (first) {
                line.operator[](0).position = inter.second;
                line.operator[](0).color = sf::Color::Black;
                first = false;
            }
            else {
                line.operator[](1).position = inter.second;
                line.operator[](1).color = sf::Color::Black;
            }
        }
    }

    window.draw(line);
}