#pragma once

#include "stdafx.h"

class Neurone {
public:
	//Constructeurs
	Neurone();
	Neurone(std::vector<float> lPL, float b);

	//Calcul de la sortie du neurone en fonction des valeurs d'entrées
	float calculSortie(std::vector<float> entree);

	//Accesseurs
	std::vector<float> getListePoids() { return listePoidsLiens; }
	float getBiais() { return biais; };
	bool estInitialise() { return initialise; };

	void print();
	
private:
	//Liste des poids qui font le lien entre l'entrée et la sortie
	std::vector<float> listePoidsLiens;
	//Biais
	float biais = 0;

	//Vrai si le neurone a bien été initialisé
	float initialise = false;
};