
#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Node.h"
#define MAX_TILES_COUNT_IN_HAND 7

class LinkedList {
public:

    LinkedList();

    ~LinkedList();

    void add(Tile *tile);

    void clear();

    void shuffleList();

    void removeHead();

    int getSize();

    void showList(std::ostream &out = std::cout);

    void loadFromString(const std::string &str);

    void removeByLetter(char letter);

    bool isLetterInList(char letter);

    bool isEmpty();

    void saveNodes(ofstream &file);

    Node *getHead();

private:
    Node *head;
};


class player {
public:
    string name;
    int score;
    int passTimes;
    LinkedList *handTiles;

    player(): score(0), passTimes(0), handTiles(new LinkedList()) {}
    player(string name, int score): name(move(name)), score(score), passTimes(0), handTiles(new LinkedList()) {}
    ~player();
    void resetPlayer(); 
    string getName() const { return name; }
    int getScore() const { return score; }
    void setScore(int pScore) { this->score = pScore; }
    void setName(string pName) { this->name = std::move(pName); }
    void showTilesInHand(ostream &out = cout) const { cout << endl; }

    void drawTiles(LinkedList* tilesPool) const;
};


int getLetterValue(char letter);

#endif // ASSIGN2_LINKEDLIST_H
