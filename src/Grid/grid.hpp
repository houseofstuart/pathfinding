//
//  grid.hpp
//  PathfindingVisual
//
//  Created by Stuart Roy on 16/11/2026.
//

#pragma once

#include "../Cell/cell.hpp"
#include <vector>
#include <array>

class Grid {
    
public:
    int width;
    int height;
    
    std::vector<Cell> cells;
    std::vector<std::array<int, 2>> walls;
    
    Grid(int w, int h);
    
    int toID(std::array<int, 2> pos);
    std::array<int, 2> toPos(int id);
    
    Cell& getCell(std::array<int, 2> pos);
    Cell& getCell(int id);
    std::vector<std::array<int, 2>> getWalls() const;
};
