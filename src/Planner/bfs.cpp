//
//  bfs.cpp
//  PathfindingVisual
//
//  Created by Stuart Roy on 16/11/2026.
//

#include "bfs.hpp"
#include <iostream>

bfsPlanner::bfsPlanner(int w, int h) : grid(w, h), path_cells(w * h, false) {}

void bfsPlanner::start(std::array<int, 2> start, std::array<int, 2> end) {
    start_pos = start;
    end_pos = end;
    
    // Build walls
    for (int i = 3; i < 56; i++) {
        setWall({i, 50}, true);
        dirty_cells.push_back(grid.toID({i, 50}));
    }
    for (int i = 51; i < 85; i++) {
        setWall({3, i}, true);
        dirty_cells.push_back(grid.toID({3, i}));
    }

    int sid = grid.toID(start);
    grid.cells[sid].state = CellState::Closed;
    Q.push(sid);
    
    dirty_cells.push_back(sid);
    dirty_cells.push_back(grid.toID(end));
}

bool bfsPlanner::step() {
    
    if (Q.empty()) {
        return true;
    }
    
    int cid = Q.front();
    Q.pop();
    if (cid == grid.toID(end_pos)) {
        path = buildPath(cid);
        return true;
    }
    
    dirty_cells.push_back(cid);
    
    std::vector<std::array<int, 2>> neighbours = possibleMoves(grid.toPos(cid));
    for (const auto& pos : neighbours) {
        int nid = grid.toID(pos);
        Cell& neighbour_cell = cell(nid);
        
        if (!neighbour_cell.wall && neighbour_cell.state != CellState::Closed) {
            neighbour_cell.state = CellState::Closed;
            neighbour_cell.parent = cid;
            Q.push(nid);
            dirty_cells.push_back(nid);
        }
    }
    return false;
}

std::vector<std::array<int, 2>> bfsPlanner::possibleMoves(std::array<int, 2> pos) {
    int x = pos[0];
    int y = pos[1];
    int moves[4][2] = {
        {x + 1, y},
        {x - 1, y},
        {x, y + 1},
        {x, y - 1}
    };
    std::vector<std::array<int, 2>> valid_moves;
    for (int i = 0; i < 4; i++) {
        if (moves[i][0] >= 0 && moves[i][0] <= 99 && moves[i][1] >= 0 && moves[i][1] <= 99) {
            valid_moves.push_back({moves[i][0], moves[i][1]});
        }
    }
    return valid_moves;
}

Cell& bfsPlanner::cell(int id) {
    return grid.getCell(id);
}
Cell& bfsPlanner::cell(std::array<int, 2> pos) {
    return grid.getCell(pos);
}

const std::vector<Cell>& bfsPlanner::getCells() const {
    return grid.cells;
}

std::vector<std::array<int, 2>> bfsPlanner::getWalls() {
    return grid.getWalls();
}

void bfsPlanner::setWall(std::array<int, 2> pos, bool wall) {
    grid.cells[grid.toID(pos)].wall = wall;
    grid.walls.push_back(pos);
}

bool bfsPlanner::isPathCell(int id) const {
    return path_cells[id];
}

std::vector<int> bfsPlanner::buildPath(int id) {
    std::vector<int> path;
    while (id != -1) {
        path.push_back(id);
        id = cell(id).parent;
    }
    for (int pid: path) {
        path_cells[pid] = true;
        dirty_cells.push_back(pid);
    }
    return path;
}

