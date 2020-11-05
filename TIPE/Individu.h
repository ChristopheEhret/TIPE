#include "stdafx.h"
#include "Capteur.h"
#include "ReseauNeuronal.h"
#include "Circuit.h"

#define PI 3.14159265358979323846

#pragma once

/*
L'individu est un voiture, avec deux roues arrières motrices
Elle est représentée par un rectangle defini comme ceci :

			 l
		 <------->
	
    	A    F    D    
		 *---*---*    ^
		 |       |	  |
		 |       |	  |
		 |       |	  | h
		 |       |	  |
		 |       |	  |
		 *---*---*	  v
		B    R	  C
*/

using namespace sf;
class Individu {
public:

	//Constructeur
	Individu();
	Individu(Vector2f f, Vector2f r, int largeur, Circuit* c);
	Individu(Vector2f f, Vector2f r, int largeur, Circuit* c, vector<arma::fmat> poids, vector<arma::fvec> biais);
	Individu(Vector2f f, Vector2f r, int largeur, Circuit* c, vector<int> struc);
	Individu(Vector2f f, Vector2f r, int largeur, Circuit* c, Individu* parent1, Individu* parent2);

	//Destructeur
	~Individu();

	//Fonction qui permet d'initialiser la majorité des variables de l'individu
	//Elle est utilisée dans presque tous les constructeurs
	void constructeurCommun(Vector2f f, Vector2f r, int largeur, Circuit* c);

	//Fonction qui évalue l'individu sur un circuit donné
	void evaluationIndividu(Vector2f f, Vector2f r, int largeur, Circuit* c, RenderWindow* w = nullptr);

	//Tick : fait avancer le véhicule d'un pas de temps dt
	void tick();
	//Dessine le véhicule
	void draw(RenderWindow* w);

	void mutation();

	//Fonction pour faire effectuer une rotation à un point
	Vector2f Rotation(Vector2f point, double angle);
	//Rotation autour d'un centre
	Vector2f Rotation(Vector2f point, double angle, Vector2f centre);

	//Fonctions de set
	void setTailleFenetre(Vector2f nTF) { tailleFenetre = nTF; }
	void setPathFond(String npf) { pathFond = npf; }
	void defaultFitness() { fitness = 1; }

	//Liste des points qui forment la trajectoire
	std::vector<Vector2f> listePtTrajectoire;

	//Accesseurs
	ReseauNeuronal* getReseauNeuronal() { return reseau_neuronal; }
	float getFitness() { return fitness; }
	void setFitness(int f) { fitness = f; }
	void setVitesseRoue(float nV) { vitesseRoue = nV; }

	//9 commandes disponibles pour le réseau neuronal
	vector<vector<float>> wCommandes = { {-7, 7 },{ -3, 17 },{ 3, 11 },{ 8, 13 },{ 10, 10 },{ 13, 8 },{ 11, 3 },{ 17, -3 },{ 7, -7 } };

private:

	//Réseau neuronal de l'individu
	ReseauNeuronal *reseau_neuronal;

	//Points qui définissent la position de l'individu
	Vector2f A;
	Vector2f B;
	Vector2f C;
	Vector2f D;

	Vector2f F;
	Vector2f R;

	//Vecteur directeur du véhicule
	Vector2f u;
	//Vecteur normal au véhicule
	Vector2f n;

	//Largeur et hauteur du véhicule
	int l;
	int h;

	//Vitesses de rotation des roues
	float wDroite = 0;
	float wGauche = 0;

	//Vitesse du véhicule
	float vitesseRoue = 5;

	float wRoue = 100 / 7;
	float rRoue = 7;
	//Diamètre de la roue
	float rayonRoue = 1;
	//Pas de temps (Euler)
	float dt = 0.16;

	float temps = 0;

	//Taille de la fenetre
	Vector2f tailleFenetre;

	//Nombre de capteurs
	static const int nbCapteurs = 5;
	//Listes des capteurs
	Capteur *listeCapteurs[nbCapteurs] = { nullptr, nullptr, nullptr, nullptr, nullptr };

	//Le véhicule s'est t-il cogné?
	bool cogne = false;

	//Fonction d'initialisation
	void initialisationCapteurs();

	//Fonctions appellées durant un tick()
	void calculReseauNeuronal();
	void checkTour();
	void checkSortieCircuit();
	void avance();

	//Vérifie si l'un des cotés du véhicule heurte un mur
	bool checkSortieSegmentCircuit(Vector2f depart, Vector2f arrivee, int nbPointsCheck);

	//Replace les points A, B, C et D
	void updatePoints();

	//Chemin vers l'image qui fait le circuit
	String pathFond = "null";

	//Nombre de tours parcourus
	int nbTours = 0;
	//Nombre de CherckpointsPassés
	int nbCheckpointsPassés = 0;

	//Sens du parcours du circuit
	int sensParcoursCircuit = 0;
	//Indice du dernier checkpoint dans la liste des checkpoints du circuit
	int indexDernierCheckpoint = -1;

	//Distance parcourue par l'individu
	float distanceParcourue = 0;
	//Distance parcourue depuis le dernier checkpoint
	float distCheckpoint = 0;

	//Circuit
	Circuit* circuit;

	//Resolution avec laquelle on check si un côté du véhicule est sorti du circuit
	int resolutionCheckSortieCircuit = 3;

	//Score général de l'individu. Plus il est haut, plus l'individu est performant.
	float fitness = 1;
};