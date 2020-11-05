#pragma once

#include "stdafx.h"
#include "Neurone.h"

using namespace std;
class Reseau_Neuronal {
public:

	//Constructeurs
	Reseau_Neuronal();
	//A partir des valeurs des poids et des biais
	/*Exemple d'un liste de poids :
		{ { {1, -0.5, 0.7, 1}, {0.2, 0.4, -0.56, -3}, {0.5, -0.4, -0.89, 0.4}, {0.322, -0.1, -0.1, 0.2} },
		  { {-0.2, 0.2, -0.2, 0.2, 0.4}, {-1, 0.1, 0.9, -0.3, 0.69} },
		  { {0.1, 0.1, 0.420}, {-0.5, -0.7, -0.42}, {0.2, 0.7, -0.9} } }
		
		Ca produira un r�seau � 3 couches, � 3 entr�es, 4 neurone pour la couche 1, 2 neurones pour la couche 2, 3 neurones pour la couche de sortie
	*/
	Reseau_Neuronal(vector<vector<vector<float>>> poids, int nbEntrees);	
	//Aleatoirement a partir d'un structure
	Reseau_Neuronal(vector<int> struc);
	//A partir de deux parents
	Reseau_Neuronal(Reseau_Neuronal *parent1, Reseau_Neuronal *parent2);

	~Reseau_Neuronal();

	//Calcul des valeurs de sortie du r�seau selon les valeurs d'entr�e
	vector<float> calculSortie(vector<float> input);

	//Mutation al�atoire d'un neurone du r�seau
	void mutation();

	//Accesseurs
	vector<int> getStructure() { return structure; };
	bool estInitialise() { return initialise; };
	Neurone* getNeurone(int i, int j) { if (i >= reseau.size() || j >= reseau[i].size()) return new Neurone(); else return reseau[i][j]; }

	void print();

private:
	//Structure du r�seau neuronal
	//Chaque entier correspond au nombre de neurone de la couche associ�e; Le premier correspond au nombre d'entr�es
	//Ex : {3, 5, 2, 1} correspond a un r�seau � 3 couches, 3 entr�es, 5 neurones pour la couche 1, 2 neurones pour la couche 2, 1 neurone de sortie
	vector<int> structure = {}; 

	//R�seau neuronal
	//Liste qui contient des liste de neurones. Chaque liste de neurones correspond � une couche.
	vector<vector<Neurone*>> reseau;

	//Creation de neurones
	//Creation par al�atoire
	Neurone* neuroneAleatoire(int nbInputs);
	//Creation par reproduction
	Neurone* neuroneReproduction(Neurone *neuroneParent1, Neurone *neuroneParent2);

	//Vrai si le r�seau a bien �t� initialis�
	bool initialise = false;
};