#include "stdafx.h"
#include "Individu.h"

Individu::Individu() {

}

Individu::Individu(Vector2f f, Vector2f r, int largeur, Circuit* t) {

	//Constructeur commun à tous les types de contructeurs présents
	constructeurCommun(f, r, largeur, t);
}

Individu::Individu(Vector2f f, Vector2f r, int largeur, Circuit* t, vector<arma::fmat> poids, vector<arma::fvec> biais) {

	//Constructeur commun à tous les types de contructeurs présents
	constructeurCommun(f, r, largeur, t);

	//Création du réseau neuronal, spécifique à ce constructeur
	reseau_neuronal = new ReseauNeuronal(poids, biais);

}

Individu::Individu(Vector2f f, Vector2f r, int largeur, Circuit* t, vector<int> struc) {

	//Constructeur commun à tous les types de contructeurs présents
	constructeurCommun(f, r, largeur, t);

	//Création du réseau neuronal, spécifique à ce constructeur
	reseau_neuronal = new ReseauNeuronal(struc);

}

Individu::Individu(Vector2f f, Vector2f r, int largeur, Circuit* t, Individu* parent1, Individu* parent2) {

	//Constructeur commun à tous les types de contructeurs présents
	constructeurCommun(f, r, largeur, t);

	//Création du réseau neuronal, spécifique à ce constructeur
	reseau_neuronal = ReseauNeuronal::reproduction(parent1->getReseauNeuronal(), parent2->getReseauNeuronal());

}

//Destructeur
Individu::~Individu() {
	delete reseau_neuronal;
	for (int i = 0; i < nbCapteurs; i++)
		delete listeCapteurs[i];
}

void Individu::constructeurCommun(Vector2f f, Vector2f r, int largeur, Circuit* c) {
	//Géométrie du véhicule
	F = f;
	R = r;

	l = largeur;
	h = sqrt(pow(F.x - R.x, 2) + pow(F.y - R.y, 2));

	//Circuit
	circuit = c;

	//Calcul du vecteur directeur du vehicule
	u = Vector2f((F.x - R.x) / h, (F.y - R.y) / h);
	//Calcul du vecteur normal 
	n = Vector2f(u.y, -u.x);


	//Points caractéristiques du véhicule
	A = F + n * float(l / 2);
	D = F - n * float(l / 2);
	B = R + n * float(l / 2);
	C = R - n * float(l / 2);

	//Initialisation des capteurs
	initialisationCapteurs();

}


void Individu::initialisationCapteurs() {

	for (Capteur* c : listeCapteurs) {
		if (c != nullptr)
			delete c;
	}

	//Initialisation des capteurs
	Capteur *c1 = new Capteur(Vector2f(0, 0), 0, F, R, circuit);
	Capteur *c2 = new Capteur(D - F, 45, F, R, circuit);
	Capteur *c3 = new Capteur(A - F, -45, F, R, circuit);
	Capteur *c4 = new Capteur(D - F - u * 10.f, 90, F, R, circuit);
	Capteur *c5 = new Capteur(A - F - u * 10.f, -90, F, R, circuit);

	//Mise en liste
	listeCapteurs[0] = c1;
	listeCapteurs[1] = c2;
	listeCapteurs[2] = c3;
	listeCapteurs[3] = c4;
	listeCapteurs[4] = c5;
}

//Mutation
void Individu::mutation() {
	reseau_neuronal->mutation();
}


void Individu::draw(RenderWindow* w) {
	
	//Dessine les lignes qui représentent le véhicule
	Vertex lineAB[] =
	{
		Vertex(A),
		Vertex(B)
	};
	w->draw(lineAB, 2, Lines);

	Vertex lineBC[] =
	{
		Vertex(B),
		Vertex(C)
	};
	w->draw(lineBC, 2, Lines);

	Vertex lineCD[] =
	{
		Vertex(C),
		Vertex(D)
	};
	w->draw(lineCD, 2, Lines);

	Vertex lineDA[] =
	{
		Vertex(D),
		Vertex(A)
	};
	w->draw(lineDA, 2, Lines);

	//Dessine les capteurs
	for (Capteur* c : listeCapteurs) {
		c->draw(w);
	}

	//Dessine la trajectoire
	if (listePtTrajectoire.size() >= 2) {
		for (int i = 0; i < listePtTrajectoire.size() - 1; i++) {
			Vertex line[] =
			{
				Vertex(listePtTrajectoire.at(i)),
				Vertex(listePtTrajectoire.at(i + 1))
			};
			w->draw(line, 2, Lines);
		}
	}

}


void Individu::evaluationIndividu(Vector2f f, Vector2f r, int largeur, Circuit * c, RenderWindow* window) {
	//Réinitialisation de la position
	constructeurCommun(f, r, largeur, c);

	//Réinitialisation des paramètres de l'individu
	cogne = false;
	nbTours = 0;
	nbCheckpointsPassés = 0;
	distanceParcourue = 0;
	distCheckpoint = 0;
	indexDernierCheckpoint = -1;
	sensParcoursCircuit = 0;

	//Distance entre les checkpoints
	int distMoyenneCheckpoint = 400;

	//Si w n'est pas nullptr, on affiche le terrain dans la fenetre
	Sprite s;
	Texture te;
	if (window) {
		te.loadFromImage(*(c->getImgCircuit()));
		s = Sprite(te);
	}

	//Fait avancer l'individu jusqu'à qu'il se cogne contre un mur ou qu'il fasse 2 tours ou qu'il parcoure trop de distance sans traverser de checkpoint
	while (!cogne && nbTours < 2 && distCheckpoint < distMoyenneCheckpoint * 5) {
		//Tick
		tick();

		//Si w n'est pas nullptr, on affiche l'individu
		if (window) {
			sf::Event event;
			while (window->pollEvent(event))
			{
				if (event.type == sf::Event::Closed)
					window->close();
			}

			window->clear(sf::Color::Black);
			window->draw(s);
			draw(window);
			window->display();
		}
	}

	//Calcul du score de l'individu
	int nbCheckpointsEnTout = nbCheckpointsPassés + nbTours * circuit->getListeCheckPoints().size();

	float ajoutFitness = 1;
	ajoutFitness += nbCheckpointsEnTout;
	ajoutFitness -= distanceParcourue / ((nbCheckpointsEnTout + 1) * 100);


	if (ajoutFitness < 1)
		ajoutFitness = 1;

	//Multiplie le nouveau score avec l'ancien score
	fitness *= ajoutFitness;
}

//Tick
//Unité de temps, permet de faire avancer l'individu de dt (pas de temps)
void Individu::tick() {

	//Calcul de la commande du réseau neuronal
	calculReseauNeuronal();

	//Avance l'individu de dt
	avance();

	//Vérifications : checkpoints et murs
	if (circuit) {
		//Si on est hors de la fenetre, on ne vérifie rien
		if (!((F.x + R.x) / 2 > circuit->getImgCircuit()->getSize().x || (F.y + R.y) / 2 > circuit->getImgCircuit()->getSize().y)) {
			checkTour();
			checkSortieCircuit();
		}
	}
}

void Individu::calculReseauNeuronal() {
	//Recupère les entrées
	std::vector<float> entrees;
	for (Capteur* c : listeCapteurs) {
		float dist = c->captDistance();

		if (dist <= -1)
			entrees.push_back(0);
		else
			entrees.push_back(1 - (dist / 200));
	}

	fvec output = reseau_neuronal->feedforward(entrees);

	int imax = 0;
	for (int i = 1; i < output.n_elem; i++)
		if (output[i] >= output[imax])
			imax = i;

	output.print();

	wGauche = wCommandes[imax][0];
	wDroite = wCommandes[imax][1];
}

void Individu::avance() {
		
	
	//Récupère l'ancien point R
	Vector2f old_R = R;

	if (abs(wDroite - wGauche) > 0.1){
		//Vitesses au centre des roues
		float vDroite = wDroite * rRoue / 10;
		float vGauche = wGauche * rRoue / 10;

		//Calcul du rayon : distance au CIR
		float pente = (vDroite - vGauche) / l;
		float r = -vDroite / pente;

		//Calcul de l'angle dTheta : rotation pendant le temps dt
		float dTheta = (vDroite / r) * dt;

		//Calcul de la position du CIR
		Vector2f CIR = R - n * float(r - l/2);

		//On tourne autour du CIR d'un angle dTheta
		F = Rotation(F, dTheta, CIR);
		R = Rotation(R, dTheta, CIR);

		//Tourne les capteurs
		for (Capteur* c : listeCapteurs) {
			c->updatePosition(F, R, Rotation(c->getPos(), dTheta, CIR));
		}
	}
	else {
		float vitesse = wDroite * rRoue;

		//L'angle de la roue est trop petit, on ne tourne pas, on avance en ligne droite
		F += Vector2f(u.x * vitesse * dt, u.y * vitesse * dt);
		R += Vector2f(u.x * vitesse * dt, u.y * vitesse * dt);

		for (Capteur* c : listeCapteurs) {
			c->updatePosition(F, R, c->getPos() + Vector2f(u.x * vitesse * dt, u.y * vitesse * dt));
		}
	}

	//Update la position des points A, B, C et D
	updatePoints();

	//Calcule la distance parcourue apres un tick
	Vector2f d = R - old_R;
	distanceParcourue += sqrt(pow(d.x, 2) + pow(d.y, 2));
	distCheckpoint += sqrt(pow(d.x, 2) + pow(d.y, 2));

	listePtTrajectoire.push_back(R);
}

void Individu::checkTour() {

	//Vérifie si le véhicule a effectué un tour complet
	if (nbCheckpointsPassés >= circuit->getListeCheckPoints().size()) {
		nbCheckpointsPassés = 0;
		nbTours++;
	}
	
	if (circuit->getListeCheckPoints().size() > 0) {
		//Récupère la couleur sous l'individu
		Color couleurSousIndividu = circuit->getImgCircuit()->getPixel((F.x + R.x) / 2, (F.y + R.y) / 2);
		
		if (couleurSousIndividu != Color::White && couleurSousIndividu != Color::Black) {
			//Si on ne sait pas ou il est
			if (indexDernierCheckpoint == -1) {
				vector<Color> listeCp = circuit->getListeCheckPoints();
				for (int i = 0; i < listeCp.size(); i++) {
					if (listeCp[i] == couleurSousIndividu) {
						indexDernierCheckpoint = i;
						nbCheckpointsPassés++;
						distCheckpoint = 0;
						break;
					}
				}
			}
			//Si on ne sait pas dans quel sens il parcours le circuit
			else if (sensParcoursCircuit == 0) {

				int nextIndex = (indexDernierCheckpoint + 1) % circuit->getListeCheckPoints().size();
				if (circuit->getListeCheckPoints()[nextIndex] == couleurSousIndividu) {
					sensParcoursCircuit = 1;
					nbCheckpointsPassés++;
					indexDernierCheckpoint = nextIndex;
					distCheckpoint = 0;
				}
				else {
					nextIndex = indexDernierCheckpoint - 1;
					if (nextIndex < 0)
						nextIndex += circuit->getListeCheckPoints().size();

					if (circuit->getListeCheckPoints()[nextIndex] == couleurSousIndividu) {
						sensParcoursCircuit = -1;
						nbCheckpointsPassés++;
						indexDernierCheckpoint = nextIndex;
						distCheckpoint = 0;
					}
				}
			}
			//Si on sait ou il est et dans quel sens il parcours le circuit
			else {
				int nextIndex = (indexDernierCheckpoint + sensParcoursCircuit);
				if(nextIndex < 0)
					nextIndex += circuit->getListeCheckPoints().size();

				nextIndex = nextIndex % circuit->getListeCheckPoints().size();
				
				if (circuit->getListeCheckPoints()[nextIndex] == couleurSousIndividu) {
					nbCheckpointsPassés++;
					indexDernierCheckpoint = nextIndex;
					distCheckpoint = 0;
				}
			}
		}
	}
}

void Individu::checkSortieCircuit() {
	//Vérification de sortie du circuit: on vérifie si un des côté est hors du circuit
	//Pour cela, on divise chaque côté en n (résolution) segments et on vérifie si une des extrémités est hors du circuit.
	//Il y a cependant un marge d'erreur, si le véhicule sort du circuit entre deux de ces extrémtés
	//Cette erreur est diminuée en augmentant la résolution.

	//Check si AB est hors du circuit :
	cogne = checkSortieSegmentCircuit(A, B, resolutionCheckSortieCircuit);	

	//Check si AC est hors du circuit :	
	if (!cogne) {
		cogne = checkSortieSegmentCircuit(A, C, resolutionCheckSortieCircuit);
	}

	//Check si CD est hors du circuit :
	if (!cogne) {
		cogne = checkSortieSegmentCircuit(C, D, resolutionCheckSortieCircuit);
	}

	//Check si BD est hors du circuit :	
	if (!cogne) {
		cogne = checkSortieSegmentCircuit(B, D, resolutionCheckSortieCircuit);
	}
}

bool Individu::checkSortieSegmentCircuit(Vector2f depart, Vector2f arrivee, int nbSegmentsCheck) {
	//Principe : On part du point de départ, et on avance jusqu'au point d'arrivée avec un pas de (dist/nbSegmentsCheck)
	bool sortie = false;

	float dist = sqrt(pow(arrivee.x - depart.x, 2) + pow(arrivee.y - depart.y, 2));
	//Vecteur directeur du segment
	Vector2f u = (arrivee - depart) / dist;										

	//On part du départ
	Vector2f pointVerif = depart;												

	//On a donc nbSegmentsCheck + 1 points à vérifier; ex : si on a 3 segments, on a 4 points à vérifier
	for (int i = 0; i < nbSegmentsCheck + 1; i++) {	
		//Si le point n'est pas sorti, on avance de dist/nbSegmentsCheck
		if (circuit->getImgCircuit()->getPixel(pointVerif.x, pointVerif.y) == Color::White)
			sortie = true;
		pointVerif += u * float(dist / nbSegmentsCheck);
	}

	//Renvoie si le segment est hors du circuit ou non
	return sortie;
}

Vector2f Individu::Rotation(Vector2f point, double angle)
{
	//Formules de trigonométrie pour la rotation
	double xr = cos(angle) * point.x - sin(angle) * point.y;
	double yr = cos(angle) * point.y + sin(angle) * point.x;

	return Vector2f(xr, yr);
}

Vector2f Individu::Rotation(Vector2f point, double angle, Vector2f centre)
{
	//On bouge autour du centre de rotation
	Vector2f pointr = Rotation(point - centre, angle);

	return pointr + centre;
}

void Individu::updatePoints() {
	//Calcul du vecteur directeur du véhicule
	u = Vector2f((F.x - R.x) / h, (F.y - R.y) / h);
	//Calcul du vecteur normal 
	n = Vector2f(u.y, -u.x);


	//Points caractéristiques du véhicule
	A = F + n * float(l / 2);
	D = F - n * float(l / 2);
	B = R + n * float(l / 2);
	C = R - n * float(l / 2);
}