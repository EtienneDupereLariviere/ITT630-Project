#include "cell.h"

Cell::Cell(const Cell &c) {
    value = c.value;
    found = c.found;

    for (int i = 0; i < c.peers.size(); i++)
        peers.push_back(c.peers[i]);
}

Cell::Cell(string val, int line, int column, int boardSize)
{
	value = val;
    
    found = (value.length() == 1);

    SetPeersToCell(line, column, boardSize);
}

void Cell::SetPeersToCell(int line, int column, int boardSize)
{
    SetLinePeers(line, column, boardSize);
    SetColumnPeers(line, column, boardSize);
    SetSquarePeers(line, column, boardSize);
}

void Cell::SetSquarePeers(int line, int column, int boardSize)
{
    int dx = column - (column % (int)sqrt(boardSize));
    int dy = line - (line % (int)sqrt(boardSize));

    for (int i = dy; i < dy + (int)sqrt(boardSize); i++)
        for (int j = dx; j < dx + (int)sqrt(boardSize); j++)
        {
            if ((i == line) || (j == column)) continue;
			Pos pos(i, j);
            peers.push_back(pos);
        }
}

void Cell::SetColumnPeers(int line, int column, int boardSize)
{
    for (int i = 0; i < boardSize; i++)
    {
        if (i == line) continue;
		Pos pos(i, column);
		peers.push_back(pos);
    }
}

void Cell::SetLinePeers(int line, int column, int boardSize)
{
    for (int i = 0; i < boardSize; i++)
    {
        if (i == column) continue;
		Pos pos(line, i);
		peers.push_back(pos);
    }
}
