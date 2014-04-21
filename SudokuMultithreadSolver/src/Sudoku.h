#include <string>

using namespace std;

class Sudoku {
   public:
		void initialiseGrid(string filePath);
		void printGrid();
		void launchThreads();
		void clearValue();
		
		struct threadParameters;
        int **grid;
		int emptyBlocks;
		int nbBlocks;
		bool notSolvable;
};