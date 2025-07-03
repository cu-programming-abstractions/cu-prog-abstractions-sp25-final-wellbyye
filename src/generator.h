#pragma once
#include <vector>
#include <string>

/**
 * Generates a random dungeon using recursive backtracking algorithm.
 * Creates a perfect maze (exactly one path between any two points) and
 * optionally adds extra rooms for gameplay variety.
 * 
 * @param rows Number of rows in the dungeon (should be odd for proper maze)
 * @param cols Number of columns in the dungeon (should be odd for proper maze)
 * @param roomRate Percentage (0-100) of additional rooms to punch after maze generation
 * @return 2D dungeon represented as vector of strings, with:
 *         '#' = walls, ' ' = open space, 'S' = start, 'E' = exit
 */
std::vector<std::string> generateDungeon(int rows, int cols, int roomRate = 20);

/**
 * Helper function to check if a cell coordinate is valid within the dungeon bounds
 * and represents a carveable cell (odd coordinates in a perfect maze).
 * 
 * @param row The row coordinate to check
 * @param col The column coordinate to check  
 * @param rows Total number of rows in the dungeon
 * @param cols Total number of columns in the dungeon
 * @return true if the cell is valid and carveable, false otherwise
 */
bool isValidCell(int row, int col, int rows, int cols);

/**
 * Helper function to carve a passage between two adjacent cells by removing
 * the wall between them. Used during recursive backtracking.
 * 
 * @param dungeon The dungeon grid to modify
 * @param fromRow Starting cell row
 * @param fromCol Starting cell column
 * @param toRow Target cell row
 * @param toCol Target cell column
 */
void carvePassage(std::vector<std::string>& dungeon, int fromRow, int fromCol, int toRow, int toCol);