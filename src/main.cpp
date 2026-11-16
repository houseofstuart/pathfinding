
#include "Planner/planner.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

constexpr unsigned grid_width = 100;
constexpr unsigned grid_height = 100;
constexpr float cell = 8.f;

void setCellColor(sf::VertexArray& grid, std::array<int, 2> pos, sf::Color color) {
    sf::Vertex* tri = &grid[(pos[1] * grid_width + pos[0]) * 6];
    for (int i = 0; i < 6; ++i)
        tri[i].color = color;
}

void handleEvents(sf::RenderWindow& window) {
    while (const auto event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>())
            window.close();
    }
}

sf::VertexArray createGrid() {
    sf::VertexArray grid(
        sf::PrimitiveType::Triangles,
        grid_width * grid_height * 6
    );

    for (unsigned y = 0; y < grid_height; ++y) {
        for (unsigned x = 0; x < grid_width; ++x) {
            sf::Vertex* v = &grid[(y * grid_width + x) * 6];

            float px = x * cell;
            float py = y * cell;

            sf::Vector2f tl{px, py};
            sf::Vector2f tr{px + cell, py};
            sf::Vector2f br{px + cell, py + cell};
            sf::Vector2f bl{px, py + cell};

            v[0].position = tl;
            v[1].position = tr;
            v[2].position = bl;

            v[3].position = tr;
            v[4].position = br;
            v[5].position = bl;

            for (int i = 0; i < 6; ++i)
                v[i].color = sf::Color(200, 200, 200);
        }
    }
    return grid;
}

void initialiseGrid(sf::VertexArray& grid, Planner& planner) {
    setCellColor(grid, planner.start_pos, sf::Color(200, 0, 0));
    setCellColor(grid, planner.end_pos, sf::Color(0, 200, 0));
    for (const auto& [x, y] : planner.getWalls()) {
        setCellColor(grid, {x, y}, sf::Color::Black);
    }
}

void updateGrid(sf::VertexArray& grid, Planner& planner) {
    const auto& cells = planner.getCells();
    const int sid = planner.grid.toID(planner.start_pos);
    const int eid = planner.grid.toID(planner.end_pos);
    
    for (int i : planner.dirty_cells) {
        sf::Color color(200,200,200);
        if (cells[i].wall)
            color = sf::Color::Black;
        else if (i == sid || i == eid)
            color = sf::Color::Blue;
        else if (planner.isPathCell(i))
            color = sf::Color::Green;
        else if (cells[i].state == CellState::Open)
            color = sf::Color(255, 165, 0);
        else if (cells[i].state == CellState::Closed)
            color = sf::Color::Red;
        setCellColor(grid, planner.grid.toPos(i), color);
    }
    planner.dirty_cells.clear();
}

void render(sf::RenderWindow& window, sf::VertexArray& grid, Planner& planner) {
    updateGrid(grid, planner);
    window.clear();
    window.draw(grid);
    window.display();
}

int main() {
    Planner planner(100, 100);
    planner.start({11, 11}, {10, 60});
    bool searching = true;

    sf::RenderWindow window(sf::VideoMode({800, 800}), "Pathfinder");
    window.setFramerateLimit(60);
    
    sf::VertexArray grid = createGrid();
    initialiseGrid(grid, planner);

    while (window.isOpen()) {
        handleEvents(window);
        if (searching) {
            searching = !planner.step();
        }
        render(window, grid, planner);
    }
    return 0;
}
