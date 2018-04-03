# Game 2048
## Introduction
My 2048 game is played on a 4x4 grid (16 squares) and started with 1 or 2 randomly placed numbers. The player uses the four arrow keys(up, down, left, right) to move the squares, and 2 squares of the same number will be merged and added up. Every move after the player making, one or two new squares with a value either 2 or 4 will randomly appear in the empty squares. Once the number 2048 made in a square, the player wins the game. On the contrary, if there is no space to let the new squares be placed and every square and its neighbours is not equal value, the player lose the game.  

In the terminal, it shows the grid and squares on the screen and there are some command such as move directions, undo, restart, save and load. The player can key in the character corresponded to its correspond abbreviation like (w), (s), (a), (d), (u), (r), (v), (l) by clicking enter. And there is a display message between the board and the control panel. Before staring the game, the player need to enter your for record your score and results, once the player save the game and it can be loaded any time when key in 'l'. Even the game reopened, when the player types the previous name, the game can be continued from previous record saved by the player.

![fig1](images/fig1)  
Fig 1. Showing the message: saved

![fig2](images/fig2)  
Fig 2. Showing the message: win

![fig3](images/fig3)  
Fig 3. Showing the message: game over

## Approach
Because I played 2048 before, I find that this game are composed of a grid board, squares with values and a player. Further, the board loads squares so that I represent my board as a array containing with square and implement a function to receive any input from the player. Moreover, I implement different functions to check any two clashed squares can either be merged or not in every turn after moving and validate the squares' number on the board to check if the player wins the game.  

## Issues Faced
The aim of my coursework is making this 2048 game be a object-oriented programme. Therefore, I implement this game with creating a "Square" class which contains two variables and their mutator function to control changes. Secondly, I create a "Board" class to do mainly operations of this game such as drawing the grid on the terminal, detecting input from the player and  implementing the logic of this game. Thirdly, I create a "Player" class which contains the player's name and score. Moreover, I implement a database connection using sqlite3 to save or load the game record include CRUD manipulation. Finally, I create a "PlayBoard" class to inherit "Board" class for combining "Player" class to implement save/load functions. Furthermore, I create a header called "2048.h" included lots of STL library to complete this game.

![fig4](images/fig4)
Fig 4. The Class Diagram (UML) of my programme structure  

## Function Explanation  
Firstly, I use lots of mutator functions which are used to control changes to variables such as getter and setter for OOP so that I will not explain this part because they are simple functions.  

In the Board class:
* drawBorder: Draw the borders of the board according to the grid size.
* isEqualSquare: Compare two squares which are the same value.
* squareSwap: Swap position between empty square and square with value.
* checkSpace: Check every place on the grid with empty square or square can be added up with its neighbours.
* checkAdd: Check the square has the same value with its neighbours.
* moveHorizontally, moveVertically: Move every square horizontally or vertically and check there are enough space or the neighbour with same value can be combined.
* move: Move the squares according to the player input.
* backup: Backup the square values on the board for "undo" action.
* undo: Undo the operation to previous step.
* restart: Restart the game 
* setSquaresByString: Convert the format string to square values queried from table database.
* draw: Draw the grid board and show the squares on it.
* keyDirIn: Detect the player input and respond to correct operations.
* addSquares: Add 1 or 2 Squares with either 2 or 4 value randomly after every move.

In the Player class:
* int2str: Use string stream library for casting integer to string.
* checkQueryResults: Check query result by its result code and show the error message.
* resultsContains: Check the vector of the query results contains the string given from the argument.
* callback: Push back the query results to a vector for loading record from database.
* connectToDB: Open connection to database (2048.db).
* checkPlayerTable: Check if the player table exist or create a new table.
* queryPlayerByName: Use the player name, the primary key, to query the record in database.
* insertPlayerData: Insert the new player data when executing saving game.
* updatePlayerData: Update the exist player data when executing saving game.
* closeConnect: Close connection to database.

In the PlayBoard class:
* convertSquaresToString: Convert every square's value into a string for inserting the player data.
* save: Call the "Player" class method to save the record.
* load: Call the "Player" class method to load the record.
* play: Execute the loop followed the game logic.

## Limitations
Actually, I want to implement the input function intuitively when the player key in a character and then the operation executed directly without press enter key after keying. However, its seems that there are a library called "ncurse.h" can accomplish this part. Furthermore, I really confuse that how to use type casting correctly in c++, because there are too many functions to do the same thing and there is no precise way to solve it. Therefore, in my code, it can see that there are different method to convert types such as casting string to integer.  

## Test
Every time before I write some code to the major programme, I create a temporarily class called "test.cpp" to test the code can be executed. With a pure text editor, there is no assistance when I coding so that I need to solve lots of problems after compiling my programme with error messages. Moreover, it needs to test the game I created many times to check every logic operator is correct, and it may face infinite loops when running or getting stuck in all space but there is no message to tell the player that the game is over. Actually, it needs lots of time to debug when coding in a unfamiliar programming language.  

## Database Schema
* Database file: 2048.db
* Table: player
* Columns: name(text), high_score(int), score(int) and squares(text)
* Example data: XD|16704|16704|4,2,32,128,0,2,4,512,0,4,32,512,2,4,8,1024,

## Compilation and Code requirement
### Correctly compiling and executing code: 
* Compiling command:
    * g++ PlayBoard.cpp -o PlayBoard -l sqlite3
* Executing code:
    * ./PlayBoard

### Code structure: 
* Four classes: "Square", "Board", "Player" and "PlayBoard".
* "Square": replaces integer to do more operators.
* "Player": contains functions to connect database for implementing saving and loading games.
* "PlayBoard": inherit "Board" to combine "Player" for playing the game and recording the results.

### Logical use of variables and functions: 
* Declaring private variables and setting their accessors.
* Implementing logic functions to play the game and database connection to record the results.

### Logical and correct use of flow control structures: 
* Using "for loop" for iterating every square in the board for checking the game logic.
* Using "if else" statement to determine every operator.
* Using "switch case" statement to filter the user input.

### Use of arrays: 
* Using square arrays to do the major operation and backup array for undo function.
* Using vector to record the results from querying data in database manipulation.

### Use of classes (multiple classes): 
* Four classes: "Square", "Board", "Player" and "PlayBoard".

### Use of pointers and references: 
* Using pointer to swap squares' value.
* Using pointer and reference to implement sqlite3 API for querying data.
* Using reference to filter content in the query results.

### Correct memory management (no leaks, destructors): 
* Using destructor to delete declared variable by keyword "new".
* Using delete syntax after casting string to a char array for composing SQL. 

### Use of inheritance (single inheritance): 
* "PlayBoard": inherit "Board" to combine "Player" for playing the game and recording the results

### User input and output: 
* Using "cout" and "cin" to display the grid, every square's value and control panel.

File input and output: 
Using database sqlite3 in "Player" for saving and loading the game record.