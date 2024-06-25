#include "../PointManager.h"

void calculateMesh(const PointManager& pManager, sf::Vector2u window_size, Point ***meshTable) {
    sf::Color colorTable[20] = {
    sf::Color::Red,
    sf::Color::Green,
    sf::Color::Blue,
    sf::Color::Yellow,
    sf::Color::Magenta,
    sf::Color::Cyan,
    sf::Color(128, 0, 128), // Purple
    sf::Color(255, 165, 0), // Orange
    sf::Color(128, 128, 0), // Olive
    sf::Color(0, 128, 128), // Teal
    sf::Color(0, 0, 128),   // Navy
    sf::Color(255, 192, 203), // Pink
    sf::Color(128, 0, 0),   // Maroon
    sf::Color(0, 128, 0),   // Dark Green
    sf::Color(75, 0, 130),  // Indigo
    sf::Color(255, 20, 147), // Deep Pink
    sf::Color(210, 105, 30), // Chocolate
    sf::Color(105, 105, 105), // Dim Gray
    sf::Color(255, 250, 205), // Lemon Chiffon
    sf::Color(173, 216, 230) // Light Blue
    };

    for (size_t i = 0; i < window_size.y; ++i) {
        for (size_t j = 0; j < window_size.x; ++j) {
            meshTable[i][j]->pointShape.setFillColor(colorTable[pManager.getClosestPointID(sf::Vector2f(j, i))]);
        }
    }

}

int main()
{
    constexpr static size_t window_size_x = 800, window_size_y = 800;

    srand(static_cast<unsigned int>(NULL));

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(window_size_x, window_size_y), "Voronoi diagram", 7U, settings);
    
    PointManager pManager{ window.getSize(), 20 };

    Point*** meshTable = new Point **[window_size_y];
    for (int i = 0; i < window_size_y; ++i) {
        meshTable[i] = new Point * [window_size_x];
        for (int j = 0; j < window_size_x; ++j) {
            meshTable[i][j] = new Point{ sf::Vector2f{static_cast<float>(j), static_cast<float>(i)}, 1.f, sf::Color::Green };
        }
    }
    calculateMesh(pManager, window.getSize(), meshTable);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                pManager.reset();
                calculateMesh(pManager, window.getSize(), meshTable);
            }
        }

        window.clear(sf::Color::White);

        for (size_t i = 0; i < window.getSize().y; ++i)
            for (size_t j = 0; j < window.getSize().x; ++j)
                window.draw(meshTable[i][j]->pointShape);

        pManager.render(window);
        window.display();
    }

    return 0;
}