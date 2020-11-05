#include "stdafx.h"
#include "Circuit.h"

Circuit::Circuit() {

}

Circuit::Circuit(sf::String pathCircuit) {

	///Récupère l'image du circuit

	//Récupère le chemin d'accès au circuit
	pathImgCircuit = pathCircuit + ".png";
	//Charge l'image du circuit
	if (!imgCircuit.loadFromFile(pathImgCircuit))
		//Si elle est mal chargée, on ne continue pas; le circuit n'est pas valide
		return;

	///Récupère les couleurs des checkpoints et les positions de départ à partir du fichier data

	//Chemin vers le fichier data, contenant les checkpoints et les départs du circuit
	sf::String pathData = pathCircuit + "_data.txt";
	//Ouvre le fichier data
	std::ifstream data = std::ifstream(std::string(pathData));
	//Si le fichier est mal chargé, on ne continue pas; le circuit n'est pas valide
	if (data.fail())
		return;
	
	std::string ligne;
	std::vector<std::string> listeLignes;
	//Récupère toutes les lignes du fichier
	while (std::getline(data, ligne)) {
		listeLignes.push_back(ligne);
	}

	//Indice de la ligne qui est entrain d'être lue
	int i = 0;
	//Première boucle : récupère la couleur de tous les check-point
	//Le caractère '>' signifie que l'on termine la boucle des checkpoints et qu'on passe à la boucle des positions de départ
	while (i < listeLignes.size() && listeLignes[i] != ">") {
		//Efface le retour à la ligne
		listeLignes[i].erase(listeLignes[i].length() - 1, listeLignes[i].length());

		//Liste qui contient la couleur du checkpoint en code RGB
		std::vector<int> liste_RGB;
		//Boucle qui récupère les deux premiers paramètres du code RGB : la nuance de rouge et et de vert
		for (int j = 0; j < 2; j++) {
			//Récupère les caractères entre le début de la ligne et le premier caractère espace
			//Ces caractère correspondent à la nuance d'une des couleurs
			std::string c = listeLignes[i].substr(0, listeLignes[i].find(" "));
			//Efface les caractères qui viennent d'être lus
			listeLignes[i].erase(0, c.length() + 1);
			//Convertit les caratères lus en integer et les stocke dans la liste
			liste_RGB.push_back(std::stoi(c));
		}

		//Le reste de la ligne ne contient alors que la nuace de bleu
		//Convertit le reste de la ligne en integer et la stocke dans la liste
		liste_RGB.push_back(std::stoi(listeLignes[i]));

		//Stocke la couleur du checkpoint en code RGB
		listeCheckPoints.push_back(sf::Color(liste_RGB[0], liste_RGB[1], liste_RGB[2]));

		//Incrémente l'indice 
		i++;
	}

	//Incrémente l'indice pour lire la ligne après celle qui contient le caractère ">"
	i++;

	//Deuxième boucle : récupère les positions de départ
	for (i; i < listeLignes.size(); i++) {
		//Efface le retour à la ligne
		listeLignes[i].erase(listeLignes[i].length() - 1, listeLignes[i].length());

		//Créé le vecteur position
		sf::Vector2f pos;
		
		//Récupère les caractères qui correspondent à la position x
		std::string c = listeLignes[i].substr(0, listeLignes[i].find(" "));
		//Efface ces caractères de la ligne
		listeLignes[i].erase(0, c.length() + 1);
		//Converti ces caractères en integer et les stocke dans le vecteur position
		pos.x = std::stoi(c);

		//Idem avec la position y
		c = listeLignes[i].substr(0, listeLignes[i].find(" "));
		listeLignes[i].erase(0, c.length() + 1);
		pos.y = std::stoi(c);

		//Stocke la position de départ
		listePosDeparts.push_back(pos);
		//Stocke l'angle de départ
		listeAnglesDeparts.push_back(std::stoi(listeLignes[i]));

	}

	//Tout s'est bien passé, le circuit est valide
	valide = true;
}

