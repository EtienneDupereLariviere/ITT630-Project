#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <math.h>

using namespace std;

/* Struc declaration */
struct Pos {
    int line, column;
    Pos(int l = 0, int c = 0) {
        line = l;
        column = c;
    }
};

class Cell {
    private:
        void SetPeersToCell(int, int, int);
        void SetSquarePeers(int, int, int);
		void SetLinePeers(int, int, int);
		void SetColumnPeers(int, int, int);
    public:
        string value;
        vector<Pos> peers;
        bool found = false;

        Cell() {};
        Cell(const Cell&);
        Cell(string, int, int, int);
};