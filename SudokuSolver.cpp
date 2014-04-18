#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <vector>

#define SUDOKU_SIZE 9

using namespace std;

/* Struc declaration */
struct Pos {
    int line, column;
    Pos(int l = 0, int c = 0) {
        line = l;
        column = c;
    }
};

struct Cell {
    string value;
    vector<Pos> peers;
    bool found = false;
};

typedef vector< vector<Cell> > Board;

/* Function definition */
void PrintBoard(Board);
Board FillBoard();
Board EliminateFowardChecking(Board board, Pos, char);
Pos MinimumRemainingValues(Board);
char LeastConstraintValue(Board, Pos);
int GetMinElement(vector<int>);
Board BacktrackingSearch(Board);
bool IsComplete(Board);

/* Global variables */
//string Domain = "123456789";

/* Main */
int main() {
	int r,c,tmp;
	bool b = true;
	Board sudokuBoard;

	sudokuBoard = FillBoard();
	
	//sudokuBoard = BacktrackingSearch(sudokuBoard);

	PrintBoard(sudokuBoard);
	
	return 0;
}

/* Function to fill the board */
Board FillBoard() {
	Board board = Board(SUDOKU_SIZE, vector<Cell>(SUDOKU_SIZE));
	const char* filename = "sudoku.txt";
	ifstream input(filename);
	
	if (!input.is_open()) {
		cout << "Problem with the file\n";
		return Board();
	}
	
	for (int i = 0; i < SUDOKU_SIZE; ++i)
		for (int j = 0; j < SUDOKU_SIZE; ++j) {
			input >> board[i][j].value;
			board[i][j].found = true;
		}
	
	input.close();

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
			cout << "\n+-------+-------+-------+";
		cout << "\n";
	}
}

/* */
Board EliminateFowardChecking(Board board, Pos p, char val) {
	string value = board[p.line][p.column].value;
	board[p.line][p.column].value = value.replace(value.begin(), value.end(), string(1, val).c_str(), "");
	return board;
}

/*  */
Board AssignForwardChecking(Board board, Pos p, char val) {
	for (int i = 0; i < board[p.line][p.column].peers.size(); ++i) {
		string value = board[board[p.line][p.column].peers[i].line][board[p.line][p.column].peers[i].column].value;
		board[board[p.line][p.column].peers[i].line][board[p.line][p.column].peers[i].column].value =
			value.replace(value.begin(), value.end(), string(1, val).c_str(), "");
		if (board[board[p.line][p.column].peers[i].line][board[p.line][p.column].peers[i].column].value.length() == 0)
			return Board();
	}

	board[p.line][p.column].value = string(1, val).c_str();
	board[p.line][p.column].found = true;
	return board;
}

/* */
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

/* */
char LeastConstraintValue(Board board, Pos varPos) {
	vector<int> arr(board[varPos.line][varPos.column].value.length());

	for (int i = 0; i < arr.size(); ++i)
		arr[i] = 0;

	for (int i = 0; i < board[varPos.line][varPos.column].value.length(); ++i)
		for (int j = 0; j < board[varPos.line][varPos.column].peers.size(); ++j) {
			if (board[board[varPos.line][varPos.column].peers[j].line]
					 [board[varPos.line][varPos.column].peers[j].column].value
					 .find(board[varPos.line][varPos.column].value[i]))
				arr[i]++;
		}

	return board[varPos.line][varPos.column].value[GetMinElement(arr)];
}

/* Return the smallest element of a vector */
int GetMinElement(vector<int> element) {
	int min = 0;
	
	if(element.size() != 0) {
		min = element[0];

		for (int i = 0; i < element.size(); ++i)
			if (element[i] < min)
				min = element[i];
	}

	return min;
}

/* */
Board BacktrackingSearch(Board board) {
	Board local;

	//if (board == NULL) return Board();
	if (IsComplete(board)) return Board();

	Pos s = MinimumRemainingValues(board);

	while (board[s.line][s.column].value.length() > 0) {
		char c = LeastConstraintValue(board, s);

		local = BacktrackingSearch(AssignForwardChecking(board, s, c));

		//if (local != NULL)
		//	return local;
		board = EliminateFowardChecking(board, s, c);

		//if (board == NULL) return NULL;
	}
}

bool IsComplete(Board board) {
	for (int i = 0; i < SUDOKU_SIZE; ++i)
		for (int j = 0; j < SUDOKU_SIZE; ++j)
			if (board[i][j].found == false) return false;

	return true;
}

/*
MyCell[,] backtrackingSearch(MyCell[,] branch)
{
    MyCell[,] ret;

    if (branch == null) return null; 
    if (isFinish(branch)) return branch;

    Pos s = SelectUnassignedVariable(branch);

    while (branch[s.ln, s.col].Value.Length > 0)
    {

        char c = SelectDomainValue(branch, s);

        ret = backtrackingSearch(Assign(Clone(branch), s, c));

        if (ret != null)
            return ret;

        m_NumOfBacktracking++;
        branch = Eliminate(branch, s, c);

        if (branch == null) return null;
    }
*/