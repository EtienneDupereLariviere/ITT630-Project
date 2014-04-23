
using namespace std;

class SubGrid {
   private:
		void initChoices(int nbBlocks, int **grid);
   public:
		struct Solution;
		Solution **solution;
		int nbSquare;
		int emptyBlocks;
		int x;
		int y;
		
		void initSubGrid(int threadNumber, int nbBlocks, int **grid);
		int checkBlock(int yLocal, int xLocal, int yGlobal, int xGlobal, int nbBlocks, int **grid);
		int getNaiveChoices(int yLocal, int xLocal, int yGlobal, int xGlobal, int nbBlocks, int **grid);
		int getSingletonChoices(int yLocal, int xLocal, int yGlobal, int xGlobal, int nbBlocks, int **grid);
		void clearValue(); 
};