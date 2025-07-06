#include "generator.h"
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>

using namespace std;

// Directions: up, down, left, right — two steps to allow walls between paths
const vector<pair<int, int>> DIRECTIONS = {
    {-2, 0}, {2, 0}, {0, -2}, {0, 2}
};

// Helper: Check if a cell is valid and carveable
bool isValidCell(int row, int col, int rows, int cols) {
    return row > 0 && row < rows - 1 &&
           col > 0 && col < cols - 1 &&
           row % 2 == 1 && col % 2 == 1;
}

// Helper: Carve a passage between two cells by removing the wall
void carvePassage(vector<string>& dungeon, int fromRow, int fromCol, int toRow, int toCol) {
    int wallRow = (fromRow + toRow) / 2;
    int wallCol = (fromCol + toCol) / 2;
    dungeon[toRow][toCol] = ' ';
    dungeon[wallRow][wallCol] = ' ';
}

// Recursive maze generation using backtracking
void generateMaze(vector<string>& dungeon, int row, int col, int rows, int cols, mt19937& rng) {
    vector<pair<int, int>> dirs = DIRECTIONS;
    shuffle(dirs.begin(), dirs.end(), rng);

    for (const auto& dir : dirs) {
        int newRow = row + dir.first;
        int newCol = col + dir.second;

        if (isValidCell(newRow, newCol, rows, cols) && dungeon[newRow][newCol] == '#') {
            carvePassage(dungeon, row, col, newRow, newCol);
            generateMaze(dungeon, newRow, newCol, rows, cols, rng);
        }
    }
}

// Main function: generates the dungeon grid
vector<string> generateDungeon(int rows, int cols, int roomRate) {
    if (rows % 2 == 0) rows++;  // ensure odd size
    if (cols % 2 == 0) cols++;

    vector<string> dungeon(rows, string(cols, '#'));

    mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    dungeon[1][1] = ' ';  // Start carving from top-left

    generateMaze(dungeon, 1, 1, rows, cols, rng);

    // Add extra rooms based on roomRate percentage
    int totalCells = ((rows - 1) / 2) * ((cols - 1) / 2);
    int extraRooms = totalCells * roomRate / 100;

    for (int i = 0; i < extraRooms; ++i) {
        int r = rng() % rows;
        int c = rng() % cols;
        if (dungeon[r][c] == '#' && r % 2 == 1 && c % 2 == 1) {
            dungeon[r][c] = ' ';
        }
    }

    // Place start 'S' and exit 'E'
    dungeon[1][1] = 'S';

    // Find a suitable place for 'E' (searching from bottom right)
    for (int r = rows - 2; r > 0; --r) {
        for (int c = cols - 2; c > 0; --c) {
            if (dungeon[r][c] == ' ') {
                dungeon[r][c] = 'E';
                return dungeon;
            }
        }
    }

    // Fallback exit placement
    dungeon[rows - 2][cols - 2] = 'E';
    return dungeon;
}
