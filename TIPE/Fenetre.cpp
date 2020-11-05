#include "stdafx.h"
#include "Fenetre.h"
#include "Circuit.h"


Fenetre::Fenetre() {

	tailleFenetre = Vector2f(1280, 900);

	RenderWindow fenetre(VideoMode(tailleFenetre.x, tailleFenetre.y), "Hey!");

	//Chemin vers l'image qui fait le circuit
	String pathfond = "terrains/terrain4";
	Texture textFond;
	Circuit t = Circuit(pathfond);

	//Defini les conditions de départ des individus
	Vector2f rDemarrage = t.getListePosDeparts()[0];

	float rot = t.getlisteAnglesDeparts()[0] * PI / 180;
	//float rot = PI / 2;
	Vector2f fDemarrage = rDemarrage + Vector2f(cos(rot), sin(rot)) * 20.f;

	//Cree l'individu
	i = *(new Individu(fDemarrage, rDemarrage, 20, &t, { 5, 10, 9 }));
	i.getReseauNeuronal()->load("NN_Data_43");
	//i.getReseauNeuronal()->save_arduino("NN_Data_Arudino");
	//i.getReseauNeuronal()->print();
	//i2 = *(new Individu(fDemarrage, rDemarrage, 20, &t));
	/*i.setTailleFenetre(tailleFenetre);
	i2.setTailleFenetre(tailleFenetre);*/
	
	/*for (int n = 0; n < 50; n++) {
		i.tick();
	}*/

	//i2.getReseauNeuronal()->print();

	//Charge le circuit
	if (pathfond != "null") {
		textFond.loadFromFile(pathfond + ".png");
		spriteFond = Sprite(textFond);
	}

	//On fait tourner le programme tant que la fenêtre n'a pas été fermée
	while (fenetre.isOpen()) {

		sf::Event event;
		while (fenetre.pollEvent(event))
		{
			//Fermeture de la fenêtre lorsque l'utilisateur le souhaite
			if (event.type == sf::Event::Closed)
				fenetre.close();
		}

		//Tick
		tick();
		//i.listePtTrajectoire.push_back(i.R);

		//Effacement de la fenêtre en noir
		fenetre.clear(sf::Color::Black);

		//C'est ici qu'on dessine tout
		affichage(&fenetre);

		//Fin de la frame courante, affichage de tout ce qu'on a dessiné
		fenetre.display();
	}
}

void Fenetre::tick() {
	//Tick de l'individu
	i.tick();
	//i2.tick();
}

void Fenetre::affichage(RenderWindow* w) {
	//Dessine le circuit
	w->draw(spriteFond);

	//Dessine l'individu
	i.draw(w);
	//i2.draw(w);
}

//FONCTION INUTILE
void Fenetre::dessineRectangle(RenderWindow* w, Vector2f P, Vector2f A){
	int a = 100;

	sf::Vertex linePA[] =
	{
		sf::Vertex(P),
		sf::Vertex(A)
	};
	
	w->draw(linePA, 2, sf::Lines);

	int E = sqrt(pow(A.x - P.x, 2) + pow(A.y - P.y, 2));
	sf::Vector2f u((A.x - P.x)/ E, (A.y - P.y)/ E);
	sf::Vector2f v(u.y, -u.x);

	sf::Vector2f Q(P + sf::Vector2f(v.x * a / 2, v.y * a / 2));
	sf::Vector2f S(A + sf::Vector2f(v.x * a / 2, v.y * a / 2));
	sf::Vector2f D(A - sf::Vector2f(v.x * a / 2, v.y * a / 2));
	sf::Vector2f F(P - sf::Vector2f(v.x * a / 2, v.y * a / 2));

	sf::Vertex lineQS[] =
	{
		sf::Vertex(Q),
		sf::Vertex(S)
	};
	w->draw(lineQS, 2, sf::Lines);

	sf::Vertex lineSD[] =
	{
		sf::Vertex(D),
		sf::Vertex(S)
	};
	w->draw(lineSD, 2, sf::Lines);

	sf::Vertex lineDF[] =
	{
		sf::Vertex(F),
		sf::Vertex(D)
	};
	w->draw(lineDF, 2, sf::Lines);

	sf::Vertex lineFQ[] =
	{
		sf::Vertex(Q),
		sf::Vertex(F)
	};
	w->draw(lineFQ, 2, sf::Lines);
}