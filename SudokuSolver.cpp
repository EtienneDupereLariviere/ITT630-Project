#include <iostream>
#include <fstream>
#include <cmath>

using namespace std;

int sudokuBoard[9][9];

int main(){
	int un = 0,r,c,tmp;
	bool b = true;
	const char* filename = "sudoku";
	ifstream input(filename);
	
	if(!input.isopen())
	{
		cout << "Problem with the file\n";
		return 0;
	}
	
	for(int i=0;i<9;++i)
		for(int j=0;j<9;++j)
		{
			input >> sudokuBoard[i][j];
			if(!sudokuBoard[i][j])
				un++;
		}
	input.close();
	
	return 0;
}