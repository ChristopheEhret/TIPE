#include "stdafx.h"
#include "Circuit.h"

Circuit::Circuit() {

}

Circuit::Circuit(sf::String pathCircuit) {

	///R�cup�re l'image du circuit

	//R�cup�re le chemin d'acc�s au circuit
	pathImgCircuit = pathCircuit + ".png";
	//Charge l'image du circuit
	if (!imgCircuit.loadFromFile(pathImgCircuit))
		//Si elle est mal charg�e, on ne continue pas; le circuit n'est pas valide
		return;

	///R�cup�re les couleurs des checkpoints et les positions de d�part � partir du fichier data

	//Chemin vers le fichier data, contenant les checkpoints et les d�parts du circuit
	sf::String pathData = pathCircuit + "_data.txt";
	//Ouvre le fichier data
	std::ifstream data = std::ifstream(std::string(pathData));
	//Si le fichier est mal charg�, on ne continue pas; le circuit n'est pas valide
	if (data.fail())
		return;
	
	std::string ligne;
	std::vector<std::string> listeLignes;
	//R�cup�re toutes les lignes du fichier
	while (std::getline(data, ligne)) {
		listeLignes.push_back(ligne);
	}

	//Indice de la ligne qui est entrain d'�tre lue
	int i = 0;
	//Premi�re boucle : r�cup�re la couleur de tous les check-point
	//Le caract�re '>' signifie que l'on termine la boucle des checkpoints et qu'on passe � la boucle des positions de d�part
	while (i < listeLignes.size() && listeLignes[i] != ">") {
		//Efface le retour � la ligne
		listeLignes[i].erase(listeLignes[i].length() - 1, listeLignes[i].length());

		//Liste qui contient la couleur du checkpoint en code RGB
		std::vector<int> liste_RGB;
		//Boucle qui r�cup�re les deux premiers param�tres du code RGB : la nuance de rouge et et de vert
		for (int j = 0; j < 2; j++) {
			//R�cup�re les caract�res entre le d�but de la ligne et le premier caract�re espace
			//Ces caract�re correspondent � la nuance d'une des couleurs
			std::string c = listeLignes[i].substr(0, listeLignes[i].find(" "));
			//Efface les caract�res qui viennent d'�tre lus
			listeLignes[i].erase(0, c.length() + 1);
			//Convertit les carat�res lus en integer et les stocke dans la liste
			liste_RGB.push_back(std::stoi(c));
		}

		//Le reste de la ligne ne contient alors que la nuace de bleu
		//Convertit le reste de la ligne en integer et la stocke dans la liste
		liste_RGB.push_back(std::stoi(listeLignes[i]));

		//Stocke la couleur du checkpoint en code RGB
		listeCheckPoints.push_back(sf::Color(liste_RGB[0], liste_RGB[1], liste_RGB[2]));

		//Incr�mente l'indice 
		i++;
	}

	//Incr�mente l'indice pour lire la ligne apr�s celle qui contient le caract�re ">"
	i++;

	//Deuxi�me boucle : r�cup�re les positions de d�part
	for (i; i < listeLignes.size(); i++) {
		//Efface le retour � la ligne
		listeLignes[i].erase(listeLignes[i].length() - 1, listeLignes[i].length());

		//Cr�� le vecteur position
		sf::Vector2f pos;
		
		//R�cup�re les caract�res qui correspondent � la position x
		std::string c = listeLignes[i].substr(0, listeLignes[i].find(" "));
		//Efface ces caract�res de la ligne
		listeLignes[i].erase(0, c.length() + 1);
		//Converti ces caract�res en integer et les stocke dans le vecteur position
		pos.x = std::stoi(c);

		//Idem avec la position y
		c = listeLignes[i].substr(0, listeLignes[i].find(" "));
		listeLignes[i].erase(0, c.length() + 1);
		pos.y = std::stoi(c);

		//Stocke la position de d�part
		listePosDeparts.push_back(pos);
		//Stocke l'angle de d�part
		listeAnglesDeparts.push_back(std::stoi(listeLignes[i]));

	}

	//Tout s'est bien pass�, le circuit est valide
	valide = true;
}

