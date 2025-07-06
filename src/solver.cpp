/**
 * Dungeon Pathfinder - BFS Solver
 *
 * This file implements BFS pathfinding algorithms for dungeon navigation,
 * including key-door mechanics using bitmask BFS.
 */

#include "solver.h"
#include "cell.h"
#include <vector>
#include <algorithm>
#include <string>
#include <queue>
#include <unordered_map>
#include <unordered_set>

using namespace std;

// Find the position of a specific character in the dungeon
Cell findPosition(const vector<string>& dungeon, char target) {
    for (size_t row = 0; row < dungeon.size(); row++) {
        for (size_t col = 0; col < dungeon[row].size(); col++) {
            if (dungeon[row][col] == target) {
                return Cell(static_cast<int>(row), static_cast<int>(col));
            }
        }
    }
    return Cell(-1, -1);
}

// Check if the position is in bounds and not a wall
static bool isValidCell(const vector<string>& dungeon, int row, int col) {
    return row >= 0 && static_cast<size_t>(row) < dungeon.size() &&
           col >= 0 && static_cast<size_t>(col) < dungeon[0].size() &&
           dungeon[row][col] != '#';
}

// Reconstruct a path from parent map
static vector<Cell> reconstructPath(const unordered_map<Cell, Cell, CellHash>& parents,
                                    const Cell& start, const Cell& goal) {
    vector<Cell> path;
    Cell current = goal;
    while (!(current == start)) {
        path.push_back(current);
        current = parents.at(current);
    }
    path.push_back(start);
    reverse(path.begin(), path.end());
    return path;
}

vector<Cell> bfsPath(const vector<string>& dungeon) {
    Cell start = findPosition(dungeon, 'S');
    Cell goal = findPosition(dungeon, 'E');
    if (start.r == -1 || goal.r == -1) return {};

    queue<Cell> q;
    unordered_map<Cell, Cell, CellHash> parents;
    unordered_set<Cell, CellHash> visited;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        Cell cur = q.front(); q.pop();
        if (cur == goal) return reconstructPath(parents, start, goal);

        for (int d = 0; d < NUM_DIRECTIONS; d++) {
            int nr = cur.r + DIRECTIONS[d][0];
            int nc = cur.c + DIRECTIONS[d][1];
            if (isValidCell(dungeon, nr, nc)) {
                char ch = dungeon[nr][nc];
                if (ch >= 'A' && ch <= 'F') continue; // basic BFS can't go through doors
                Cell next(nr, nc);
                if (!visited.count(next)) {
                    visited.insert(next);
                    parents[next] = cur;
                    q.push(next);
                }
            }
        }
    }
    return {};
}

struct State {
    int row, col, keys;
    bool operator==(const State& other) const {
        return row == other.row && col == other.col && keys == other.keys;
    }
};

struct StateHash {
    size_t operator()(const State& s) const {
        return ((size_t)s.row << 20) ^ ((size_t)s.col << 10) ^ s.keys;
    }
};

vector<Cell> bfsPathKeys(const vector<string>& dungeon) {
    Cell start = findPosition(dungeon, 'S');
    Cell goal = findPosition(dungeon, 'E');
    if (start.r == -1 || goal.r == -1) return {};

    queue<State> q;
    unordered_map<State, State, StateHash> parent;
    unordered_set<State, StateHash> visited;

    State startState{start.r, start.c, 0};
    q.push(startState);
    visited.insert(startState);

    while (!q.empty()) {
        State cur = q.front(); q.pop();
        if (cur.row == goal.r && cur.col == goal.c) {
            // reconstruct path
            vector<Cell> path;
            State s = cur;
            while (!(s.row == start.r && s.col == start.c && s.keys == 0)) {
                path.push_back(Cell(s.row, s.col));
                s = parent[s];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        for (int d = 0; d < NUM_DIRECTIONS; d++) {
            int nr = cur.row + DIRECTIONS[d][0];
            int nc = cur.col + DIRECTIONS[d][1];
            if (!isValidCell(dungeon, nr, nc)) continue;

            char ch = dungeon[nr][nc];
            int newKeys = cur.keys;

            if (ch >= 'A' && ch <= 'F') {
                int keyBit = ch - 'A';
                if (!((newKeys >> keyBit) & 1)) continue; // door locked
            }
            if (ch >= 'a' && ch <= 'f') {
                int keyBit = ch - 'a';
                newKeys |= (1 << keyBit); // collect key
            }

            State next{nr, nc, newKeys};
            if (!visited.count(next)) {
                visited.insert(next);
                parent[next] = cur;
                q.push(next);
            }
        }
    }
    return {};
}

#ifdef IMPLEMENT_OPTIONAL_FUNCTIONS
int countReachableKeys(const std::vector<std::string>& dungeon) {
    Cell start = findPosition(dungeon, 'S');
    if (start.r == -1) return 0;
    queue<Cell> q;
    unordered_set<Cell, CellHash> visited;
    int keys = 0;

    q.push(start);
    visited.insert(start);

    while (!q.empty()) {
        Cell cur = q.front(); q.pop();
        char ch = dungeon[cur.r][cur.c];
        if (ch >= 'a' && ch <= 'f') {
            keys |= (1 << (ch - 'a'));
        }
        for (int d = 0; d < NUM_DIRECTIONS; d++) {
            int nr = cur.r + DIRECTIONS[d][0];
            int nc = cur.c + DIRECTIONS[d][1];
            if (isValidCell(dungeon, nr, nc)) {
                Cell next(nr, nc);
                if (!visited.count(next)) {
                    visited.insert(next);
                    q.push(next);
                }
            }
        }
    }
    int count = 0;
    for (int i = 0; i < 6; i++) {
        if ((keys >> i) & 1) count++;
    }
    return count;
}
#else
int countReachableKeys(const std::vector<std::string>&) {
    return 0;
}
#endif
