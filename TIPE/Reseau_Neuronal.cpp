#include "stdafx.h"
#include "Reseau_Neuronal.h"

Reseau_Neuronal::Reseau_Neuronal() {
	//Le réseau n'a pas bien été initalisé
	initialise = false;
}
 //Attention ! : La liste poids doit contenir les biais des neurones
Reseau_Neuronal::Reseau_Neuronal(vector<vector<vector<float>>> poids, int nbEntrees){
	//On crée chaque neurone avec les poids et les biais en paramètre, tout en construisant la sructure équivalente

	//Le nombre d'entrées est la première valeur de la structure
	structure.push_back(nbEntrees); 

	//On se balade dans les couches
	for (int i = 0; i < poids.size(); i++) {
		//On ajoute le nombre de neurone de la couche à la structure
		structure.push_back(poids[i].size());

		vector<Neurone*> couche;
		//On se balade dans les neurones de la couche
		for (int j = 0; j < poids[i].size(); j++) {
			//On recupère le biais
			float biais = poids[i][j].back();
			//On l'enlève pour pas le considérer comme un poids
			poids[i][j].pop_back();

			vector<float> listePoids = poids[i][j];
			
			//On crée le neurone associé aux poids et aux biais
			Neurone *n = new Neurone(listePoids, biais);
			//On met le neurone dans la couche
			couche.push_back(n);
		}
		//On met la couche dans le réseau
		reseau.push_back(couche);
	}

	//Le réseau a bien été initialisé
	initialise = true;
}

Reseau_Neuronal::Reseau_Neuronal(vector<int> struc) {

	//On initialise la structure
	structure = struc;
	//On se balade dans les couches de la structure
	//On ne prend pas la première couche, car elle correspond au nombre d'entrées
	for (int i = 1; i < structure.size(); i++) {
		vector<Neurone*> couche;

		//On crée le nombre voulu de neurones pour cette couche, qui correspond a la valeur indiquée par la structure pour la couhce correspondante 
		for (int j = 0; j < structure[i]; j++) {
			//Le neurone a autant d'entrée que le nombre de neurones sur la couche précédente
			Neurone *n = neuroneAleatoire(structure[i - 1]);
			couche.push_back(n);
		}

		//On met la couche dans le réseau
		reseau.push_back(couche);
	}

	//Le réseau a bien été initialisé
	initialise = true;
}

Reseau_Neuronal::Reseau_Neuronal(Reseau_Neuronal *parent1, Reseau_Neuronal *parent2) {
	//Si les deux parents n'ont pas la même structure
	//Ou si l'un des deux parents n'est pas initialisé,  on ne peut pas créer d'enfant
	if (parent1->getStructure() != parent2->getStructure() || !parent1->estInitialise() || !parent2->estInitialise())
		return;

	//On récupère la structure des parents
	structure = parent1->getStructure();

	//On se balade dans les couches de la structure
	for (int i = 1; i < structure.size(); i++) {
		vector<Neurone*> couche;

		//On crée le nombre voulu de neurones pour cette couche, par reproduction
		for (int j = 0; j < structure[i]; j++) {
			Neurone *n = neuroneReproduction(parent1->getNeurone(i - 1, j), parent2->getNeurone(i - 1, j));
			couche.push_back(n);
		}

		//On met la couche dans le réseau
		reseau.push_back(couche);
	}

	//Le réseau a bien été initialisé
	initialise = true;
}

Reseau_Neuronal::~Reseau_Neuronal() {
	//On se balade dans les couches
	for (int i = 0; i < reseau.size(); i++)
		//On détruit chaque neurone
		for (int j = 0; j < reseau[i].size(); j++)
			delete reseau[i][j];
}

Neurone* Reseau_Neuronal::neuroneAleatoire(int nbInputs) {

	//On crée autant de points que le neurone a d'entrées
	vector<float> listePoids;
	for (int i = 0; i < nbInputs; i++) {
		float p = (rand() % 2000) - 1000;
		p /= 1000;
		listePoids.push_back(p);
	}
	
	//On crée le biais
	float biais = (rand() % 2000) - 1000;
	biais /= 1000;

	if (listePoids.size() == 0)
		std::cout << std::endl;

	//On renvoie le neurone associé aux poids et au biais
	return new Neurone(listePoids, biais);
}

Neurone* Reseau_Neuronal::neuroneReproduction(Neurone *neuroneParent1, Neurone *neuroneParent2) {

	//Si les deux neurones parents n'ont pas le même nombre de pods
	//Ou si l'un des deux neurones parents n'est pas initialisé,  on ne peut pas créer d'enfant
	if (neuroneParent1->getListePoids().size() != neuroneParent2->getListePoids().size() || !neuroneParent1->estInitialise() || !neuroneParent2->estInitialise())
		return new Neurone();

	vector<float> listePoids;
	//On se balade dans les poids
	for (int i = 0; i < neuroneParent1->getListePoids().size(); i++) {
		//On choisi aléatoirement pour chaque poids de quel parent il provient
		float p = 0;
		if (int(rand() % 2) == 0)
			p = neuroneParent1->getListePoids()[i];
		else
			p = neuroneParent2->getListePoids()[i];

		listePoids.push_back(p);
	}

	//On choisi aléatoirement de quel parent provient le biais
	float biais = 0;
	if (int(rand() % 2) == 0)
		biais = neuroneParent1->getBiais();
	else
		biais = neuroneParent2->getBiais();

	if (listePoids.size() == 0)
		std::cout << std::endl;

	//On renvoie le neurone associé aux poids et au biais
	return new Neurone(listePoids, biais);
}

vector<float> Reseau_Neuronal::calculSortie(vector<float> input) {
	//Si le réseau n'est pas initialisé, on ne renvoie rien
	if (!initialise)
		return {};

	//last_output : Sorties des neurones de la couche précédente
	vector<float> last_output = input;

	//On se balade dans les couches
	for (int i = 0; i < reseau.size(); i++) {

		vector<float> tmp_output;
		//On calcule les sorties des neurones de la couche, avec pour entrées les sortie des neurones de la couche précédente
		for (int j = 0; j < reseau[i].size(); j++) {
			float output = reseau[i][j]->calculSortie(last_output);
			tmp_output.push_back(output);
		}

		last_output = tmp_output;
	}

	//On renvoie les sorties des neurones de la dernière couche
	return last_output;
}

void Reseau_Neuronal::mutation() {
	//Si le réseau n'est pas initialisé, il ne peut pas muter
	if (!initialise)
		return;

	//On prend un neurone aléatoire d'une couche aléatoire
	int couche = rand() % reseau.size();
	int neurone = rand() % reseau[couche].size();
	
	//On le remplace par un nouveau neurone créé aléatoirement
	Neurone* n = reseau[couche][neurone];
	reseau[couche][neurone] = neuroneAleatoire(reseau[couche][neurone]->getListePoids().size());
	delete n;
}

void Reseau_Neuronal::print() {
	for (int i = 0; i < reseau.size(); i++) {
		for (int j = 0; j < reseau[i].size(); j++)
			reseau[i][j]->print();
		std::cout << "-------------------" << std::endl;
	}
}