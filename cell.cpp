#include "cell.h"

Cell::Cell(const Cell &cell) {
    this.value = cell.value;
    this.found = cell.found;

    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < cell.units[i].size(); j++)
            this.units[i].push_back(cell.units[i][j]);
    }

    for (int i = 0; i < cell.peers.size(); i++)
        this.peers.push_back(cell.peers[i]);
}

Cell::Cell(string value, int line, int column, int boardSize)
{
    this.value = value;
    
    assigned = (value.Length == 1);

    SetUnitsAndPeersToCell(line, column, boardSize);
}

void Cell::SetUnitsAndPeersToCell(int line, int column, int boardSize)
{
    SetLineUnitAndPeers(line, column, boardSize);
    SetRowUnitAndPeers(line, column, boardSize);
    SetSquareUnitAndPeers(line, column, boardSize);
}

void Cell::SetSquareUnitAndPeers(int line, int column, int boardSize)
{
    int dx = column - (column % (int)sqrt(boardSize));
    int dy = line - (line % (int)sqrt(boardSize));

    for (int i = dy; i < dy + (int)sqrt(boardSize); i++)
        for (int j = dx; j < dx + (int)sqrt(boardSize); j++)
        {
            Pos pos;
            this.pos.line = i;
            this.pos.column = j;

            this.units[2].push_back(pos);
            if ((i == line) || (j == column)) continue;
            this.peers.push_back(pos);
        }
}

void Cell::SetRowUnitAndPeers(int line, int column, int boardSize)
{
    for (int i = 0; i < boardSize; i++)
    {
        Pos pos;
        pos.line = i;
        pos.column = column;
        this.units[1].push_back(pos);
        if (i == line) continue;
        this.peers.push_back
        this.Peers.push_back(pos);
    }
}

void Cell::SetLineUnitAndPeers(int line, int column, int boardSize)
{
    for (int i = 0; i < boardSize; i++)
    {
        Pos pos = new Pos();
        pos.line = line;
        pos.column = i;

        this.units[0].push_back(pos);
        if (i == column) continue;
        this.peers.push_back
    }
}
