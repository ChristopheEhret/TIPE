#include "stdafx.h"
#include "Individu.h"
#include "Neurone.h"

#pragma once


using namespace sf;
class Fenetre {

public:

	//Construction
	Fenetre();

	Individu i;
	Individu i2;

	//Fonctions principales 
	void tick();
	void affichage(RenderWindow* w);
	//FONCTION INUTILE
	void dessineRectangle(RenderWindow* w, Vector2f P, Vector2f A);
	
	void tickIndividus();
	Vector2f tailleFenetre;

	Sprite spriteFond;

	//RenderWindow fenetre;

protected:

	int nbIndividus = 10;
	int nbGenerations = 10;

	std::vector<Individu*> listeIndividus;

};