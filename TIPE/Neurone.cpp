#include "stdafx.h"
#include "Neurone.h"
#include <iomanip>

Neurone::Neurone() {
	//Le neurone n'a pas bien �t� initialis�
	initialise = false;
}

Neurone::Neurone(std::vector<float> lPL, float b) {
	listePoidsLiens = lPL;
	biais = b;
	//Le neurone n'a pas bien �t� initialis�
	initialise = true;
}

float Neurone::calculSortie(std::vector<float> entree) {
	//Si le neurone n'a pas bien �t� initialis�
	//Ou qu'il n'y a pas autant d'entr�es que de poids associ�s
	if (!initialise)
		return -1;
	if (entree.size() != listePoidsLiens.size())
		return -1;

	//Calcul et renvoi de la valeur
	float valeur = biais;
	for (int i = 0; i < entree.size(); i++)
		valeur += entree.at(i) * listePoidsLiens.at(i);

	//Sortie en sigmoide :
	valeur = 2 / (1 + exp(-valeur));
	valeur -= 1;

	return valeur;
}

//Affiche les poids et le biais du neurone
void Neurone::print() {
	for (float p : listePoidsLiens) {
		//Aligne les poids positifs (le "-" d�cale les poids n�gatifs)
		if (p > 0)
			std::cout << " ";
		//On affiche que 3 d�cimale par poids
		std::cout << std::fixed << std::setprecision(3) << p << "\t";
	}

	//S�pare le biais des poids
	std::cout << "\t";
	//Idem
	if (biais > 0)
		std::cout << " ";

	//Affiche le biais
	std::cout << std::fixed << std::setprecision(3) << biais << std::endl;
}