#include "VoronoiMesh.h"

VoronoiMesh::VoronoiMesh(const sf::RenderWindow& window)
{
    box.setPointCount(4U);
    box.setPoint(0U, sf::Vector2f{ 0.f, 0.f });
    box.setPoint(1U, sf::Vector2f{ static_cast<float>(window.getSize().x), 0.f });
    box.setPoint(2U, static_cast<sf::Vector2f>(window.getSize()));
    box.setPoint(3U, sf::Vector2f{ 0.f, static_cast<float>(window.getSize().y) });
}

void VoronoiMesh::calculate(const std::vector<Point*>& v_points)
{
    std::vector<sf::ConvexShape> v_cells;

    size_t first_intersection_index{};
    size_t second_intersection_index{};
    sf::Vector2f t{}, u{}, v{};
    size_t m{};

    for (const Point* p : v_points) {
        Cell cell{ box };

        for (const Point* q : v_points) {
            if (q == p) continue;

            m = cell.getPointCount();
            Cell new_cell;

            Bisector two_points_bisector{ p->position, q->position };

            for (size_t k = 0; k < m; ++k) {
                sf::Vector2f A{ cell.getPoint(k) };
                sf::Vector2f B{ cell.getPoint((k + 1) % m) };

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

        if (is_border_visible)
            cell.setOutlineThickness(1.f);
    }

    if (is_colored) {
        size_t i = 0U, j = 0U;

        for (i, j; i < v_cells.size() && j < cells.size(); ++i, ++j)
            v_cells[i].setFillColor(cells.at(j).getFillColor());

        for (i; i < v_cells.size(); ++i) {
            sf::Uint8 r = (rand() % 256 - 64) + 64;
            sf::Uint8 g = (rand() % 256 - 64) + 64;
            sf::Uint8 b = (rand() % 256 - 64) + 64;
            v_cells[i].setFillColor(sf::Color{ r, g, b });
        }
    }

    this->cells = v_cells;
}

void VoronoiMesh::toggleBorderVisibility()
{
    is_border_visible = !is_border_visible;

    if (is_border_visible)
        for (auto& cell : cells) 
            cell.setOutlineThickness(1.f);
    else 
        for (auto& cell : cells) 
            cell.setOutlineThickness(0.f);
}

void VoronoiMesh::toggleCellsColoring()
{
    is_colored = !is_colored;

    if (is_colored)
        for (auto& cell : cells) {
            sf::Uint8 r = (rand() % 256 - 64) + 64;
            sf::Uint8 g = (rand() % 256 - 64) + 64;
            sf::Uint8 b = (rand() % 256 - 64) + 64;
            cell.setFillColor(sf::Color{ r, g, b });
        }
    else
        for (auto& cell : cells)
            cell.setFillColor(sf::Color::White);
}

void VoronoiMesh::render(sf::RenderWindow* window)
{
    for (const auto& cell : cells)
        window->draw(cell);
}
