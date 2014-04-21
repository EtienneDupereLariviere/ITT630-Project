#include <cmath>
#include <algorithm>
#include <iostream>

#include "SubGrid.h"

struct SubGrid::Solution{
	int Nb_solution;
	int choices[255];
};

void SubGrid::initSubGrid(int threadNumber, int nbBlocks, int **grid){
	nbSquare = (int) sqrt(nbBlocks);
	x = threadNumber % nbSquare;
	y = threadNumber / nbSquare;
	emptyBlocks = 0;
	
	solution = new struct Solution* [nbSquare];
	for (int i=0; i < nbSquare; i++)
		solution[i] = new struct Solution[nbSquare];

	initChoices(nbBlocks, grid);
}

void SubGrid::initChoices(int nbBlocks, int **grid){
	for( int i = 0 ; i < nbSquare ; i++) {
		for( int j = 0 ; j < nbSquare ; j++) {	
			Solution *s = &(solution[i][j]);
			int Y = y * nbSquare + i;
			int X = x * nbSquare + j;
			int squareValue;
			
			if ( (squareValue = grid[Y][X]) != 0) {//si on connait déjà la case, on définit choices pour cette valeur et Nb_solution = 1
				s->Nb_solution = 1;
				for(int k=0; k<nbBlocks; k++) {
					s->choices[k] = 0;
				}
				s->choices[squareValue - 1] = 1;//exemple : si il y a un 2 dans la case, la 2° case de choices sera à 1, les autres à 0
			} else { // Si on ne connait pas la valeur (0), on active tous les choix et on met Nb_solution à n
				s->Nb_solution = nbBlocks;
				for(int k=0; k<nbBlocks; k++) {
					s->choices[k] = 1;
				}
			}
		}
	} 
}

int SubGrid::checkBlock(int yLocal, int xLocal, int yGlobal, int xGlobal, int nbBlocks, int **grid) {
	int result=0;
	Solution *s = &(solution[yLocal][xLocal]);
			
	if( s->Nb_solution != 1) { // Si Nb_solution = 1, on connait déjà la solution, on retourne alors 0 pour dire que l'on a rien trouvé de nouveau
		// renvoie la valeur résultat si la case concerné n'a qu'un choix de nombre disponible, 0 sinon
		result = getNaiveChoices(yLocal, xLocal, yGlobal, xGlobal, nbBlocks, grid);
		if(result != 0) { // Alors on efface ce choix des possibilités ...
			for(int p=0; p<nbSquare; p++) {
				for(int q=0; q<nbSquare; q++) {	
					// ... sauf pour la case concernée
					if( !((yGlobal % nbSquare == p) && (xGlobal % nbSquare == q)) ) {
						solution[p][q].choices[result - 1] = 0; // Donc ce choix n'est plus possible dans les autres cases
					}
				}
			}
		}
		else {
			// renvoie la valeur résultat si un nombre n'apparait qu'une fois dans un sous carré, 0 sinon
			result = getSingletonChoices(yLocal, xLocal, yGlobal, xGlobal, nbBlocks, grid);
		}
		return result;
	}
	return 0;
}

int SubGrid::getNaiveChoices(int yLocal, int xLocal, int yGlobal, int xGlobal, int nbBlocks, int **grid) {
	int result = 0;
	Solution *s = &(solution[yLocal][xLocal]);

	for(int i=0; i<nbBlocks; i++) {
		int tmp;

		//... Horizontal ...
		tmp = grid[yGlobal][i];
		if( tmp != 0 && s->choices[tmp-1] != 0) {
			s->choices[tmp-1] = 0;
			s->Nb_solution--;
		}

		//... Vertical ...
		tmp = grid[i][xGlobal];
		if( tmp != 0 && s->choices[tmp-1] != 0) {
			s->choices[tmp-1] = 0;
			s->Nb_solution--;
		}

		//... Dans le sous carré
		tmp = grid[y * nbSquare + i/nbSquare][x * nbSquare + i%nbSquare];
		if(tmp != 0 && s->choices[tmp-1] != 0) {
			s->choices[tmp-1] = 0;
			s->Nb_solution--;
		}
	}

	if(s->Nb_solution == 1) {
		// si on a plus qu'un choix, on trouvé la valeur. On recherche le seul choix disponible et on le renvoie 
		for(int i=0 ; i<nbBlocks ; i++) {
			if(s->choices[i] != 0) {
				result = i+1;
				break;
			}
		}
		return result; 
	}
	else {
		return 0;
	}
}

int SubGrid::getSingletonChoices(int yLocal, int xLocal, int yGlobal, int xGlobal, int nbBlocks, int **grid) {
	int tmp[nbBlocks]; // dans tmp on met la somme des tableaux "choices" du sous carré où Nb_solution > 1
	Solution *s = &(solution[yLocal][xLocal]);
	
	for(int l=0; l<nbBlocks; l++)
		tmp[l] = 0;

	//on repère si un nombre n'a qu'une seule case possible dans le carré. C'est le cas si une case de tmp est égale à 1 après toutes les sommes
	for(int i=0; i<nbSquare; i++) {
		for(int j=0; j<nbSquare; j++) {
			if(solution[i][j].Nb_solution != 1) { // On ne prend pas en compte les cases qu'on connait déjà
				for(int k=0; k<nbBlocks; k++) { // On fait la somme sur tout le tableau 
					tmp[k] = tmp[k] + solution[i][j].choices[k];
				}
			}				
		}
	}
	
	for(int i=0; i<nbBlocks; i++) { // On vérifie s'il y a un singleton dans le sous carré
		if(tmp[i] == 1 && s->choices[i] == 1) { // Si on en trouve un qui correspond au block analysé, on envoie la valeur  
			s->Nb_solution = 1;
			for(int j=0; j<nbBlocks; j++) {
				s->choices[j] = 0;
			}
			s->choices[i] = 1;
			
			for(int p=0; p<nbBlocks; p++) { // on regarde si une case avec cette valeur n'existe pas déjà dans le sous carré
				if(grid[yGlobal][xGlobal] == i+1) {
					return 0; // dans ce cas, notre résultat est faux : on renvoie 0
				}
			}
			return i+1; // On retourne la valeur trouvée de la case
		}
	}
	
	return 0;
}

void SubGrid::clearValue(){
	for (int i=0; i < nbSquare; i++)
		delete[] solution[i];
	delete[] solution;
}