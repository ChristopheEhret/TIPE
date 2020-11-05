#pragma once

#include "stdafx.h"
#include "Circuit.h"
#define PI 3.14159265358979323846

using namespace sf;
class Capteur {
public: 

	//Constructeurs
	Capteur();
	Capteur(Vector2f posRel, float rotRel, Vector2f A, Vector2f P, Circuit* t, float dist = 200);

	//Bouge le capteur dans la position voulue
	void updatePosition(Vector2f A, Vector2f P, Vector2f nPos);
	Vector2f getPos() { return position; }

	//Renvoie la distance capt�e
	float captDistance();
	//Dessine la port�e du capteur
	void draw(RenderWindow* w);
	
	void setTailleFenetre(Vector2f nTF) { tailleFenetre = nTF; }
	void setCircuit(Circuit* c) { circuit = c; }

protected:
	//Si false, le capteur n'a pas bien �t� initialis�
	bool defini = false;

	//Position relative au point F de l'individu
	Vector2f positionRelative;
	//Position absolue dans le rep�re de la fenetre
	Vector2f position;

	//Vecteur directeur de la droite parcourue par le capteur
	Vector2f vecteurDirecteur;
	float rotation;

	Vector2f tailleFenetre;

	//Circuit
	Circuit * circuit;

	//Distance maximale � laquelle le capteur peut d�tecter un obstacle
	float distanceMax;

	//Resolution du capteur pour la distance
	int resolutionCapteur = 20;
};
