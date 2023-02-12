#include "LinkedList.h"


#define EXIT_SUCCESS 0
#define BINGO_SCORE 50
#define BOARD_SIZE 15
#define BOARD_ROW_COUNT 15
#define BOARD_COL_COUNT 15


void strSplit(const string &s, vector<string> &v, const char seq = ' ') {
    v.clear();
    string::size_type i = 0;
    string::size_type j = s.find(seq);
    while (j != string::npos) {
        v.push_back(s.substr(i, j - i));
        i = ++j;
        j = s.find(seq, j);
        if (j == string::npos) {
            v.push_back(s.substr(i, s.length()));
        }
    }

	if(!s.empty() && v.empty())
	{
        v.push_back(s);
	}
}

static void Credits() {
    cout << "Student Name: Lin-Chun Weng " << endl;
    cout << "Student ID: s3838169 " << endl;
    cout << "Email: s3838169@student.rmit.edu.au " << endl;
}


void quit() {
	cout << "Goodbye" << endl;
	exit(0);
}


void isEOFPressed(const std::string &s) {
	for (auto &ch: s) {
		if (static_cast<int>(ch) == 4) {
			cout << "Goodbye" << endl;
			quit();
			return;
		}
	}
	if (cin.eof()) {
		cin.clear();
		cout << "Goodbye" << endl;
		quit();
	}
}

void printFileContentByFileName(const string& fileName){
    ifstream file;
    file.open(fileName);
    string line;
    while(getline(file, line)){
        cout << line << endl;
    }
}

bool ifStringIsDigit(const string& str){ 
    if(str.length() == 1 && isdigit(str[0] && str[0] != '0')){
        return true;
    }

    if(all_of(str.begin(), str.end(), ::isdigit)){
        return true;
    }
    return false;
}

int stringToInt(string str){ 
    int num = 0;
    for(auto const &ch: str){
        num = num * 10 + ch - '0';
    }
    return num;
}





class PlaceTileInfo {
public:
    int x;
    int y;
    bool isValidCommand;
    Tile tile;
    PlaceTileInfo() : x(0), y(0), isValidCommand(false){}
    PlaceTileInfo(int x, int y, char letter) : x(x), y(y), isValidCommand(false), tile(letter, getLetterValue(letter)) {}
};

// Class
class Scrabble {
public:
    vector<player*> playerList;
    player *currentPlayer;
    int row, col;
    vector<vector<Tile> > board;
    vector<PlaceTileInfo> tilesPlacedThisTurn;
    int commandIndex;
    string inFilePath;
    string outFilePath;
    vector<string> commandListLoaded;
    string saveFileName;
    LinkedList *tilesBag;
    bool isTestMode;

    Scrabble(int playerCount=2): row(BOARD_ROW_COUNT), col(BOARD_COL_COUNT),
	                             board(15, vector<Tile>(15, Tile())), commandIndex(0),
                                 tilesBag(new LinkedList()), isTestMode(false){
        tilesPlacedThisTurn = vector<PlaceTileInfo>();
        for (int i = 0; i < playerCount; ++i) {
            playerList.emplace_back(new player(intToString(i + 1), 0));
        } 

        currentPlayer = playerList[0];
        commandIndex = 0;
        Initialization();
    }

    void setTestMode()
    {
        isTestMode = true;
    }

    void resetBoard()
    {
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                board[i][j] = Tile(' ', 0);
            }
        }
    }


    void setTestFilePath(string inFp, string outFp) {
        inFilePath = std::move(inFp);
        outFilePath = std::move(outFp);
    }

    void showScores()
    {
        for(auto it = playerList.begin(); it != playerList.end(); ++it) {
            cout << (*it)->getName() << ": " << (*it)->getScore() << endl;
        }
    }

    void showGameResult()
    {
        int minScore = 100000;
        int maxScore = -1;
	    for(auto & it : playerList) {
            if(it->getScore() > maxScore) {
                maxScore = it->getScore();
            }
            if(it->getScore() < minScore) {
                minScore = it->getScore();
            }
        }
        // show losers
        cout << "Losers:" << endl;
        for(auto & it : playerList) {
            if (it->getScore() == minScore) {
                cout << it->getName() << endl;
            }
        }
        // show winners
        cout << "Winners:" << endl;
        for(auto & it : playerList) {
            if (it->getScore() == maxScore) {
                cout << it->getName() << endl;
            }
        }
    }

    void showCurrentPlayerInfo() {
        cout << currentPlayer->getName() << ", it's turn" << endl;
        showScores();
        showBoard();
        currentPlayer->handTiles->showList();
    }

    void drawTilesForEachPlayer() const{
        for(auto & it : playerList) {
            it->drawTiles(tilesBag);
        }
    }

    void Initialization() {
        resetBoard();
        tilesBag = new LinkedList();
        ifstream TilesFile("ScrabbleTiles.txt");
        if (TilesFile.is_open()) {
            string line;
            while (getline(TilesFile, line)) {
                vector<string> words;
                stringstream ss(line);
                string word;
                while (ss >> word) {
                    words.push_back(word);
                }
                Tile *tile = new Tile(words[0][0], stoi(words[1]));
                tilesBag->add(tile);
            }
            TilesFile.close();
        }

        if (inFilePath.empty()) {
            tilesBag->shuffleList();
        }

        drawTilesForEachPlayer();
    }

    void saveBoard(ofstream &out) {
        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < col; ++j) {
                if(board[i][j].getLetter() == ' ')
                {
					out << "* ";
                }
                else
                {
					out << board[i][j].getLetter() << " ";
                }
            }
			out << endl;
        }
    }

    void saveGame(const string &fileName) {
        ofstream file(fileName);
        if (file.is_open()) {
            for(auto const &it : playerList) {
                file << it->getName() << endl << it->getScore() << endl;
                it->handTiles->saveNodes(file);
            }
            saveBoard(file);
            tilesBag->saveNodes(file);
            file << currentPlayer->getName() << endl;
            cout << "Game saved successfully!" << endl;
            file.close();
        }
    }

    static bool checkUppercase(string str) {
        return all_of(str.begin(), str.end(), ::isupper);
    }

    void getUserName() {
        cout << "Starting a New Game" << endl << endl;
		int playerNum = 0;
        for(auto const &it : playerList) {
            string name;
            do {
                cout << "Enter a name for player " << playerNum + 1 << ": " << " (uppercase characters only) ";
                name = getUserCommand();
            } while (!checkUppercase(name));
            it->setName(name);
            playerNum++;
        }
    }

    static bool letterStringCheck(string letterStr)
    {
        return letterStr.size() == 1 && letterStr[0] <= 'Z' && letterStr[0] >= 'A';
    }

	static bool posStringCheck(string posStr)
    {
		int const colIndex = stoi(posStr.substr(1));
        return posStr.size() == 2 && posStr[0] <= 'O' && posStr[0] >= 'A' && colIndex <= 15 && colIndex >= 0;
    }

    PlaceTileInfo placeCommandValidCheck(vector<string> const  &cmd) {
        PlaceTileInfo placeResult;
        // place command syntax check
    	if (cmd.size() == 4 && cmd[0] == "place") {
            if (!letterStringCheck(cmd[1])) {
                return placeResult;
            }
            if (cmd[2] != "at") {
                return placeResult;
            }
            if (!posStringCheck(cmd[3])) {
                return placeResult;
            }
            int x = cmd[3][0] - 'A';
            int y = stoi(cmd[3].substr(1));

            // tile not in hand
            if (!currentPlayer->handTiles->isLetterInList(cmd[1][0])) {
                placeResult.isValidCommand = false;
                cout << "You don't have this tile in your hand!" << endl;
                return placeResult;
            }
            if (board[x][y] != ' ') {
                placeResult.isValidCommand = false;
                cout << "Position is occupied!" << endl;
                return placeResult;
            }

            placeResult.tile.letter = cmd[1][0];
            placeResult.tile.value = 0;
            placeResult.x = cmd[3][0] - 'A';
            placeResult.y = y;
            placeResult.isValidCommand = true;
            return placeResult;
        }

        return placeResult;
    }

    bool placeTile(vector<string> &command) {
        string cmd;
        bool isPlaceContinue = true;
        bool isThisTurnEnd = false;
        int PlacedTilesCount = 0;
        tilesPlacedThisTurn.clear();
        while (isPlaceContinue) {
            while (!isThisTurnEnd) {
				if (command.size() == 2 && command[0] == "place" && command[1] == "Done") {
                    isThisTurnEnd = true;
                    isPlaceContinue = false;
                }
                else
                {
					PlaceTileInfo pi = placeCommandValidCheck(command);
					if (pi.isValidCommand) {
						PlacedTilesCount++;
						tilesPlacedThisTurn.push_back(pi);
						isThisTurnEnd = true;
						if (PlacedTilesCount == MAX_TILES_COUNT_IN_HAND) {
							isPlaceContinue = false;
							isThisTurnEnd = true;
						}
					} else {
						cmd = getUserCommand();
						if (cmd.empty() && !inFilePath.empty()) {
							isPlaceContinue = false;
							isThisTurnEnd = true;
						}
						strSplit(cmd, command);
					}
                }
            }
            if (isPlaceContinue) {
                cmd = getUserCommand();
                strSplit(cmd, command);
                isThisTurnEnd = false;
                if (cmd.empty() && !inFilePath.empty()) {
                    isPlaceContinue = false;
                }
            } else {
                cout << endl;
            }
        }

        if (tilesPlacedThisTurn.size() > 1) {
            int x = tilesPlacedThisTurn[0].x;
            int y = tilesPlacedThisTurn[0].y;
            bool isSameRow = true;
            bool isSameCol = true;
            for (int i = 1; i < static_cast<int>(tilesPlacedThisTurn.size()); i++) {
                if (tilesPlacedThisTurn[i].x != x) {
                    isSameRow = false;
                }
				if (tilesPlacedThisTurn[i].y != y) {
                    isSameRow = false;
				}
            }

            bool const result = isSameRow || isSameCol;
            if(!result)
            {
                cout << "tiles should be in same row or col" << endl;
                return false;
            }
        }

        bool isCenterOccupied = false;
        if (isBoardEmpty()) {
            if (tilesPlacedThisTurn.size() <= 1) {
                cout << "You must place at least 2 tiles on the center!" << endl << endl;
                return false;
            }
            for (auto &i: tilesPlacedThisTurn) {
                if (i.x == 7 && i.y == 7) {
                    isCenterOccupied = true;
                }
            }
        } else {
            isCenterOccupied = true;
            bool isTouchOtherTile = false;
            for (auto &i: tilesPlacedThisTurn) {
                if (isAdjacentWithOtherTile(i.x, i.y)) {
                    isTouchOtherTile = true;
                }
            }
            if (!isTouchOtherTile) {
                return false;
            }
        }

        if (!isCenterOccupied) {
            return false;
        }

        for (auto const &i: tilesPlacedThisTurn) {
            board[i.x][i.y].letter = i.tile.letter;
            currentPlayer->handTiles->removeByLetter(i.tile.letter);
            currentPlayer->score += getLetterValue(static_cast<char>(toupper(i.tile.letter)));
        }
        if (PlacedTilesCount == MAX_TILES_COUNT_IN_HAND) {
            cout << "BINGO!!!" << endl;
            currentPlayer->score += BINGO_SCORE;
        }

        currentPlayer->drawTiles(tilesBag);
        return true;
    }


    bool placeTileFromFile(vector<string> &command, ifstream &f) {
        string cmd;
        bool isPlaceContinue = true;
        bool isThisTurnEnd = false;
        int PlacedTilesCount = 0;
        tilesPlacedThisTurn.clear();
        while (isPlaceContinue) {
            while (!isThisTurnEnd) {
                if (command.size() == 2 && command[0] == "place" && command[1] == "Done") {
                    isThisTurnEnd = true;
                    isPlaceContinue = false;
                }
                else
                {
                    PlaceTileInfo pi = placeCommandValidCheck(command);
                    if (pi.isValidCommand) {
                        PlacedTilesCount++;
                        tilesPlacedThisTurn.push_back(pi);
                        isThisTurnEnd = true;
                        if (PlacedTilesCount == MAX_TILES_COUNT_IN_HAND) {
                            isPlaceContinue = false;
                            isThisTurnEnd = true;
                        }
                    } else {
                        cmd = getUserCommandFromFile(f);
                        if (cmd.empty() && !inFilePath.empty()) {
                            isPlaceContinue = false;
                            isThisTurnEnd = true;
                        }
                        strSplit(cmd, command);
                    }
                }
            }
            if (isPlaceContinue) {
                cmd = getUserCommandFromFile(f);
                strSplit(cmd, command);
                isThisTurnEnd = false;
                if (cmd.empty() && !inFilePath.empty()) {
                    isPlaceContinue = false;
                }
            } else {
                cout << endl;
            }
        }

        if (tilesPlacedThisTurn.size() > 1) {
            int x = tilesPlacedThisTurn[0].x;
            int y = tilesPlacedThisTurn[0].y;
            bool isSameRow = true;
            bool isSameCol = true;
            for (int i = 1; i < static_cast<int>(tilesPlacedThisTurn.size()); i++) {
                if (tilesPlacedThisTurn[i].x != x) {
                    isSameRow = false;
                }
                if (tilesPlacedThisTurn[i].y != y) {
                    isSameRow = false;
                }
            }

            bool const result = isSameRow || isSameCol;
            if(!result)
            {
                cout << "tiles should be in same row or col" << endl;
                return false;
            }
        }

        bool isCenterOccupied = false;
        if (isBoardEmpty()) {
            if (tilesPlacedThisTurn.size() <= 1) {
                cout << "You must place at least 2 tiles on the center!" << endl << endl;
                return false;
            }
            for (auto &i: tilesPlacedThisTurn) {
                if (i.x == 7 && i.y == 7) {
                    isCenterOccupied = true;
                }
            }
        } else {
            isCenterOccupied = true;
            bool isTouchOtherTile = false;
            for (auto &i: tilesPlacedThisTurn) {
                if (isAdjacentWithOtherTile(i.x, i.y)) {
                    isTouchOtherTile = true;
                }
            }
            if (!isTouchOtherTile) {
                return false;
            }
        }

        if (!isCenterOccupied) {
            return false;
        }

        for (auto const &i: tilesPlacedThisTurn) {
            board[i.x][i.y].letter = i.tile.letter;
            currentPlayer->handTiles->removeByLetter(i.tile.letter);
            currentPlayer->score += getLetterValue(static_cast<char>(toupper(i.tile.letter)));
        }
        if (PlacedTilesCount == MAX_TILES_COUNT_IN_HAND) {
            cout << "BINGO!!!" << endl;
            currentPlayer->score += BINGO_SCORE;
        }

        currentPlayer->drawTiles(tilesBag);
        return true;
    }

    void nextPlayer() {
        // find current player's index
        int currentPlayerIndex = 0;
        for (int i = 0; i < static_cast<int>(playerList.size()); i++) {
            if (playerList[i] == currentPlayer) {
                currentPlayerIndex = i;
            }
        }
        int nextPlayerIndex = (currentPlayerIndex + 1) % playerList.size();
        currentPlayer = playerList[nextPlayerIndex];
    }

    void replaceTile(char c) const {
        if (currentPlayer->handTiles->isLetterInList(c) && !tilesBag->isEmpty()) {
            currentPlayer->handTiles->removeByLetter(c);
            currentPlayer->drawTiles(tilesBag);
        } else {
            cout << "You don't have this letter in your hand! Or Tiles Bag is Empty" << endl;
        }
    }

    void processCommand() {
        bool isRunning = true;
        bool isInvalidCommand = false;
        while (isRunning) {
            if (!isInvalidCommand) {
                showCurrentPlayerInfo();
            }
            isInvalidCommand = false;
            string cmd = getUserCommand();
            vector<string> words;
            strSplit(cmd, words, ' ');
            if (words.empty()) {
                return;
            }
            // check command
            string command = words[0];
            if (command == "place") {
                bool r = placeTile(words);
                if (r) {
                    nextPlayer();
                }
            } else if (command == "replace" && words.size() == 2) {
                replaceTile(words[1][0]);
                nextPlayer();
            } else if (command == "pass") {
                currentPlayer->drawTiles(tilesBag);
                if (tilesBag->isEmpty()) {
                    currentPlayer->passTimes++;
                }
                nextPlayer();
            }  else if (command == "save") {
                saveGame(words[1]);
            } else if (command == "quit") {
                isRunning = false;
                quit();
            } else if (command == "help") {
                HelpFunc();
            } else {
                cout << "Invalid Input!" << endl;
                cout << "command was not found within the system" << endl;
                cout << "" << endl;
                cout << "If not sure what command you could execute at the moment," << endl;
                cout << "please type 'help' to see all the available commands" << endl;
            }
            if (isGameEnd()) {
                cout << "Game over" << endl;
                showScores();
                showGameResult();
                quit();
            }
        }
    }

    void HelpFunc() {
        cout << "" << endl;
        cout << "If you are in the main menu:" << endl;
        cout << "Please enter command in digits between 1 ~ 4" << endl;
        cout << "" << endl;
        cout << "If you are in the game:" << endl;
        cout << "Enter the following commands to do such action: " << endl;
        cout << "place - place 'your letter' at 'the location'" << endl;
        cout << "place Done - once finished placing tiles, enter this command to finish placing" << endl;
        cout << "replace - replace 'a letter in your hand'" << endl;
        cout << "pass - pass this round to the next person" << endl;
        cout << "save - save the game" << endl;
        cout << "quit - quit the game" << endl;
        cout << "" << endl;
    }

    string getUserCommandFromFile(ifstream &f) {
        string line;
        getline(f, line, '\n');
        return line;
    }


    void processCommandFromFile(ifstream &f) {
        bool isRunning = true;
        bool isInvalidCommand = false;
        while (isRunning) {
            if (!isInvalidCommand) {
                showCurrentPlayerInfo();
            }
            isInvalidCommand = false;
            string cmd = getUserCommandFromFile(f);
            vector<string> words;
            strSplit(cmd, words, ' ');
            if (words.empty()) {
                return;
            }
            // check command
            string command = words[0];
            if (command == "place") {
                bool r = placeTileFromFile(words, f);
                if (r) {
                    //nextPlayer();
                    saveGame(outFilePath);
                    return;
                }
            } else if (command == "replace" && words.size() == 2) {
                replaceTile(words[1][0]);
				saveGame(outFilePath);
				return;
            } else if (command == "pass") {
                currentPlayer->drawTiles(tilesBag);
                if (tilesBag->isEmpty()) {
                    currentPlayer->passTimes++;
                }
                nextPlayer();
            }  else if (command == "save") {
                saveGame(words[1]);
            } else if (command == "quit") {
                isRunning = false;
                quit();
            } else if (command == "help") {
                HelpFunc();
            } else {
                cout << "Invalid Input!" << endl;
                cout << "command was not found within the system" << endl;
                cout << "" << endl;
                cout << "If not sure what command you could execute at the moment," << endl;
                cout << "please type 'help' to see all the available commands" << endl;
                isInvalidCommand = true;
            }
            if (isGameEnd()) {
                cout << "Game over" << endl;
                showScores();
                showGameResult();
                quit();
            }
        }
    }

    bool isGameEnd() const {
        if (tilesBag->isEmpty()) {
            for(auto const player : playerList) {
                if (!player->handTiles->isEmpty() || player->passTimes == 2) {
                    return true;
                }
            }
        }
        return false;
    }

    void newGame() {
        getUserName();
        cout << "Let's Play!" << endl << endl;
        processCommand();
    }

    static inline bool exists(const std::string &name) {
        std::ifstream f(name.c_str());
        return f.good();
    }

    void setCurrentPlayerByName(string name) {
        for(auto p : playerList) {
            if (p->name == name) {
                currentPlayer = p;
                return;
            }
        }
    }


    bool loadGame(const string &fileName = "./save.txt", bool isNeedProcessCommand=true) {
        // if file not exist
        if (!exists(fileName)) {
            cout << "File not exist!" << endl;
            return false;
        }
        ifstream file(fileName);
        for(auto &player : playerList) {
            player->resetPlayer();
        }
        try {
            if (file.is_open()) {
                string line;
                for(auto const &player : playerList) {
                    getline(file, line);
                    player->setName(line);
                    getline(file, line);
                    player->setScore(stoi(line));
                    getline(file, line);
                    player->handTiles->loadFromString(line);
                }
                loadBoard(file);
                getline(file, line);
                tilesBag->loadFromString(line);
                getline(file, line);
                setCurrentPlayerByName(line);
                if (!inFilePath.empty()) {
                    while (getline(file, line)) {
                        commandListLoaded.push_back(line);
                    }
                } else {
                    cout << "Game loaded successfully!" << endl;
                    file.close();
                }
            }
        } catch (const std::exception &e) {
            cout << "Invalid file!" << endl;
            return false;
        }
        if(isNeedProcessCommand)
        {
			processCommand();
        }
        return true;
    }



    string getUserCommand(bool fromCommandLine = true) {
        char cmd[100] = {0};
        if(fromCommandLine)
        {
            cin.clear();
			cout << "> ";
			cin.getline(cmd, sizeof(cmd));
            isEOFPressed(cmd);
        }

        return string(cmd);
    }

    int mainMenu() {
        int choice;
        do
	    {
            printFileContentByFileName("mainMenu.txt");
		    const string command = getUserCommand();
            if(ifStringIsDigit(command))
            {
				choice = stringToInt(command);
				if (choice < 1 || choice > 4) {
					cout << "Invalid input! The input should be digit between 1 ~ 4" << endl;
                    cout << "If more information needed, please type 'help' to see all the commands" << endl;
                    choice = -1;
				}
            }
            else if (command == "help"){
                HelpFunc();
                choice = -1;
			} else {
                cout << "Invalid input! The input should be in digits" << endl;
                cout << "If more information needed, please type 'help' to see all the commands" << endl;
            }
	    } while (choice == -1);

        return choice;
    }

    void play(bool isLoadGame = false) {
        bool stop = false;
        while (!stop) {
            int c = mainMenu();
            if (c == 1) {
                newGame();
            } else if (c == 2) {
                string fn;
                cout << "please input the file name to load: ";
                fn = getUserCommand();
                if (!loadGame(fn)) {
                    c = -1;
                }
            } else if (c == 3) {
                Credits();
            }
            else if (c == 4) {
                quit();
                stop = true;
            }
        }
    }

    // board related
    inline bool isPositionInBoard(int x, int y) const {
        return x >= 0 && x < row && y >= 0 && y < col;
    }

    bool isBoardEmpty() {
        vector< vector<Tile> >::iterator row;
        vector<Tile>::iterator col;
        for (row = board.begin(); row != board.end(); row++) {
            for (col = row->begin(); col != row->end(); col++) {
                if (!col->isEmpty()) {
                    return false;
                }
            }
        }
        return true;
    }


    static void printMarginLeft(char RowIndexLetter) {
        if(RowIndexLetter == '*')
        {
            RowIndexLetter = ' ';
        }
        cout << "  " << RowIndexLetter << "  ";
    }

    static void printRow(int rowIndex, const vector<Tile> &row) {
        printMarginLeft(static_cast<char> (rowIndex + 'A'));
        cout << "|";
        int index = 0;
        for (auto i: row) {
            cout << " " << i.letter << " |";
            index++;
        }
        cout << endl;
    }

    static void skipLine(ifstream &f, int skipCount = 1) {
        for (int i = 0; i < skipCount; ++i) {
            f.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }

    void loadBoard(ifstream &f) {
        string line;
        for (int i = 0; i < row; ++i) {
            getline(f, line, '\n');
            vector<string> v;
            strSplit(line, v, ' ');
            for (int j = 1; j < 16; j++) {
                if(!v[j].empty())
                {
					if (v[j] == "*") {
						board[i][j - 1].letter = ' ';
					} else {
						board[i][j - 1].letter = v[j][0];
					}
                }
            }
        }
        cout << "";
    }

    void showBoard() {
        printMarginLeft(' ');
        for (int i = 0; i < BOARD_SIZE; i++) {
            cout << setw(3) << i << " ";
        }
        cout << endl;
        printMarginLeft('*');
        for (int i = 0; i < BOARD_SIZE; i++) {
            cout << "----";
        }
        cout << "-" << endl;
        for (int i = 0; i < static_cast<int>(board.size()); i++) {
            printRow(i, board[i]);
        }
        printMarginLeft('*');
        for (int i = 0; i < BOARD_SIZE; i++) {
            cout << "----";
        }
        cout << "-" << endl;
    }

    bool isAdjacentWithOtherTile(int x, int y) {
        for (int i = x - 1; i <= x + 1; i++) {
            for (int j = y - 1; j <= y + 1; j++) {
                if(i == x && j == y) {
                    continue;
                }
                if(isPositionInBoard(i, j) && !board[i][j].isEmpty()) {
                    return true;
                }
            }
        }
        return false;
    }

    void loadTestFile(string fileName)
    {
        loadGame(fileName, false);
        ifstream f(fileName);
        skipLine(f, 23);
        // read file
        processCommandFromFile(f);
    }
};

int main(int argc, char *argv[]) {
    srand(time(nullptr));
    Scrabble game = Scrabble();
    string ifile = "save.txt";
    string ofile = "result.txt";
    //argc = 3;

    if (argc == 3) {
//        game.setTestFilePath(argv[1], argv[2]);
//        game.setTestFilePath(ifile, ofile);
//        game.loadTestFile(argv[1]);
        game.loadTestFile(ifile);
    }
	else if(argc == 1) {
        game.play();
    }
    else {
        cout << "Invalid number of arguments" << endl;
        return 1;
    }
    return EXIT_SUCCESS;
}
