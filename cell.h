#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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
        void SetUnitsAndPeersToCell(int, int, int);
        void SetSquareUnitAndPeers(int, int, int);
        void SetRowUnitAndPeers(int, int, int);
        void SetLineUnitAndPeers(int, int, int);
    public:
        string value;
        vector<Pos> units[3];
        vector<Pos> peers;
        bool found = false;

        Cell() {};
        //Cell(const Cell&);
        Cell(string, int, int, int);
};