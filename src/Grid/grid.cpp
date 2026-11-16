//
//  grid.cpp
//  PathfindingVisual
//
//  Created by Stuart Roy on 16/11/2026.
//

#include "../Cell/cell.hpp"
#include "grid.hpp"

Grid::Grid(int w, int h) : width(w), height(h), cells (w * h) {}

Cell& Grid::getCell(std::array<int, 2> pos) {
    return cells[toID(pos)];
}

Cell& Grid::getCell(int id) {
    return cells[id];
}

std::vector<std::array<int, 2>> Grid::getWalls() const {
    return walls;
}

int Grid::toID(std::array<int, 2> pos) {
    return pos[0] * width + pos[1];
}

std::array<int, 2> Grid::toPos(int id) {
    return {
        (id / width), (id % height)
    };
}
