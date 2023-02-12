
#include "Node.h"

Node::Node(Tile *tile, Node *next) {
    this->tile = tile;
    this->next = next;
}

Node::Node(Node &other) {
    this->tile = new Tile(other.tile->letter, other.tile->value);
    this->next = other.next;
}

Node::~Node() {
    delete tile;
}
