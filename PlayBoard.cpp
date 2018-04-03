#include "game2048.h"

class PlayBoard : public Board {
private:
    Player* player;
    string prompt;

    // convert squares to string for saving into sqlite3
    string convertSquaresToString() {
        Square* grid = getSquares();
        string squares = "";
        int size = getSize();
        for (int i = 0; i < size*size; i++) {
            squares += player->int2str(grid[i].getValue()) + ",";
        }
        return squares;
    }

protected:
    void save() {
        Board::save();
        string squaresStr = convertSquaresToString();
        player->setScore(getScore());
        player->setHighScore(getHighScore());
        player->setSquares(squaresStr);
        if (player->isNewPlayer()) {
            player->connectToDB();
            player->insertPlayerData();
            player->closeConnect();
        } else {
            player->connectToDB();
            player->updatePlayerData();
            player->closeConnect();
        }
        prompt = "-----------Saved!------------";
    }

    void load() {
        Board::load();
        this->setScore(player->getScore());
        this->setHighScore(player->getHighScore());
        this->setSquaresByString(player->getSquares());
        prompt = "-----------Loaded!-----------";
    }

public: 
    PlayBoard() {
        player = new Player();
    }
    PlayBoard(string name) {
        player = new Player(name);
        if (!player->isNewPlayer()) {
            load();
        }
    }
    ~PlayBoard() {
        delete player;
    }
    void play() {
        while (true) {
            if (isMoved()) {
                addSquares();
            }
            draw();
            if (isDone() || isWin()) {
                prompt = isDone() ? "Game Over!" : "Winner is " + player->getName();
            }
            cout << prompt << endl;
            prompt = "";
            keyDirIn();
        }
    }
};

int main() {
    string name;
    cout << "Enter your name: " << endl;
    cin >> name;
    PlayBoard* board = new PlayBoard(name);
    board->play();

    delete board;
    return 0;
}