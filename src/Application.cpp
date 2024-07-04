#include "Application.h"

void Application::initWindow()
{
    sf::ContextSettings settings{ depth, stencil, antialiasing, major, minor, attributes, sRgb };
    window = new sf::RenderWindow{ sf::VideoMode{ window_size_x, window_size_y }, "Voronoi diagram", 7U, settings };
}

void Application::initPointManager()
{
    pManager = new PointManager{ window->getSize(), defaultPointCount };
}

void Application::initGUI()
{
    gui = new GUI{ window };
}

void Application::initVoronoiMesh()
{
    voronoi = new VoronoiMesh{ *window };
    voronoi->calculate(pManager->getPoints());
}

// Constructors / Destructors
Application::Application()
{
    srand(static_cast<unsigned int>(NULL));

    initWindow();
    initPointManager();
    initGUI();
    initVoronoiMesh();
}

Application::~Application()
{
    delete pManager;
    delete gui;
    delete voronoi;
    delete window;
}

void Application::run() {
    while (window->isOpen()) {
        update();
        render();
    }
}

// Update functions
void Application::update() {
    dt = dt_clock.restart().asSeconds();

    gui->update(dt);
    updatePollEvents();
    updateVoronoiMesh();
}

void Application::updatePollEvents() {
    sf::Event event;
    while (window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
            window->close();

        if (event.type == sf::Event::KeyPressed) {

            // R = Randomize points
            if (event.key.code == sf::Keyboard::R) {
                pManager->reset();
                voronoi->calculate(pManager->getPoints());
            }

            // C - Toggle controls panel
            if (event.key.code == sf::Keyboard::C) {
                gui->toggleSidePanel();
            }

            // UP - Add points
            if (event.key.code == sf::Keyboard::Up) {
                ++pManager;
                voronoi->calculate(pManager->getPoints());
            }

            // DOWN - Remove points
            if (event.key.code == sf::Keyboard::Down) {
                --pManager;
                voronoi->calculate(pManager->getPoints());
            }

            // G - Toggle GUI visibility
            if (event.key.code == sf::Keyboard::G) {
                is_gui_visible = !is_gui_visible;
            }

            // B - Toggle border visibility
            if (event.key.code == sf::Keyboard::B) {
                voronoi->toggleBorderVisibility();
            }

            // J - Toggle cells coloring
            if (event.key.code == sf::Keyboard::J) {
                voronoi->toggleCellsColoring();
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
                    --*pManager;
                    ++*pManager;
                }
                ++*pManager;
            }
            else
                --*pManager;

            voronoi->calculate(pManager->getPoints());
        }

        // LMB - Add point
        if (!is_interactive && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
            pManager->addPoint(static_cast<sf::Vector2f>(mousePos));
            voronoi->calculate(pManager->getPoints());
        }

        // RMB - Remove point
        if (!is_interactive && event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
            pManager->removePoint(static_cast<sf::Vector2f>(mousePos));
            voronoi->calculate(pManager->getPoints());
        }

        // I - Interactive mode cell movement
        if (is_interactive && event.type == sf::Event::MouseMoved) {
            sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
            if (pManager->getPoints().empty() == false) {
                pManager->getPoints().back()->pointShape.setPosition(static_cast<sf::Vector2f>(mousePos));
                pManager->getPoints().back()->position = static_cast<sf::Vector2f>(mousePos);
                update_needed = true;
            }
        }
    }
}

void Application::updateVoronoiMesh()
{
    if (update_needed) {
        time_since_last_update += dt;

        if (time_since_last_update >= max_time_between_updates) {
            update_needed = false;
            voronoi->calculate(pManager->getPoints());
            time_since_last_update = 0.f;
        }
    }
}

// Render functions
void Application::render()
{
    window->clear(background_color);
    
    voronoi->render(window);

    if (is_points_visible)
        pManager->render(*window);

    if (is_gui_visible)
        gui->render();
}