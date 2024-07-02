#include "../PointManager.h"

struct Edge {
    sf::Vector2f p1{}, p2{};

    Edge() = delete;
    Edge(const sf::Vector2f& p, const sf::Vector2f& q) {
        p1 = p;
        p2 = q;
    }
};

class Cell {
    sf::ConvexShape shape;

public:
    Cell() {};
    Cell(const sf::ConvexShape& obj) { shape = obj; };

    Cell& setPoints(const std::initializer_list<sf::Vector2f> points) {
        shape.setPointCount(points.size());

        for (size_t i = 0; i < points.size(); ++i)
            shape.setPoint(i, *(points.begin() + i));

        return *this;
    }

    Cell& addPoint(const sf::Vector2f point) {
        shape.setPointCount(shape.getPointCount() + 1);
        shape.setPoint(shape.getPointCount() - 1, point);

        return *this;
    }

    bool contains(sf::Vector2f point) {
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

    operator sf::ConvexShape& () { return shape; };

    size_t getPointCount() const { return shape.getPointCount(); }
    sf::Vector2f getPoint(size_t index) const { return shape.getPoint(index); }
};

class Bisector {
    int a, b, c;

public:
    Bisector() = delete;
    Bisector(const sf::Vector2f& p1, const sf::Vector2f& p2) {
        float x0 = (p1.x + p2.x) / 2.f;
        float y0 = (p1.y + p2.y) / 2.f;

        a = p2.x - p1.x;
        b = p2.y - p1.y;
        c = -(a * x0) - (b * y0);
    }

    std::pair<bool, sf::Vector2f> intersection(const Edge& edge) const { return intersection(edge.p1, edge.p2); }

    std::pair<bool, sf::Vector2f> intersection(const sf::Vector2f& p1, const sf::Vector2f& p2) const{
        const float t = (-(a * p1.x + b * p1.y + c) / (a * (p2.x - p1.x) + b * (p2.y - p1.y)));

        if (t < 0 || t > 1) return std::pair{ false, sf::Vector2f{} };

        const float x = p1.x + (t * (p2.x - p1.x));
        const float y = p1.y + (t * (p2.y - p1.y));

        return std::pair{ true, sf::Vector2f{x, y} };
    }

    void draw(sf::RenderWindow& window) {
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
};

std::vector<sf::ConvexShape> calculateMesh(const std::vector<Point*> &v_points, const sf::ConvexShape &box, bool outline = true, bool randomColor = false) {
    std::vector<sf::ConvexShape> v_cells;

    size_t first_intersection_index{};
    size_t second_intersection_index{};
    sf::Vector2f t{}, u{}, v{};
    size_t m{};

    for (const Point *p : v_points) {
        Cell cell{ box };

        for (const Point* q : v_points) {
            if (q == p) continue;

            m = cell.getPointCount();
            Cell new_cell;

            Bisector two_points_bisector{ p->position, q->position };

            for (size_t k = 0; k < m; ++k) {
                sf::Vector2f A{ cell.getPoint(k) };
                sf::Vector2f B{ cell.getPoint((k + 1) % m )};

                Edge edge{ A, B };

                std::pair<bool, sf::Vector2f> intersection = two_points_bisector.intersection(edge);

                if (intersection.first == true) {
                    t = intersection.second;

                    if (t == B) {
                        new_cell.setPoints({ B, cell.getPoint((k + 2) % m) });
                        first_intersection_index = (k + 2) % m;
                    }
                    else {
                        new_cell.setPoints({ t, B });
                        first_intersection_index = (k + 1) % m;
                    }

                    break;
                }
            }

            if (new_cell.getPointCount() == 0)
                new_cell = cell;
            else {
                for (size_t k = first_intersection_index; k < m; ++k) {
                    sf::Vector2f A{ cell.getPoint(k) };
                    sf::Vector2f B{ cell.getPoint((k + 1) % m) };

                    Edge edge{ A, B };

                    std::pair<bool, sf::Vector2f> intersection = two_points_bisector.intersection(edge);

                    if (intersection.first == true) {
                        u = intersection.second;
                        new_cell.addPoint(u);
                        second_intersection_index = k + 1;
                        break;
                    }
                    else
                        new_cell.addPoint(B);
                }

                if (new_cell.contains(p->position) == false) {
                    new_cell.setPoints({ u });

                    while ((second_intersection_index % m) != first_intersection_index) {
                        v = cell.getPoint(second_intersection_index % m);
                        new_cell.addPoint(v);
                        second_intersection_index++;
                    }

                    new_cell.addPoint(t);
                }
            }
            
            cell = new_cell;
        }

        v_cells.push_back(cell);
    }

    for (auto& cell : v_cells) {
        if (outline) {
            cell.setOutlineThickness(1.f);
            cell.setOutlineColor(sf::Color::Black);
        }
        
        if (randomColor) {
            sf::Uint8 r = (rand() % 256 - 64) + 64;
            sf::Uint8 g = (rand() % 256 - 64) + 64;
            sf::Uint8 b = (rand() % 256 - 64) + 64;
            cell.setFillColor(sf::Color{ r, g, b });
        }
    }

    return v_cells;
}

int main()
{
    srand(static_cast<unsigned int>(NULL));

    constexpr static size_t window_size_x = 800, window_size_y = 800;

    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(window_size_x, window_size_y), "Voronoi diagram", 7U, settings);
    
    sf::ConvexShape box{ 4U };
    box.setPoint(0, sf::Vector2f{ 0.f, 0.f });
    box.setPoint(1, sf::Vector2f{ static_cast<float>(window.getSize().x), 0.f });
    box.setPoint(2, static_cast<sf::Vector2f>(window.getSize()));
    box.setPoint(3, sf::Vector2f{ 0.f, static_cast<float>(window.getSize().y) });

    PointManager pManager{ window.getSize(), 10 };
    std::vector<sf::ConvexShape> cells = calculateMesh(pManager.getPoints(), box);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                pManager.reset();
                cells = calculateMesh(pManager.getPoints(), box);
            }

            if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                pManager.getPoints().back()->pointShape.setPosition(static_cast<sf::Vector2f>(mousePos));
                pManager.getPoints().back()->position = static_cast<sf::Vector2f>(mousePos);
                cells = calculateMesh(pManager.getPoints(), box);
            }
        }
        
        window.clear(sf::Color::White);
        
        for (const auto& cell : cells)
            window.draw(cell);

        pManager.render(window);
        window.display();
    }

    return 0;
}