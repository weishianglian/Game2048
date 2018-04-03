#include "game2048.h"

class Board {
private:
    Square* grid;
    Square* backupGrid;
    int size, score, highScore, lastPlus, max;
    bool win, done, moved;
    enum moveDirection { Up, Down, Left, Right };

    void drawBorder(bool isBottom) {
        string borderUnit = "+------";
        for (int i = 0; i < size; i++) {
            cout << borderUnit;
        }
        if (isBottom) {
            cout << "+" << endl;
        } else {
            cout << "+" << endl << "| ";
        }
    }

    bool isEqualSquare(Square* s1, Square* s2) {
        return s1->getValue() == s2->getValue();
    }

    template<typename T>
    void squareSwap(T* s1, T* s2) {
        T temp = *s1;
        *s1 = *s2;
        *s2 = temp;
    }

    bool checkSpace() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                Square* sq = &grid[i*size+j];
                if (sq->isEmpty()) {
                    return true;
                }
                if (checkAdd(i+1, j, sq->getValue())) {
                    return true;
                } else if (checkAdd(i-1, j, sq->getValue())) {
                    return true;
                } else if (checkAdd(i, j+1, sq->getValue())) {
                    return true;
                } else if (checkAdd(i, j-1, sq->getValue())) {
                    return true;
                }
            }
        }
        return false;
    }

    bool checkAdd(int i, int j, int value) {
        if (i < 0 || i > size-1 || j < 0 || j > size-1) {
            return false;
        } else {
            return grid[i*size+j].getValue() == value;
        }
    }

    bool checkAdd() {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                Square* sq = &grid[i*size+j];
                if (checkAdd(i - 1, j, sq->getValue())) {
                    return true;
                } else if (checkAdd(i + 1, j, sq->getValue())) {
                    return true;
                } else if (checkAdd(i, j - 1, sq->getValue())) {
                    return true;
                } else if (checkAdd(i, j + 1, sq->getValue())) {
                    return true;
                }
            }
        }
        return false;
    }

    void moveHorizontally(int i, int j, int distance) {
        Square* s1 = &grid[i*size+j];
        Square* s2 = &grid[i*size+j+distance];
        // combime two squares
        if (!s2->isEmpty() && isEqualSquare(s1, s2) && !s1->isLocked() && !s2->isLocked()) {
            s1->clean();
            s2->addValue();
            score += s2->getValue();
            lastPlus = s2->getValue();
            highScore = highScore>score ? highScore : score;
            s2->lock();
            moved = true;
        } else if (s2->isEmpty() && !s1->isEmpty()) {
            squareSwap(s1, s2);
            moved = true;
        }

        if (distance > 0) {
            if (j + distance < size-1) {
                moveHorizontally(i, j + distance, 1);
            }
        } else {
            if (j + distance > 0) {
                moveHorizontally(i, j + distance, -1);
            }
        }
    }

    void moveVertically(int i, int j, int distance) {
        Square* s1 = &grid[i*size+j];
        Square* s2 = &grid[(i+distance)*size+j];

        // combime two squares
        if (!s2->isEmpty() && isEqualSquare(s1, s2) && !s1->isLocked() && !s2->isLocked()) {
            s1->clean();
            s2->addValue();
            score += s2->getValue();
            lastPlus = s2->getValue();
            highScore = highScore>score ? highScore : score;
            s2->lock();
            moved = true;
        } else if (s2->isEmpty() && !s1->isEmpty()) {
            squareSwap<Square>(s1, s2);
            moved = true;
        }

        if (distance > 0) {
            if (i + distance < size-1) {
                moveVertically(i + distance, j, 1);
            }
        } else {
            if (i + distance > 0) {
                moveVertically(i + distance, j, -1);
            }
        }
    }

    void move(moveDirection dir) {
        backup();
        switch (dir) {
            case Up:
                for (int i = 1; i < size; i ++) {
                    for (int j = 0; j < size; j++) {
                        if (!grid[i*size+j].isEmpty()) {
                            moveVertically(i, j, -1);
                        }
                    }
                }
                break;
            case Down:
                for (int i = size-2; i >= 0; i--) {
                    for (int j = 0; j < size; j++) {
                        if (!grid[i*size+j].isEmpty()) {
                            moveVertically(i, j, 1);
                        }
                    }
                }
                break;
            case Left:
                for (int i = 0; i < size; i++) {
                    for (int j = 1; j < size; j++) {
                        if (!grid[i*size+j].isEmpty()) {
                            moveHorizontally(i, j, -1);
                        }
                    }
                }
                break;
            case Right:
                for (int i = 0; i < size; i++) {
                    for (int j = size-2; j >= 0; j--) {
                        if (!grid[i*size+j].isEmpty()) {
                            moveHorizontally(i, j, 1);
                        }
                    }
                }
                break;
        }
    }
    void backup() {
        for (int i = 0; i < size; i ++) {
            for (int j = 0; j < size; j++) {
                backupGrid[i*size+j] = grid[i*size+j];
            }
        }
    }

    void undo() {
        score -= lastPlus;
        for (int i = 0; i < size; i ++) {
            for (int j = 0; j < size; j++) {
                grid[i*size+j] = backupGrid[i*size+j];
            }
        }
    }

    void restart() {
        score = 0;
        lastPlus = 0;
        done = false;
        win = false;
        delete [] grid;
        delete [] backupGrid;
        grid = new Square[size*size];
        backupGrid = new Square[size*size];
        addSquares();
    }

protected:
    int getSize() {
        return size;
    }

    Square* getSquares() {
        return this->grid;
    }

    void setSquaresByString(string squares) {
        istringstream ss(squares);
        string token;
        int value;
        for (int i = 0; i < size; i ++) {
            for (int j = 0; j < size; j++) {
                getline(ss, token, ',');
                stringstream str2int(token);
                str2int >> value;
                grid[i*size+j].setValue(value);
            }
        }
    }

    void setScore(int score) {
        this->score = score;
    }
    int getScore() {
        return this->score;
    }
    void setHighScore(int highScore) {
        this->highScore = highScore;
    }
    int getHighScore() {
        return this->score;
    }

    virtual void save() {
        done = false;
        win = false;
    }

    virtual void load() {
        done = false;
        win = false;
    }

public:
    Board(): size(4), score(0), highScore(0), lastPlus(0), win(false), done(false), moved(true) {
        grid = new Square[size*size];
        backupGrid = new Square[size*size];
    }
    ~Board() {
        delete [] grid;
        delete [] backupGrid;
    }
    bool isWin() {
        if (max == 2048) {
            win = true;
        }
        return this->win;
    }
    bool isDone() {
        if (!checkSpace()) {
            done = true;
        }
        return this->done;
    }
    bool isMoved() {
        return this->moved;
    }

    void draw() {
        system("clear");
        cout << "Score: " << score << "\t High Score: " << highScore << endl;
        for (int i = 0; i < size; i++) {
            drawBorder(false);
            for (int j = 0; j < size; j++) {
                Square* sq = &grid[i*size+j];
                if (sq->isEmpty()) {
                    cout << setw(4) << " ";
                } else {
                    cout << setw(4) << sq->getValue();
                }
                cout << " | ";
            }
            cout << endl;
        }
        drawBorder(true);
    }

    void keyDirIn() {
        cout << "(w)Up (s)Down (a)Left (d)Right" << endl; 
        cout << "      (u)Undo (r)Restart" << endl;
        cout << "      (v)Save (l)Load" << endl;
        moved = false;
        char input[1];
        cin >> input;
        switch (input[0]) {
            case 'w': 
                move(Up);
                break;
            case 's': 
                move(Down);
                break;
            case 'a': 
                move(Left);
                break;
            case 'd': 
                move(Right);
                break;
            case 'u':
                undo();
                break;
            case 'r':
                restart();
                break;
            case 'v':
                save();
                break;
            case 'l':
                load();
                break;
        }
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                grid[i*size+j].unlock();
                if (grid[i*size+j].getValue() > max) {
                    max = grid[i*size+j].getValue();
                }
            }
        }
    }

    void addSquares() {
        srand(time(NULL));
        int frequency = rand()%2+1; // 1~2
        int k, count = 0;
        vector<Square*> emptyVector;
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (grid[i*size+j].isEmpty()) {
                    emptyVector.push_back(&grid[i*size+j]);
                    count += 1;
                }
            }
        }
        if (count == 1) {
            frequency = 1;
        }
        if (count > 0 && count >= frequency) {
            do {
                k = rand()%emptyVector.size();
                random_shuffle(emptyVector.begin(), emptyVector.end());
                emptyVector.back()->newValue();
                emptyVector.pop_back();
                frequency--;
            } while(frequency > 0);
        }
    }
};