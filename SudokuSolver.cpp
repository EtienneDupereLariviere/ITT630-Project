#include <iostream>
#include <fstream>
#include <cmath>

#define SUDOKU_SIZE 9

using namespace std;

int sudokuBoard[SUDOKU_SIZE][SUDOKU_SIZE];
int un = 0;

void printBoard();
void fillBoard();

int main() {
	int r,c,tmp;
	bool b = true;

	fillBoard();

	printBoard();
	
	return 0;
}

/* Function to fill the board */
void fillBoard() {
	const char* filename = "sudoku.txt";
	ifstream input(filename);
	
	if (!input.is_open()) {
		cout << "Problem with the file\n";
		return;
	}
	
	for (int i = 0; i < SUDOKU_SIZE; ++i)
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			input >> sudokuBoard[i][j];
			if (!sudokuBoard[i][j])
				un++;
		}
	input.close();
}

/* Function to print the board */
void printBoard() {
	cout << "+-------+-------+-------+\n";

	for (int i = 0; i < SUDOKU_SIZE; ++i) {
		cout << "| ";
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			if (sudokuBoard[i][j] != 0) 
				cout << sudokuBoard[i][j];
			else
				cout << ".";
			cout << " ";
			if ((j + 1) % 3 == 0) cout << "| ";
		}
		if ((i + 1) % 3 == 0)
			cout << "\n+-------+-------+-------+";
		cout << "\n";
	}
}