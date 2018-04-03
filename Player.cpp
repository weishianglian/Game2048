#include "game2048.h"

typedef vector<vector<string> > results_type;

class Player {
private:
    string name, squares;
    int score, highScore;
    bool isNew;

    sqlite3* db;
    char* errorMsg;
    char* sql;
    int resultCodes;
    const char* data;
    results_type results;

    void checkQueryResults(int, string);
    bool resultsContains(results_type&, string);
    static int callback(void*, int, char**, char**);
    results_type getResults();

public: 
    // constructor with default value
    Player() : name("Player"), score(0), highScore(0), squares(""), isNew(true), errorMsg(0), data("Callback function called") {
        errorMsg = new char[65535];
        this->connectToDB();
        this->checkPlayerTable();
        this->closeConnect();
    }
    Player(string name) : score(0), highScore(0), squares(""), isNew(true), errorMsg(0), data("Callback function called") {
        errorMsg = new char[65535];
        this->name = name;
        this->connectToDB();
        this->checkPlayerTable();
        this->queryPlayerByName(name);
        this->closeConnect();
    }
    ~Player() {
        delete [] errorMsg;
    }
    string int2str(int);
    void setName(string);
    string getName();
    void setSquares(string);
    string getSquares();
    void setScore(int);
    void setScore(string);
    int getScore();
    void setHighScore(int);
    void setHighScore(string);
    int getHighScore();
    bool isNewPlayer();
    void connectToDB();
    void checkPlayerTable();
    void insertPlayerData();
    void queryPlayerByName(string);
    void updatePlayerData();
    void closeConnect();
};

// use different function declaration
void Player::setName(string name) {
    this->name = name;
}
string Player::getName() {
    return this->name;
}
void Player::setSquares(string squares) {
    this->squares = squares;
}
string Player::getSquares() {
    return this->squares;
}
void Player::setScore(int score) {
    this->score = score;
}
void Player::setScore(string scoreStr) {
    istringstream buffer(scoreStr);
    int score;
    buffer >> score;
    this->score = score;
}
int Player::getScore() {
    return this->score;
}
void Player::setHighScore(int highScore) {
    this->highScore = highScore;
}
void Player::setHighScore(string highScoreStr) {
    istringstream buffer(highScoreStr);
    int highScore;
    buffer >> highScore;
    this->highScore = highScore;
}
int Player::getHighScore() {
    return this->highScore;
}
bool Player::isNewPlayer() {
    return this->isNew;
}
results_type Player::getResults() {
    return this->results;
}

string Player::int2str(int integer) {
    stringstream ss;
    ss << integer;
    string str = ss.str();
    return str;
}

void Player::checkQueryResults(int resultCodes, string msg) {
    if (resultCodes != SQLITE_OK) {
        cout << "SQL error: " << errorMsg << endl;
        sqlite3_free(errorMsg);
    } else {
        // cout << msg << endl;
    }
}

bool Player::resultsContains(results_type& vec, string str) {
    if(!vec.empty()){
        for (int i = 0; i < vec.size(); i++) {
            vector<string> row = vec[i];
            for (int j = 0; j < row.size(); j++) {
                if (row[j] == str) {
                    return true;
                }
            }
        }
    }
    return false;
}

int Player::callback(void *ptr, int count, char **value, char **colName) {
    results_type* results = static_cast<results_type*>(ptr);
    vector<string> row;
    for(int i = 0; i < count; i++) {
        string column = colName[i];
        row.push_back(value[i] ? value[i] : "NULL");
        cout << column << " = " << (value[i] ? value[i] : "NULL") << endl;
        if ((i+1)%4 == 0) {
            results->push_back(row);
            row.clear();
        }
    }
    return 0;
}

void Player::connectToDB() {
    resultCodes = sqlite3_open("2048.db", &db);

    if (resultCodes) {
        cerr << "Can't open database: " << sqlite3_errmsg(db) << endl;
    } else {
        // cout << "Opened database successfully" << endl;
    }
}

void Player::checkPlayerTable() {
    string isTableExist = "SELECT name FROM sqlite_master WHERE type='table' AND name='player';";
    sql = new char[isTableExist.length()];
    strcpy(sql, isTableExist.c_str());
    resultCodes = sqlite3_exec(db, sql, callback, &results, &errorMsg);
    delete sql;
    checkQueryResults(resultCodes, "Check Table");
    // if player table is not exist then create table
    if (!resultsContains(results, "player")) {
        string sqlStr = "CREATE TABLE player(\
                name TEXT PRIMARY KEY NOT NULL,\
                high_score INT,\
                score INT,\
                squares TEXT);";

        sql = new char[sqlStr.length()];
        strcpy(sql, sqlStr.c_str());

        resultCodes = sqlite3_exec(db, sql, callback, &results, &errorMsg);

        delete sql;
        checkQueryResults(resultCodes, "Create Table");
    }
}

void Player::queryPlayerByName(string name) {
    string sqlStr = "select * from player where name = ";
    string queryName = name.empty() ? "'';" : "'" + name + "';";
    sqlStr += queryName;
    sql = new char[sqlStr.length()];
    strcpy(sql, sqlStr.c_str());

    resultCodes = sqlite3_exec(db, sql, callback, &results, &errorMsg);
    if (resultsContains(results, name)) {
        this->setName(results[0][0]);
        this->setHighScore(results[0][1]);
        this->setScore(results[0][2]);
        this->setSquares(results[0][3]);
        this->isNew = false;
    }

    delete sql;
    checkQueryResults(resultCodes, "Select Data");
}

void Player::insertPlayerData() {
    string playerData = "'" + name + "'," + int2str(highScore) + "," + int2str(score) + ",'" + squares + "'";
    string sqlStr = "insert into player(name, high_score, score, squares)\
            values(" + playerData + ");";
    sql = new char[sqlStr.length()];
    strcpy(sql, sqlStr.c_str());

    resultCodes = sqlite3_exec(db, sql, callback, 0, &errorMsg);

    delete sql;
    checkQueryResults(resultCodes, "Insert Data");
}

void Player::updatePlayerData() {
    string sqlStr = "update player set high_score=" + int2str(highScore) + ", score=" + int2str(score) + 
                    ", squares='" + squares + "' where name = '" + name + "'";

    sql = new char[sqlStr.length()];
    strcpy(sql, sqlStr.c_str());
    resultCodes = sqlite3_exec(db, sql, callback, (void*)data, &errorMsg);

    delete sql;
    checkQueryResults(resultCodes, "Update Data");
}

void Player::closeConnect() {
    sqlite3_close(db);
    results.clear();
}