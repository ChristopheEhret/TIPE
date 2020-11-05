#include "stdafx.h"
#include "Capteur.h"
#include "ReseauNeuronal.h"
#include "Circuit.h"

#define PI 3.14159265358979323846

#pragma once

/*
L'individu est un voiture, avec deux roues arri�res motrices
Elle est repr�sent�e par un rectangle defini comme ceci :

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

	//Fonction qui permet d'initialiser la majorit� des variables de l'individu
	//Elle est utilis�e dans presque tous les constructeurs
	void constructeurCommun(Vector2f f, Vector2f r, int largeur, Circuit* c);

	//Fonction qui �value l'individu sur un circuit donn�
	void evaluationIndividu(Vector2f f, Vector2f r, int largeur, Circuit* c, RenderWindow* w = nullptr);

	//Tick : fait avancer le v�hicule d'un pas de temps dt
	void tick();
	//Dessine le v�hicule
	void draw(RenderWindow* w);

	void mutation();

	//Fonction pour faire effectuer une rotation � un point
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

	//9 commandes disponibles pour le r�seau neuronal
	vector<vector<float>> wCommandes = { {-7, 7 },{ -3, 17 },{ 3, 11 },{ 8, 13 },{ 10, 10 },{ 13, 8 },{ 11, 3 },{ 17, -3 },{ 7, -7 } };

private:

	//R�seau neuronal de l'individu
	ReseauNeuronal *reseau_neuronal;

	//Points qui d�finissent la position de l'individu
	Vector2f A;
	Vector2f B;
	Vector2f C;
	Vector2f D;

	Vector2f F;
	Vector2f R;

	//Vecteur directeur du v�hicule
	Vector2f u;
	//Vecteur normal au v�hicule
	Vector2f n;

	//Largeur et hauteur du v�hicule
	int l;
	int h;

	//Vitesses de rotation des roues
	float wDroite = 0;
	float wGauche = 0;

	//Vitesse du v�hicule
	float vitesseRoue = 5;

	float wRoue = 100 / 7;
	float rRoue = 7;
	//Diam�tre de la roue
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

	//Le v�hicule s'est t-il cogn�?
	bool cogne = false;

	//Fonction d'initialisation
	void initialisationCapteurs();

	//Fonctions appell�es durant un tick()
	void calculReseauNeuronal();
	void checkTour();
	void checkSortieCircuit();
	void avance();

	//V�rifie si l'un des cot�s du v�hicule heurte un mur
	bool checkSortieSegmentCircuit(Vector2f depart, Vector2f arrivee, int nbPointsCheck);

	//Replace les points A, B, C et D
	void updatePoints();

	//Chemin vers l'image qui fait le circuit
	String pathFond = "null";

	//Nombre de tours parcourus
	int nbTours = 0;
	//Nombre de CherckpointsPass�s
	int nbCheckpointsPass�s = 0;

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

	//Resolution avec laquelle on check si un c�t� du v�hicule est sorti du circuit
	int resolutionCheckSortieCircuit = 3;

	//Score g�n�ral de l'individu. Plus il est haut, plus l'individu est performant.
	float fitness = 1;
};