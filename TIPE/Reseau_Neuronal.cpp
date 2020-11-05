#include "stdafx.h"
#include "Reseau_Neuronal.h"

Reseau_Neuronal::Reseau_Neuronal() {
	//Le r�seau n'a pas bien �t� initalis�
	initialise = false;
}
 //Attention ! : La liste poids doit contenir les biais des neurones
Reseau_Neuronal::Reseau_Neuronal(vector<vector<vector<float>>> poids, int nbEntrees){
	//On cr�e chaque neurone avec les poids et les biais en param�tre, tout en construisant la sructure �quivalente

	//Le nombre d'entr�es est la premi�re valeur de la structure
	structure.push_back(nbEntrees); 

	//On se balade dans les couches
	for (int i = 0; i < poids.size(); i++) {
		//On ajoute le nombre de neurone de la couche � la structure
		structure.push_back(poids[i].size());

		vector<Neurone*> couche;
		//On se balade dans les neurones de la couche
		for (int j = 0; j < poids[i].size(); j++) {
			//On recup�re le biais
			float biais = poids[i][j].back();
			//On l'enl�ve pour pas le consid�rer comme un poids
			poids[i][j].pop_back();

			vector<float> listePoids = poids[i][j];
			
			//On cr�e le neurone associ� aux poids et aux biais
			Neurone *n = new Neurone(listePoids, biais);
			//On met le neurone dans la couche
			couche.push_back(n);
		}
		//On met la couche dans le r�seau
		reseau.push_back(couche);
	}

	//Le r�seau a bien �t� initialis�
	initialise = true;
}

Reseau_Neuronal::Reseau_Neuronal(vector<int> struc) {

	//On initialise la structure
	structure = struc;
	//On se balade dans les couches de la structure
	//On ne prend pas la premi�re couche, car elle correspond au nombre d'entr�es
	for (int i = 1; i < structure.size(); i++) {
		vector<Neurone*> couche;

		//On cr�e le nombre voulu de neurones pour cette couche, qui correspond a la valeur indiqu�e par la structure pour la couhce correspondante 
		for (int j = 0; j < structure[i]; j++) {
			//Le neurone a autant d'entr�e que le nombre de neurones sur la couche pr�c�dente
			Neurone *n = neuroneAleatoire(structure[i - 1]);
			couche.push_back(n);
		}

		//On met la couche dans le r�seau
		reseau.push_back(couche);
	}

	//Le r�seau a bien �t� initialis�
	initialise = true;
}

Reseau_Neuronal::Reseau_Neuronal(Reseau_Neuronal *parent1, Reseau_Neuronal *parent2) {
	//Si les deux parents n'ont pas la m�me structure
	//Ou si l'un des deux parents n'est pas initialis�,  on ne peut pas cr�er d'enfant
	if (parent1->getStructure() != parent2->getStructure() || !parent1->estInitialise() || !parent2->estInitialise())
		return;

	//On r�cup�re la structure des parents
	structure = parent1->getStructure();

	//On se balade dans les couches de la structure
	for (int i = 1; i < structure.size(); i++) {
		vector<Neurone*> couche;

		//On cr�e le nombre voulu de neurones pour cette couche, par reproduction
		for (int j = 0; j < structure[i]; j++) {
			Neurone *n = neuroneReproduction(parent1->getNeurone(i - 1, j), parent2->getNeurone(i - 1, j));
			couche.push_back(n);
		}

		//On met la couche dans le r�seau
		reseau.push_back(couche);
	}

	//Le r�seau a bien �t� initialis�
	initialise = true;
}

Reseau_Neuronal::~Reseau_Neuronal() {
	//On se balade dans les couches
	for (int i = 0; i < reseau.size(); i++)
		//On d�truit chaque neurone
		for (int j = 0; j < reseau[i].size(); j++)
			delete reseau[i][j];
}

Neurone* Reseau_Neuronal::neuroneAleatoire(int nbInputs) {

	//On cr�e autant de points que le neurone a d'entr�es
	vector<float> listePoids;
	for (int i = 0; i < nbInputs; i++) {
		float p = (rand() % 2000) - 1000;
		p /= 1000;
		listePoids.push_back(p);
	}
	
	//On cr�e le biais
	float biais = (rand() % 2000) - 1000;
	biais /= 1000;

	if (listePoids.size() == 0)
		std::cout << std::endl;

	//On renvoie le neurone associ� aux poids et au biais
	return new Neurone(listePoids, biais);
}

Neurone* Reseau_Neuronal::neuroneReproduction(Neurone *neuroneParent1, Neurone *neuroneParent2) {

	//Si les deux neurones parents n'ont pas le m�me nombre de pods
	//Ou si l'un des deux neurones parents n'est pas initialis�,  on ne peut pas cr�er d'enfant
	if (neuroneParent1->getListePoids().size() != neuroneParent2->getListePoids().size() || !neuroneParent1->estInitialise() || !neuroneParent2->estInitialise())
		return new Neurone();

	vector<float> listePoids;
	//On se balade dans les poids
	for (int i = 0; i < neuroneParent1->getListePoids().size(); i++) {
		//On choisi al�atoirement pour chaque poids de quel parent il provient
		float p = 0;
		if (int(rand() % 2) == 0)
			p = neuroneParent1->getListePoids()[i];
		else
			p = neuroneParent2->getListePoids()[i];

		listePoids.push_back(p);
	}

	//On choisi al�atoirement de quel parent provient le biais
	float biais = 0;
	if (int(rand() % 2) == 0)
		biais = neuroneParent1->getBiais();
	else
		biais = neuroneParent2->getBiais();

	if (listePoids.size() == 0)
		std::cout << std::endl;

	//On renvoie le neurone associ� aux poids et au biais
	return new Neurone(listePoids, biais);
}

vector<float> Reseau_Neuronal::calculSortie(vector<float> input) {
	//Si le r�seau n'est pas initialis�, on ne renvoie rien
	if (!initialise)
		return {};

	//last_output : Sorties des neurones de la couche pr�c�dente
	vector<float> last_output = input;

	//On se balade dans les couches
	for (int i = 0; i < reseau.size(); i++) {

		vector<float> tmp_output;
		//On calcule les sorties des neurones de la couche, avec pour entr�es les sortie des neurones de la couche pr�c�dente
		for (int j = 0; j < reseau[i].size(); j++) {
			float output = reseau[i][j]->calculSortie(last_output);
			tmp_output.push_back(output);
		}

		last_output = tmp_output;
	}

	//On renvoie les sorties des neurones de la derni�re couche
	return last_output;
}

void Reseau_Neuronal::mutation() {
	//Si le r�seau n'est pas initialis�, il ne peut pas muter
	if (!initialise)
		return;

	//On prend un neurone al�atoire d'une couche al�atoire
	int couche = rand() % reseau.size();
	int neurone = rand() % reseau[couche].size();
	
	//On le remplace par un nouveau neurone cr�� al�atoirement
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