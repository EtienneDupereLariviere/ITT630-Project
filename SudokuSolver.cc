/*********************************
* IFT630 - Projet
*********************************/
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <vector>
#include <ctime>
#include "cell.h"

#define SUDOKU_SIZE 9
#define DOMAIN "123456789"

using namespace std;

typedef vector< vector<Cell> > Board;

/* Function definition */
void PrintBoard(Board);
Board FillBoard(const char*);
Board EliminateFowardChecking(Board, Pos, char);
Board AssignForwardChecking(Board, Pos, char);
Pos MinimumRemainingValues(Board);
char LessConstraintValue(Board, Pos);
int GetMinElement(vector<int>);
Board BacktrackingSearch(Board);
bool IsComplete(Board);

/* Global variables */
int timerStart = 0;
int timerDuration = 0;

/* Main */
int main() {
	const char* filename = "sudoku.in.txt";
	Board sudokuBoard;

	cout << "Initial sudoku : " << endl;
	sudokuBoard = FillBoard(filename);
	sudokuBoard = BacktrackingSearch(sudokuBoard);
	
	// Resolving duration
	timerDuration = (clock() - timerStart) / 1000;

	if (!sudokuBoard.empty()) {
		cout << "Sudoku resolved in " << timerDuration << "ms : " << endl;
		PrintBoard(sudokuBoard);
	}
	else {
		cout << "No solution found for this sudoku." << endl;
	}

	return 0;
}

/* Filling the board from a file */
Board FillBoard(const char* filename) {	
	Board board = Board(SUDOKU_SIZE, vector<Cell>(SUDOKU_SIZE));
	Board initBoard = Board(SUDOKU_SIZE, vector<Cell>(SUDOKU_SIZE));
	ifstream input(filename);
	
	if (!input.is_open()) {
		cout << "Problem opening the file '" << filename << "'"<< endl;
		return Board();
	}
	
	// Start the timer
	timerStart = clock();

	// Fill the board with possible value
	for (int i = 0; i < SUDOKU_SIZE; ++i)
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			input >> initBoard[i][j].value;
			board[i][j] = Cell(DOMAIN, i, j, SUDOKU_SIZE);
		}
	
	input.close();
	PrintBoard(initBoard);

	for (int i = 0; i < SUDOKU_SIZE; ++i)
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (initBoard[i][j].value != "0")
				board = AssignForwardChecking(board, Pos(i, j), initBoard[i][j].value[0]);
		}

	return board;
}

/* Function to print the board */
void PrintBoard(Board board) {
	cout << "+-------+-------+-------+\n";
	
	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		cout << "| ";
		
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (board[i][j].value != "0") 
				cout << board[i][j].value;
			else
				cout << ".";
			cout << " ";
			if ((j + 1) % 3 == 0) cout << "| ";
		}
		
		if ((i + 1) % 3 == 0)
			cout << endl << "+-------+-------+-------+";
		
		cout << endl;
	}
}

/* Recursive process that resolve sudoku */
Board BacktrackingSearch(Board board) {
	Board local;
	if (board.empty()) return Board();
	if (IsComplete(board)) return board;

	// Finding next position to look at
	Pos s = MinimumRemainingValues(board);

	// While the position's values are not empty
	while (board[s.line][s.column].value.length() > 0) {
		// Finding next value to test
		char c = LessConstraintValue(board, s);

		local = BacktrackingSearch(AssignForwardChecking(board, s, c));

		if (!local.empty())
			return local;
		// If the local board is empty, the tested value is eliminated
		board = EliminateFowardChecking(board, s, c);

		if (board.empty()) return Board();
	}
	return Board();
}

/* Erase the value from a specific position in the board */
Board EliminateFowardChecking(Board board, Pos p, char val) {
	board[p.line][p.column].value.erase(board[p.line][p.column].value.find(val), 1);
	return board;
}

/* Erase peers values from possible value of a position in the board.
* If an erase ends with a peer without possible value, the process ends
* and an empty board is returned
*/
Board AssignForwardChecking(Board board, Pos p, char val) {
	for (int i = 0; i < board[p.line][p.column].peers.size(); ++i) {
		Pos peer = board[p.line][p.column].peers[i];
		int index = board[peer.line][peer.column].value.find(val);
		
		if (index != -1)
			board[peer.line][peer.column].value.erase(index, 1);
		
		if (board[peer.line][peer.column].value.length() == 0)
			return Board();
	}

	board[p.line][p.column].value = string(1, val).c_str();
	board[p.line][p.column].found = true;
	return board;
}

/* Return the position with minimum remaining values */
Pos MinimumRemainingValues(Board board) {
	int min = SUDOKU_SIZE + 1;
	Pos pos;

	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		for (int j = 0; j < SUDOKU_SIZE; ++j)
			if (!board[i][j].found && board[i][j].value.length() < min) {
				pos.line = i;
				pos.column = j;
				min = board[i][j].value.length();
			}
	}

	return pos;
}

/* Look for each value of a position in order to choose the one with less constraint */
char LessConstraintValue(Board board, Pos varPos) {
	vector<int> arr(board[varPos.line][varPos.column].value.length());

	for (int i = 0; i < board[varPos.line][varPos.column].value.length(); ++i)
		for (int j = 0; j < board[varPos.line][varPos.column].peers.size(); ++j) {
			Pos peer = board[varPos.line][varPos.column].peers[j];
			if (board[peer.line][peer.column].value.find(board[varPos.line][varPos.column].value[i]))
				arr[i]++;
		}

	return board[varPos.line][varPos.column].value[GetMinElement(arr)];
}

/* Return the index of the smallest element of a vector */
int GetMinElement(vector<int> element) {
	int min = 0, index = 0;
	
	if(element.size() != 0) {
		min = element[0];

		for (int i = 1; i < element.size(); ++i)
			if (element[i] < min) {
				min = element[i];
				index = i;
			}
	}

	return index;
}

/* Verify if all elements have been found */
bool IsComplete(Board board) {
	for (int i = 0; i < SUDOKU_SIZE; ++i)
		for (int j = 0; j < SUDOKU_SIZE; ++j)
			if (board[i][j].found == false) return false;

	return true;
}
