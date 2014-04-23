#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string>
#include <iostream>
#include "Sudoku.h"

using namespace std;

int main(int argc, char **argv) {
	Sudoku sudoku;

	// Démarrage de l'horloge pour mesurer le temps d'exécution
	clock_t begin = clock();

	//fichier d'entré
	string filePath = "example/sudoku.in.txt"; 
	
	sudoku.initialiseGrid(filePath);
	sudoku.printGrid();
	cout << endl;
	sudoku.launchThreads();
	cout << endl;
	sudoku.printGrid();
	sudoku.clearValue();

	// Fin de l'horloge pour le temps d'éxécution
	clock_t end = clock();
	double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
	
	cout << "Temps d'Exécution : " << elapsed_secs << " secondes\n";
}