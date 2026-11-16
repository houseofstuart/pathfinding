//
//  planner.cpp
//  PathfindingVisual
//
//  Created by Stuart Roy on 16/11/2026.
//

#include "planner.hpp"

Planner::Planner(int w, int h) : grid(w, h), path_cells(w * h, false) {}

void Planner::start(std::array<int, 2> start, std::array<int, 2> end) {
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

    // Initialise start cell properties (parent stays -1)
    int sid = grid.toID(start);
    grid.cells[sid].state = CellState::Open;
    grid.cells[sid].g = 0;
    open_list.push({0, heuristic(start), sid});
    
    dirty_cells.push_back(sid);
    dirty_cells.push_back(grid.toID(end));
}

bool Planner::step() {
    std::array<int, 3> current = open_list.top();
    int cid = current[2];
    Cell& current_cell = cell(cid);
    

    // Check if we've reached the goal
    if (cid == grid.toID(end_pos)) {
        path = buildPath(cid);
        return true;
    }
    // Move current node from open to closed list
    open_list.pop();
    if (current_cell.state == CellState::Closed)
        return false;
    current_cell.state = CellState::Closed;
    dirty_cells.push_back(cid);
    
    // Check all neighbouring cells
    std::vector<std::array<int, 2>> neighbours = possibleMoves(grid.toPos(cid));
    for (const auto& pos : neighbours) {
        
        int nid = grid.toID(pos);
        Cell& neighbour_cell = cell(nid);
        
        // Skip over if we've already evaluated it
        if (neighbour_cell.wall || neighbour_cell.state == CellState::Closed)
            continue;
        
        int tentative_g = current_cell.g + 1;
        int h = heuristic(pos);
        
        if (tentative_g < neighbour_cell.g) {
            neighbour_cell.g = tentative_g;
            neighbour_cell.parent = cid;
            neighbour_cell.state = CellState::Open;
            open_list.push({tentative_g + h, euclidean(pos), nid});
            dirty_cells.push_back(nid);
        }
    }
    return false;
}

std::vector<std::array<int, 2>> Planner::possibleMoves(std::array<int, 2> pos) {
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

Cell& Planner::cell(int id) {
    return grid.getCell(id);
}
Cell& Planner::cell(std::array<int, 2> pos) {
    return grid.getCell(pos);
}

const std::vector<Cell>& Planner::getCells() const {
    return grid.cells;
}

std::vector<std::array<int, 2>> Planner::getWalls() {
    return grid.getWalls();
}

void Planner::setWall(std::array<int, 2> pos, bool wall) {
    grid.cells[grid.toID(pos)].wall = wall;
    grid.walls.push_back(pos);
}

int Planner::heuristic(int id) const {
    int dx = abs((id % 100) - end_pos[0]);
    int dy = abs((id / 100) - end_pos[1]);
    return dx + dy;
}

int Planner::heuristic(std::array<int, 2> pos) const {
    int dx = abs(pos[0] - end_pos[0]);
    int dy = abs(pos[1] - end_pos[1]);
    return dx + dy;
}

int Planner::euclidean(std::array<int, 2> pos) const {
    int dx = abs(pos[0] - end_pos[0]);
    int dy = abs(pos[1] - end_pos[1]);
    return static_cast<int>(std::sqrt(dx * dx + dy * dy));
}

bool Planner::isPathCell(int id) const {
    return path_cells[id];
}

std::vector<int> Planner::buildPath(int id) {
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

