//
//  cell.hpp
//  PathfindingVisual
//
//  Created by Stuart Roy on 16/11/2026.
//


#pragma once
#include <limits>

enum class CellState {
    Unvisited,
    Open,
    Closed
};

struct Cell {
    CellState state = CellState::Unvisited;
    int g = std::numeric_limits<int>::max();
    int parent = -1;
    bool wall = false;
};



