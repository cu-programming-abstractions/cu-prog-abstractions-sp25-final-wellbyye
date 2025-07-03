# Bitmask BFS Guide: Keys and Doors Pathfinding

## Table of Contents
- [Introduction](#introduction)
- [Basic BFS Review](#basic-bfs-review)
- [What is State Augmentation?](#what-is-state-augmentation)
- [Understanding Bitmasks](#understanding-bitmasks)
- [Bitmask Operations](#bitmask-operations)
- [Key-Door BFS Algorithm](#key-door-bfs-algorithm)
- [Implementation Details](#implementation-details)
- [Practice Exercises](#practice-exercises)
- [Real-World Applications](#real-world-applications)

## Introduction

This guide explains **Bitmask BFS**, an advanced graph algorithm that extends basic Breadth-First Search (BFS) to handle complex state spaces. In our dungeon pathfinder, we use it to solve puzzles with keys and doors.

### Why Do We Need Advanced BFS?

In basic BFS, our "state" is just a position `(row, col)`. But what if reaching the same position with different "abilities" or "items" gives us different future possibilities?

**Example:** Reaching position `(5,3)` with no keys vs. reaching `(5,3)` with key 'a' are fundamentally different situations!

## Basic BFS Review

Before diving into bitmask BFS, let's review basic BFS:

```cpp
// Basic BFS pseudocode
queue<Position> queue
set<Position> visited
queue.push(start)
visited.add(start)

while (!queue.empty()) {
    current = queue.front()
    queue.pop()
    
    if (current == goal) return "Found!"
    
    for each neighbor of current {
        if (neighbor not in visited) {
            queue.push(neighbor)
            visited.add(neighbor)
        }
    }
}
```

**Key insight:** Each position is visited at most once.

## What is State Augmentation?

**State augmentation** means expanding our definition of "state" beyond just position.

### Basic BFS State
```
State = (row, col)
```

### Augmented BFS State  
```
State = (row, col, additional_info)
```

For key-door problems:
```
State = (row, col, keys_collected)
```

### Why This Matters

With state augmentation, the same position can be visited multiple times if we have different key combinations:

- `(5,3, no keys)` ≠ `(5,3, key 'a')` ≠ `(5,3, keys 'a' and 'b')`

Each represents a unique state with different future possibilities!

## Understanding Bitmasks

A **bitmask** is an efficient way to represent a set of boolean flags using a single integer.

### Concept

Instead of using an array like `bool keys[6]`, we use one integer where each bit represents a key:

```
Bit position:  5 4 3 2 1 0
Key:           f e d c b a
```

### Examples

| keyMask | Binary | Keys Collected |
|---------|--------|----------------|
| 0       | 000000 | none           |
| 1       | 000001 | a              |
| 2       | 000010 | b              |
| 3       | 000011 | a, b           |
| 5       | 000101 | a, c           |
| 7       | 000111 | a, b, c        |
| 63      | 111111 | a, b, c, d, e, f |

### Benefits of Bitmasks

1. **Memory efficient:** 1 integer vs. array of booleans
2. **Fast operations:** Bitwise operations are very fast
3. **Easy comparison:** Can compare entire sets with `==`
4. **Compact hashing:** Easy to use in hash tables

## Bitmask Operations

### Check if key is collected
```cpp
bool hasKey(int keyMask, char key) {
    int bit = key - 'a';  // 'a'->0, 'b'->1, etc.
    return (keyMask >> bit) & 1;
}

// Example: Do we have key 'c' when keyMask = 5?
// bit = 'c' - 'a' = 2
// (5 >> 2) & 1 = (101 >> 2) & 1 = 1 & 1 = 1 (YES!)
```

### Add a key
```cpp
int addKey(int keyMask, char key) {
    int bit = key - 'a';
    return keyMask | (1 << bit);
}

// Example: Add key 'b' to keyMask = 5
// bit = 'b' - 'a' = 1
// 5 | (1 << 1) = 5 | 2 = 101 | 010 = 111 = 7
```

### Count total keys
```cpp
int countKeys(int keyMask) {
    int count = 0;
    for (int i = 0; i < 6; i++) {
        if ((keyMask >> i) & 1) count++;
    }
    return count;
}
```

## Key-Door BFS Algorithm

### State Structure

```cpp
struct KeyState {
    int row, col;    // Position
    int keyMask;     // Bitmask of collected keys
};
```

### Algorithm Overview

```cpp
// Bitmask BFS pseudocode
queue<KeyState> queue
set<KeyState> visited
queue.push(KeyState(start.row, start.col, 0))  // Start with no keys

while (!queue.empty()) {
    current = queue.front()
    queue.pop()
    
    if (current.position == goal) return "Found!"
    
    for each neighbor of current.position {
        if (neighbor is passable) {
            newKeyMask = current.keyMask
            
            // If neighbor is a key, collect it
            if (neighbor contains key) {
                newKeyMask = addKey(newKeyMask, key)
            }
            
            // If neighbor is a door, check if we can pass
            if (neighbor contains door) {
                if (!hasKey(newKeyMask, correspondingKey)) {
                    continue  // Can't pass this door
                }
            }
            
            newState = KeyState(neighbor.row, neighbor.col, newKeyMask)
            
            if (newState not in visited) {
                queue.push(newState)
                visited.add(newState)
            }
        }
    }
}
```

## Implementation Details

### Key-Door Relationships

- Key 'a' opens door 'A'
- Key 'b' opens door 'B'
- etc.

### State Space Explosion

**Important:** The number of possible states grows exponentially!

- Basic BFS: `O(rows × cols)` states
- Key BFS: `O(rows × cols × 2^numKeys)` states

With 6 keys: `2^6 = 64` possible key combinations per position!

### Memory Considerations

For a 20×20 dungeon with 6 keys:
- Basic BFS: 400 states maximum
- Key BFS: 400 × 64 = 25,600 states maximum

This is why efficient implementation matters!

## Practice Exercises

### Exercise 1: Bitmask Basics
Work through these by hand:

1. If `keyMask = 10`, what keys do we have?
2. How do we add key 'd' to `keyMask = 3`?
3. How do we check if we have key 'f' when `keyMask = 33`?

<details>
<summary>Solutions</summary>

1. `10 = binary 001010 = keys 'b' and 'd'`
2. `key 'd' = bit 3, so 3 | (1 << 3) = 3 | 8 = 11`
3. `key 'f' = bit 5, so (33 >> 5) & 1 = 1 (yes, we have it!)`

</details>

### Exercise 2: State Comparison
Are these states the same or different?

1. `(row=5, col=3, keyMask=7)` vs `(row=5, col=3, keyMask=7)`
2. `(row=5, col=3, keyMask=5)` vs `(row=5, col=3, keyMask=6)`
3. `(row=5, col=3, keyMask=3)` vs `(row=4, col=3, keyMask=3)`

<details>
<summary>Solutions</summary>

1. **Same** - All components match
2. **Different** - Same position, different keys (5 = a,c vs 6 = b,c)
3. **Different** - Different positions, same keys

</details>

### Exercise 3: Algorithm Tracing
Given this simple dungeon:
```
###
#Sa#
#A E#
####
```

Trace through the key-door BFS algorithm. What states are visited?

<details>
<summary>Solution</summary>

1. Start: `(1,1, keyMask=0)`
2. Move right: `(1,2, keyMask=1)` - collected key 'a'
3. Move down: `(2,2, keyMask=1)` - can pass door 'A' since we have key 'a'
4. Move right: `(2,3, keyMask=1)` - reached exit!

Total states visited: 4

</details>

## Real-World Applications

### Video Games
- **RPG Pathfinding:** Character abilities affect accessible areas
- **Puzzle Games:** Collected items change movement options
- **Strategy Games:** Unit upgrades modify tactical possibilities

### Robotics
- **Tool Selection:** Robot's current tools affect task possibilities
- **Multi-Robot Systems:** Team capabilities change over time
- **Exploration:** Discovered information affects future paths

### Network Routing
- **Access Control:** User permissions affect accessible resources
- **Quality of Service:** Current network state affects routing options
- **Security:** Encryption keys determine accessible connections

### General Problem Pattern

Use bitmask BFS whenever:
1. You have a pathfinding problem
2. Reaching the same location with different "states" or "capabilities" gives different future options
3. The number of possible states is manageable (typically ≤ 20 boolean flags)

## Tips for Implementation

### Debugging
1. **Print keyMask in binary** to visualize collected keys
2. **Trace simple examples** by hand before coding
3. **Use helper functions** for readability (`hasKey()`, `addKey()`)
4. **Start small** - test with 1-2 keys before scaling up

### Optimization
1. **Use efficient hash functions** for state storage
2. **Consider early termination** if all keys are collected
3. **Implement state pruning** if possible
4. **Profile memory usage** for large state spaces

### Common Mistakes
1. **Forgetting state augmentation** - treating `(x,y,keys1)` same as `(x,y,keys2)`
2. **Incorrect key-door mapping** - mixing up 'a'/'A' relationships
3. **Bit operation errors** - off-by-one errors in bit positions
4. **Hash function collisions** - inadequate state hashing

---

## Conclusion

Bitmask BFS is a powerful extension of basic BFS that handles complex state spaces efficiently. By understanding state augmentation and bitmask operations, you can solve sophisticated pathfinding problems that basic algorithms cannot handle.

The key insight is that **position alone doesn't determine your possibilities** - your current state (position + capabilities) determines what you can do next!

## Further Reading

- [Competitive Programming Handbook - Bitmasks](https://cses.fi/book/book.pdf) (Chapter 10)
- [Introduction to Algorithms - BFS Variants](https://mitpress.mit.edu/books/introduction-algorithms-third-edition)
- [Game AI Pro - Advanced Pathfinding](http://www.gameaipro.com/) 