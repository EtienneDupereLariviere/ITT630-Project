#include <pthread.h>
#include <iostream>
#include <list>
#include <iterator>
#include <unistd.h>

#include "solver.h"
#include "Sudoku.h"
#include "SubGrid.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

struct threadParameters {
	int threadNumber;
	Sudoku *sudoku;
	SubGrid *subGrid;
};

struct Result{
	int value;
	int y;
	int x;
};

list<Result> searchChoices(SubGrid *subGrid, Sudoku *sudoku);
void fillGrid(list<Result> result, Sudoku *sudoku);

void* threadStart(void* arg) {
	struct threadParameters *my_data; 
	my_data = (struct threadParameters*) arg;	
	Sudoku *sudoku = my_data->sudoku;
	SubGrid *subGrid = my_data->subGrid;
	
	int counter = 0;
	int wait = 0;
	int maxIterations = 2 * sudoku->nbBlocks * sudoku->nbBlocks;
	subGrid->initSubGrid(my_data->threadNumber, sudoku->nbBlocks, sudoku->grid);
	
	while(sudoku->emptyBlocks != 0) {
		list<Result> result;
		subGrid->emptyBlocks = sudoku->emptyBlocks;
		
		result = searchChoices(subGrid, sudoku);	
		fillGrid(result, sudoku);

		while(subGrid->emptyBlocks == sudoku->emptyBlocks && counter > 0 && wait <20) { // Si il n'y a pas eu de modif, on attend
			wait++;
			usleep(10);
		}
		wait = 0;
		counter++;
		if(counter > maxIterations) { // Si on tourne en rond, c'est qu'on est bloqué (Algo pas assez puissant)
			if(!sudoku->notSolvable) {
				printf("Le programme est incapable de résoudre le Sudoku. Seuls des Sudokus faciles et certains de difficulté moyennes peuvent être résolus\n");
				sudoku->notSolvable = true; // afin d'afficher le message qu'une fois
			}
			pthread_exit(0);
		}
	}	
	
	subGrid->clearValue();
}

list<Result> searchChoices(SubGrid *subGrid, Sudoku *sudoku) {
	list<Result> list;
	int tmp;
	
	for(int i=0; i<subGrid->nbSquare; i++) { // Pour chaque case, on cherche
		for(int j=0; j<subGrid->nbSquare; j++) {	
			// On analyse chaque case du sous carré, retourne la valeur de la case si "checkBlock" l'a trouvé
			tmp = subGrid->checkBlock(i,
							  j,
							  subGrid->y * subGrid->nbSquare + i, 
							  subGrid->x * subGrid->nbSquare + j,
							  sudoku->nbBlocks,
							  sudoku->grid);

				if(tmp != 0) { // Si on trouve une solution, on la rajoute dans result
					Result result;
					result.value = tmp;
					result.y = subGrid->y * subGrid->nbSquare + i;
					result.x = subGrid->x * subGrid->nbSquare + j;
					list.push_back(result);
				}
			}
		}
	return list;
}

void fillGrid(list<Result> result, Sudoku *sudoku) {
	list<Result>::iterator iter;
	iter = result.begin();
	
	pthread_mutex_lock(&mutex);
	
	while (iter != result.end()) {
		sudoku->grid[iter->y][iter->x] = iter->value;
		sudoku->emptyBlocks--;
		iter++;
	}

	pthread_mutex_unlock(&mutex);
}