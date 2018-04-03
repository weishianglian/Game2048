#include "game2048.h"

class Square {
private:
    int value;
    bool locked;

public:
    Square() {
        value = 0;
        locked = false;
    }
    void setValue(int value) {
        this->value = value;
    }
    int getValue() {
        return this->value;
    }
    bool isEmpty() {
        return (this->value == 0);
    }
    void lock() {
        this->locked = true;
    }
    void unlock() {
        this->locked = false;   
    }
    bool isLocked() {
        return locked;
    }
    void newValue() {
        srand(time(NULL));
        this->value = (rand()%2+1)*2;
    }
    void addValue() {
        this->value *= 2;
    }
    void clean() {
        this->value = 0;
    }
};