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

	//Retourne la liste des positions de départ
	std::vector<sf::Vector2f> getListePosDeparts() { return listePosDeparts; }
	//Retourne la liste des angles de départ
	std::vector<int> getlisteAnglesDeparts() { return listeAnglesDeparts; }

	bool estValide() { return valide; }


private:
	//Liste des couleurs des différents checkpoints
	std::vector<sf::Color> listeCheckPoints;

	//Liste des position de départ disponibles pour l'individu
	std::vector<sf::Vector2f> listePosDeparts;
	//Liste des angles initiaux liés aux differents départs du circuit (en degré)
	std::vector<int> listeAnglesDeparts;
	
	//Chemin d'accès à l'image du circuit
	sf::String pathImgCircuit;

	//Image du circuit
	sf::Image imgCircuit;
	
	//Vrai si le circuit a bien été initialisé et si toutes les données ont pu être chargsées
	bool valide = false;
};
