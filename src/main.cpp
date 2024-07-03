#include "../PointManager.h"
#include "MovementManager/SidePanel.h"

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

std::vector<sf::ConvexShape> calculateMesh(const std::vector<Point*> &v_points, const sf::ConvexShape &box, bool outline = true, bool randomColor = false, std::vector<sf::ConvexShape>* old_cells = nullptr) {
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
        cell.setOutlineColor(sf::Color::Black);

        if (outline) 
            cell.setOutlineThickness(1.f);
    }

    if (randomColor) {
        size_t i = 0U;
        if (old_cells != nullptr) {
            size_t j = 0U;

            for (i, j; i < v_cells.size() && j < old_cells->size(); ++i, ++j)
                v_cells[i].setFillColor(old_cells->at(j).getFillColor());
        }

        for (i; i < v_cells.size(); ++i) {
            sf::Uint8 r = (rand() % 256 - 64) + 64;
            sf::Uint8 g = (rand() % 256 - 64) + 64;
            sf::Uint8 b = (rand() % 256 - 64) + 64;
            v_cells[i].setFillColor(sf::Color{ r, g, b });
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
    
    const sf::Color background_color{ sf::Color::White };

    sf::ConvexShape box{ 4U };
    box.setPoint(0, sf::Vector2f{ 0.f, 0.f });
    box.setPoint(1, sf::Vector2f{ static_cast<float>(window.getSize().x), 0.f });
    box.setPoint(2, static_cast<sf::Vector2f>(window.getSize()));
    box.setPoint(3, sf::Vector2f{ 0.f, static_cast<float>(window.getSize().y) });

    PointManager pManager{ window.getSize(), 16 };
    //std::vector<sf::ConvexShape> cells = calculateMesh(pManager.getPoints(), box, is_border_visible, is_colored, &cells);

    sf::Clock dt_clock;
    float dt;

    bool update_needed = false;
    float time_since_last_update = 0.f;
    float max_time_between_updates = 0.03f;

    // = = = = = GUI Variables = = = = = 
    bool is_gui_visible = true;
    bool is_interactive = false;
    bool is_border_visible = true;
    bool is_colored = true;
    bool is_points_visible = true;

    std::vector<sf::ConvexShape> cells = calculateMesh(pManager.getPoints(), box, is_border_visible, is_colored);

    sf::Font font;
    font.loadFromFile("assets/Fonts/ClearSans-Bold.ttf");

    std::vector<sf::Text*> v_GUI_text;

    SidePanel sidePanel(window, "assets/Fonts/ClearSans-Bold.ttf", 32u, 16u, 20.f);
    sidePanel.setBackgroundColor(sf::Color(50, 50, 50, 245));
    sidePanel.setTitle("Controls");
    sidePanel.addText("UP - Add point");
    sidePanel.addText("DOWN - Remove point");
    sidePanel.addText("Mouse wheel - Add / Remove point");
    sidePanel.addText("LMB - Add point");
    sidePanel.addText("RMB - Remove point");
    sidePanel.addText("");
    sidePanel.addText("I - Toggle interactive mode");    
    sidePanel.addText("");
    sidePanel.addText("G - Toggle GUI visibility");
    sidePanel.addText("B - Toggle borders visibility");
    sidePanel.addText("P - Toggle points visibility");
    sidePanel.addText("J - Toggle cells coloring");
    
    sidePanel.addText("R - Randomize points");
    sidePanel.addText("ESC - Close");
    sidePanel.addText("");
    sidePanel.addText("C - Show controls");

    sf::Text controls_text{ "[C] - Controls", font, 20u };
    controls_text.setFillColor(sf::Color::Black);
    controls_text.setPosition(10.f, 10.f);
    v_GUI_text.push_back(&controls_text);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
                window.close();

            if (event.type == sf::Event::KeyPressed) {

                // R = Randomize points
                if (event.key.code == sf::Keyboard::R) {
                    pManager.reset();
                    cells = calculateMesh(pManager.getPoints(), box, is_border_visible, is_colored, &cells);
                }

                // C - Toggle controls panel
                if (event.key.code == sf::Keyboard::C) {
                    sidePanel.toggle();
                }

                // UP - Add points
                if (event.key.code == sf::Keyboard::Up) {
                    ++pManager;
                    cells = calculateMesh(pManager.getPoints(), box, is_border_visible, is_colored, &cells);
                }

                // DOWN - Remove points
                if (event.key.code == sf::Keyboard::Down) {
                    --pManager;
                    cells = calculateMesh(pManager.getPoints(), box, is_border_visible, is_colored, &cells);
                }

                // G - Toggle GUI visibility
                if (event.key.code == sf::Keyboard::G) {
                    is_gui_visible = !is_gui_visible;
                }

                // B - Toggle border visibility
                if (event.key.code == sf::Keyboard::B) {
                    is_border_visible = !is_border_visible;

                    if (is_border_visible == false)
                        for (auto& cell : cells)
                            cell.setOutlineThickness(0.f);
                    else
                        for (auto& cell : cells)
                            cell.setOutlineThickness(1.f);
                }

                // J - Toggle cells coloring
                if (event.key.code == sf::Keyboard::J) {
                    is_colored = !is_colored;

                    if (is_colored == false)
                        for (auto& cell : cells)
                            cell.setFillColor(background_color);
                    else
                        cells = calculateMesh(pManager.getPoints(), box, is_border_visible, is_colored);
                }

                // P - Toggle points visibility
                if (event.key.code == sf::Keyboard::P) {
                    is_points_visible = !is_points_visible;
                }

                // I - Toggle interactive mode
                if (event.key.code == sf::Keyboard::I) {
                    is_interactive = !is_interactive;
                }
            }
            
            // Mouse wheel - Add / Remove point
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta > 0) {
                    if (is_interactive) {
                        --pManager;
                        ++pManager;
                    }
                    ++pManager;
                }
                else 
                    --pManager;
                cells = calculateMesh(pManager.getPoints(), box, is_border_visible, is_colored, &cells);
            }

            // LMB - Add point
            if (!is_interactive && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                pManager.addPoint(static_cast<sf::Vector2f>(mousePos));
                cells = calculateMesh(pManager.getPoints(), box, is_border_visible, is_colored, &cells);
            }

            // RMB - Remove point
            if (!is_interactive && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                pManager.removePoint(static_cast<sf::Vector2f>(mousePos));
                cells = calculateMesh(pManager.getPoints(), box, is_border_visible, is_colored, &cells);
            }

            if (is_interactive && event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                if (pManager.getPoints().empty() == false) {
                    pManager.getPoints().back()->pointShape.setPosition(static_cast<sf::Vector2f>(mousePos));
                    pManager.getPoints().back()->position = static_cast<sf::Vector2f>(mousePos);
                    update_needed = true;
                }
            }
        }

        dt = dt_clock.restart().asSeconds();
        sidePanel.update(dt);

        if (update_needed) {
            time_since_last_update += dt;

            if (time_since_last_update >= max_time_between_updates) {
                update_needed = false;
                cells = calculateMesh(pManager.getPoints(), box, is_border_visible, is_colored, &cells);
                time_since_last_update = 0.f;
            }  
        }

        
        window.clear(background_color);
        
        for (const auto& cell : cells)
            window.draw(cell);

        if (is_points_visible)
            pManager.render(window);
        
        if (is_gui_visible)
            for (const auto& text : v_GUI_text)
                window.draw(*text);

        sidePanel.draw(window);

        window.display();
    }

    return 0;
}