
#ifndef ASSIGN2_TILE_H
#define ASSIGN2_TILE_H

#include <iomanip>
#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <random>
#include <sstream>
#include <utility>
#include <vector>
#include <memory>
#include <ctime>

using namespace std;

// Define a Letter type
typedef char Letter;

// Define a Value type
typedef int Value;

class Tile {
public:
    Tile(): letter(' '), value(0) {}
    Tile(Letter letter, Value value): letter(letter), value(value) {}
    inline bool isEmpty() const { return letter == ' '; }
    // overloading operator
    bool operator==(const char& c) const {
        return letter == c;
    }

    bool operator!=(const char& c) const {
        return letter != c;
    }

    Letter getLetter() const {
        return letter;
    }

    Letter letter;
    Value value;
};

// random number between min and max
inline int getRandomNumber(int min, int max) {
    return rand() % (max - min + 1) + min;
}


inline string intToString(int number) {
    stringstream ss;
    ss << number;
    return ss.str();
}

class Position {
public:
    int x;
    int y;
    Position(int x, int y):x(x), y(y) {}
    bool operator==(const Position &p) const {
        return (this->x == p.x && this->y == p.y);
    }
};



#endif // ASSIGN2_TILE_H
