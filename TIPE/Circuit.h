#pragma once
#include "stdafx.h"
#include <fstream>

class Circuit {
public:

	//Constructeurs
	Circuit();
	Circuit(sf::String pathCircuit);

	//Accesseurs
	//Retourne la liste des checkpoints
	std::vector<sf::Color> getListeCheckPoints() { return listeCheckPoints; }
	//Retourne l'image du circuit
	sf::Image* getImgCircuit() { return &imgCircuit; }

	//Retourne la liste des positions de d�part
	std::vector<sf::Vector2f> getListePosDeparts() { return listePosDeparts; }
	//Retourne la liste des angles de d�part
	std::vector<int> getlisteAnglesDeparts() { return listeAnglesDeparts; }

	bool estValide() { return valide; }


private:
	//Liste des couleurs des diff�rents checkpoints
	std::vector<sf::Color> listeCheckPoints;

	//Liste des position de d�part disponibles pour l'individu
	std::vector<sf::Vector2f> listePosDeparts;
	//Liste des angles initiaux li�s aux differents d�parts du circuit (en degr�)
	std::vector<int> listeAnglesDeparts;
	
	//Chemin d'acc�s � l'image du circuit
	sf::String pathImgCircuit;

	//Image du circuit
	sf::Image imgCircuit;
	
	//Vrai si le circuit a bien �t� initialis� et si toutes les donn�es ont pu �tre chargs�es
	bool valide = false;
};
