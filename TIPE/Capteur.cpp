#include "stdafx.h"
#include "Capteur.h"

Capteur::Capteur() {
	defini = false;

}

Capteur::Capteur(Vector2f posRel, float rot, Vector2f F, Vector2f R, Circuit* c, float dist) {
	//Rotation du capteur en radians
	rotation = rot * PI / 180;

	//Circuit
	circuit = c;
	if (circuit)
		tailleFenetre = Vector2f(circuit->getImgCircuit()->getSize().x, circuit->getImgCircuit()->getSize().y);
	else
		tailleFenetre = Vector2f(1280, 900);

	if(circuit)
		defini = true;

	
	//On place le capteur
	updatePosition(F, R, posRel + F);

	distanceMax = dist;

}

float Capteur::captDistance() {
	if(!defini)
		return -1;
	
	//Initialisation
	bool aCapte = false;
	float distance = 0;

	//posCapt est la position à laquelle on vérifie si il y a un obstacle
	Vector2f posCapt = position;

	while (!aCapte && distance < distanceMax) {
		//On avance posCapt
		posCapt += vecteurDirecteur;
		//On ajoute la distance
		distance = sqrt(pow(posCapt.x - position.x, 2) + pow(posCapt.y - position.y, 2));

		//On capte les bords de la fenetre
		if (posCapt.x >= tailleFenetre.x || posCapt.x <= 0 || posCapt.y >= tailleFenetre.y || posCapt.y <= 0) {
			aCapte = true;
		}
		if (!aCapte && circuit)
			//Et la partie blanche du circuit
			if (circuit->getImgCircuit()->getPixel(posCapt.x, posCapt.y) == Color::White) {
				aCapte = true;
			}
	}

	//On renvoie la distance mesurée
	if (aCapte)
		return distance;
	else
		return -1;
}

void Capteur::updatePosition(Vector2f F, Vector2f R, Vector2f nPos) {
	if (!defini)
		return;

	//Change la position
	position = nPos;

	//Calcule le vecteur directeur du capteur (en fonction de la direction de l'individu et de la rotation du capteur par rapport à celui-ci)
	Vector2f FR = F - R;
	float distPA = sqrt(pow(FR.x, 2) + pow(FR.y, 2));
	Vector2f u = Vector2f(FR.x / distPA, FR.y / distPA);

	vecteurDirecteur.x = u.x * cos(rotation) - u.y * sin(rotation);
	vecteurDirecteur.y = u.y * cos(rotation) + u.x * sin(rotation);
}

void Capteur::draw(RenderWindow* w) {
	if (!defini)
		return;

	//On dessine une ligne entre la position du capteur et la portee max du capteur
	Vector2f u = Vector2f(vecteurDirecteur.x * distanceMax, vecteurDirecteur.y * distanceMax);

	Vertex line[] =
	{
		Vertex(position),
		Vertex(u + position)
	};
	w->draw(line, 2, Lines);
}