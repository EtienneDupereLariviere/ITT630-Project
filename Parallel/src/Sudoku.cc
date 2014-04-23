#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "Sudoku.h"
#include "SubGrid.h"
#include "solver.h"

using namespace std;

struct Sudoku::threadParameters {
	int threadNumber;
	Sudoku *sudoku;
	SubGrid *subGrid;
};

void Sudoku::initialiseGrid(string filePath) {
	ifstream infile;
	string dimString;
    infile.open(filePath.c_str());

	int i = 0;
	int j = 0;

	getline(infile,dimString);
	int dim = atoi(dimString.c_str());
	
	//Initialisation des variables
	emptyBlocks = 0;
	nbBlocks = dim;
	notSolvable = false;
	
	grid = new int* [dim];
	for (int i=0; i < dim; i++)
		grid[i] = new int[dim];

	while(getline(infile,dimString))
	{
		istringstream iss(dimString);
		j=0;
		do
		{
			string sub;
			iss >> sub;		
			grid[i][j] = atoi(sub.c_str());
			j++;
		} while (iss);
		i++;
	}
	infile.close();
	
	for (int i=0; i < nbBlocks; i++) {
		for (int j=0; j < nbBlocks; j++){
		  if (grid[i][j] == 0)
			emptyBlocks++;
		}
	}
}

void Sudoku::launchThreads(){
	struct threadParameters thread_data_array[nbBlocks];
	pthread_t thread[nbBlocks];

	cout << "Lancement de " << nbBlocks << " threads" << endl;

	for( int i = 0 ; i < nbBlocks ; i++ )
	{
		thread_data_array[i].threadNumber = i;
		thread_data_array[i].sudoku = this;
		thread_data_array[i].subGrid = new SubGrid();
		pthread_create(&thread[i], NULL, threadStart, (void *) &thread_data_array[i]);
	}
	
	for( int i = 0 ; i < nbBlocks ; i++ )
	{
		pthread_join(thread[i], NULL);
		delete[] thread_data_array[i].subGrid;
	}
}

void Sudoku::clearValue(){
	for (int i=0; i < nbBlocks; i++)
		delete[] grid[i];
	delete[] grid;
}

void Sudoku::printGrid(){
	for (int i=0; i < nbBlocks; i++) {
		for (int j=0; j < nbBlocks; j++)
		  cout << grid[i][j] << " ";
		cout << endl;
  }
}