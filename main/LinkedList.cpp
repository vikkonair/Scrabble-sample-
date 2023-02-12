
#include "LinkedList.h"

#include <random>
#include <sstream>

int getLetterValue(char letter) {
    int arr[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
    int arr_size = sizeof(arr) / sizeof(arr[0]);
    const vector<int> vec(arr, arr + arr_size);
    return vec[letter - 'A'];
}

static inline void ltrim(std::string& s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
        }));
}

LinkedList::LinkedList() {
    head = nullptr;
}

LinkedList::~LinkedList() {
}

void LinkedList::add(Tile *tile) {
    if (head == nullptr) {
        head = new Node(tile, nullptr);
    } else {
        Node *curr = head;
        while (curr->next != nullptr) {
            curr = curr->next;
        }
        curr->next = new Node(tile, nullptr);
    }
}

void LinkedList::clear() {
    Node *curr = head;
    while (curr != nullptr) {
        Node *temp = curr;
        curr = curr->next;
        delete temp;
    }
    head = nullptr;
}

void LinkedList::shuffleList() {
    std::vector<Tile *> list;
    Node *curr = head;
    Node* newHead = nullptr;
    while (curr != nullptr) {
        list.push_back(curr->tile);
        curr = curr->next;
    }

    while (!list.empty()){
        int size = list.size();
        int random = getRandomNumber(0, size - 1);
        Tile *tile = list[random];
        list.erase(list.begin() + random);
		if (newHead == nullptr) {
			newHead = new Node(tile, nullptr);
		} else {
			Node *curr = newHead;
			while (curr->next != nullptr) {
				curr = curr->next;
			}
			curr->next = new Node(tile, nullptr);
		}
    }

    head = newHead;
}


void LinkedList::saveNodes(ofstream &file) {
    // save letter in node to file sep by space
    Node *curr = head;
    while (curr != nullptr) {
        file << curr->tile->getLetter() << " ";
        curr = curr->next;
    }
    file << endl;
}

Node *LinkedList::getHead() {
    return head;
}

void LinkedList::removeHead() {
    if (head == nullptr) {
        return;
    }
    Node *temp = head;
    head = head->next;
    delete temp;
}

int LinkedList::getSize() {
    int size = 0;
    Node *curr = head;
    while (curr != nullptr) {
        size++;
        curr = curr->next;
    }
    return size;
}

void LinkedList::showList(std::ostream &out) {
    Node *curr = getHead();
    while (curr != nullptr) {
        out << curr->tile->letter << "-" << curr->tile->value;
        curr = curr->next;
        if (curr != nullptr) {
            out << ", ";
        }
    }
    out << std::endl;
}

void LinkedList::loadFromString(const std::string &str) {
    clear();
    std::stringstream ss(str);
    std::string partString;
    // split string by space
    while (std::getline(ss, partString, ' ')) {
        ltrim(partString);
        if (partString.empty()) {
            continue;
        }
        int value = getLetterValue(partString[0]);
        add(new Tile(partString[0], value));
    }
}

void LinkedList::removeByLetter(char letter) {
    Node *curr = head;
    Node *prev = nullptr;
    while (curr != nullptr) {
        if (curr->tile->letter == letter) {
            if (prev == nullptr) {
                head = curr->next;
            } else {
                prev->next = curr->next;
            }
            delete curr;
            curr = nullptr;
        }
        else
        {
			prev = curr;
			curr = curr->next;
        }
    }
}

bool LinkedList::isLetterInList(char letter) {
    Node *curr = head;
    while (curr != nullptr) {
        if (curr->tile->letter == letter) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

bool LinkedList::isEmpty() {
    return head == nullptr;
}


player::~player() {
	handTiles->clear();
	delete handTiles;
	handTiles = nullptr;
}

void player::resetPlayer() {
	name = "";
	score = 0;
	if (handTiles != nullptr) {
		handTiles->clear();
	} else {
		handTiles = new LinkedList();
	}
	passTimes = 0;
}

void player::drawTiles(LinkedList *tilesPool) const {
	int tilesCount = tilesPool->getSize();
	int tilesToDraw = MAX_TILES_COUNT_IN_HAND - handTiles->getSize();
	if (tilesToDraw > tilesCount) {
		tilesToDraw = tilesCount;
	}
	for (int i = 0; i < tilesToDraw; ++i) {
		handTiles->add(new Tile(tilesPool->getHead()->tile->letter,
								  tilesPool->getHead()->tile->value));
		tilesPool->removeHead();
	}
}
