//
//  planner.hpp
//  PathfindingVisual
//
//  Created by Stuart Roy on 16/11/2026.
//

#pragma once
#include "../Cell/cell.hpp"
#include "../Grid/grid.hpp"
#include <array>
#include <vector>
#include <algorithm>
#include <queue>
#include <cmath>


class Planner {
public:
    Planner(int w, int h);
    
    Grid grid;
    Cell& cell(std::array<int, 2> pos);
    Cell& cell(int id);
    const std::vector<Cell>& getCells() const;
    
    std::array<int, 2> start_pos = {-1, 1};
    std::array<int, 2> end_pos = {-1, -1};
    std::vector<int> path;
    
    std::priority_queue<
                std::array<int, 3>,
                std::vector<std::array<int, 3>>,
                std::greater<std::array<int, 3>>> open_list;
    
    void start(std::array<int, 2> start, std::array<int, 2> end);
    bool step();
    
    std::vector<int> buildPath(int id);
    std::vector<std::array<int, 2>> possibleMoves(std::array<int, 2> pos);
    
    void setWall(std::array<int, 2> pos, bool wall);
    std::vector<std::array<int, 2>> getWalls();
    
    std::vector<bool> path_cells;
    std::vector<int> dirty_cells;
    bool isPathCell(int id) const;
    
private:
    int heuristic(int id) const;
    int heuristic(std::array<int, 2> pos) const;
    int euclidean(std::array<int, 2> pos) const;
};
