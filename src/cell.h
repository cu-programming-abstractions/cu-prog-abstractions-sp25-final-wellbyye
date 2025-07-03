#pragma once

/**
 * Simple coordinate structure for representing positions in the dungeon.
 * Used throughout the pathfinding algorithms to track locations.
 */
struct Cell {
    int r, c;  // row, column coordinates
    
    // Constructor for easy cell creation
    Cell(int row = 0, int col = 0) : r(row), c(col) {}
    
    // Equality comparison for finding cells in containers
    bool operator==(const Cell& other) const {
        return r == other.r && c == other.c;
    }
    
    // Inequality comparison
    bool operator!=(const Cell& other) const {
        return !(*this == other);
    }
};

/**
 * Hash function for using Cell in unordered containers like unordered_set.
 * This enables efficient lookups in the BFS visited tracking.
 */
struct CellHash {
    size_t operator()(const Cell& cell) const {
        // Simple hash combining row and column
        return static_cast<size_t>(cell.r) * 1000 + static_cast<size_t>(cell.c);
    }
};

// Direction vectors for moving in 4 cardinal directions (up, down, left, right)
// Useful for both maze generation and pathfinding
const int DIRECTIONS[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
const int NUM_DIRECTIONS = 4;