#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()

// Largeur et hauteur par defaut d'une image correspondant a nos criteres
#define LargeurFenetre 800
#define HauteurFenetre 600

#define COTE_PLATEFORME 32

// Fonction de trace de cercle
void cercle(float centreX, float centreY, float rayon);
/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement);

typedef struct coordonnees {
	int x;
	int y;
} Coord;

typedef struct personnage {
	Coord playerPos;
	unsigned char *donneesImage;
} Personnage;

typedef struct plateforme {
	Coord coinInferieurGauche;
	int largeur;
	int hauteur;
	unsigned char *texture;
} Plateforme;

void affichePersonnage(Personnage p);

void affichePlateforme(Plateforme p);

Plateforme initPlateforme(int x1, int y1, int largeur, int hauteur, char *lienTexture);

int main(int argc, char **argv)
{
	initialiseGfx(argc, argv);
	
	prepareFenetreGraphique("OpenGL", LargeurFenetre, HauteurFenetre);
	
	/* Lance la boucle qui aiguille les evenements sur la fonction gestionEvenement ci-apres,
		qui elle-meme utilise fonctionAffichage ci-dessous */
	lanceBoucleEvenements();
	
	return 0;
}



/* Fonction de trace de cercle */
void cercle(float centreX, float centreY, float rayon)
{
	const int Pas = 20; // Nombre de secteurs pour tracer le cercle
	const double PasAngulaire = 2.*M_PI/Pas;
	int index;
	
	for (index = 0; index < Pas; ++index) // Pour chaque secteur
	{
		const double angle = 2.*M_PI*index/Pas; // on calcule l'angle de depart du secteur
		triangle(centreX, centreY,
				 centreX+rayon*cos(angle), centreY+rayon*sin(angle),
				 centreX+rayon*cos(angle+PasAngulaire), centreY+rayon*sin(angle+PasAngulaire));
			// On trace le secteur a l'aide d'un triangle => approximation d'un cercle
	}
	
}




/* La fonction de gestion des evenements, appelee automatiquement par le systeme
des qu'une evenement survient */
void gestionEvenement(EvenementGfx evenement)
{
	static bool pleinEcran = false; // Pour savoir si on est en mode plein ecran ou 

	/* On va aussi animer une balle traversant l'ecran */
	static int xBalle;
	static int yBalle;
	static int vxBalle = 7;
	static int vyBalle = -7;
	
	static Personnage Player;
	static Plateforme p1;
	
	static int vyPerso = 0;
	
	
	switch (evenement)
	{
		case Initialisation:
			
			Player.playerPos.x = largeurFenetre()/2;
			Player.playerPos.y = hauteurFenetre()/2;
			DonneesImageRGB * pImagePerso;
			pImagePerso = lisBMPRGB("/home/isen/travail/projetS2/images/steve.bmp");
			DonneesImageRGB imagePerso = *pImagePerso;
			Player.donneesImage = imagePerso.donneesRGB;
			
			p1 = initPlateforme(16, 16, 20*COTE_PLATEFORME, 2*COTE_PLATEFORME, "/home/isen/travail/projetS2/images/grass.bmp");
			
			/* Le message "Initialisation" est envoye une seule fois, au debut du
			programme : il permet de fixer "image" a la valeur qu'il devra conserver
			jusqu'a la fin du programme : soit "image" reste a NULL si l'image n'a
			pas pu etre lue, soit "image" pointera sur une structure contenant
			les caracteristiques de l'image "imageNB.bmp" */

			// Configure le systeme pour generer un message Temporisation
			// toutes les 20 millisecondes
			
			demandeTemporisation(20);
			break;
		
		case Temporisation:
			// On met a jour les coordonnees de la balle
			xBalle += vxBalle;
			yBalle += vyBalle;
			
			
			// GRAVITE
			vyPerso -= 1;
			
			Player.playerPos.y += vyPerso;
			
			// On fait rebondir la balle si necessaire
			if (xBalle < 0 || xBalle >= largeurFenetre())
				vxBalle = -vxBalle;
			if (yBalle < 0 || yBalle >= hauteurFenetre())
				vyBalle = -vyBalle;
			// Les coordonnees de la balle ayant eventuellement change,
			// il faut redessiner la fenetre :
			rafraichisFenetre();
			break;
			
		case Affichage:
			
			// On part d'un fond d'ecran blanc
			effaceFenetre (255, 255, 255);
			affichePersonnage(Player);
			affichePlateforme(p1);
			break;
			
		case Clavier:
			printf("%c : ASCII %d\n", caractereClavier(), caractereClavier());

			switch (caractereClavier())
			{
				case 'F':
				case 'f':
					pleinEcran = !pleinEcran; // Changement de mode plein ecran
					if (pleinEcran)
						modePleinEcran();
					else
						redimensionneFenetre(LargeurFenetre, HauteurFenetre);
					break;

				case 'R':
				case 'r':
					// Configure le systeme pour generer un message Temporisation
					// toutes les 20 millisecondes (rapide)
					demandeTemporisation(20);
					break;

				case 'L':
				case 'l':
					// Configure le systeme pour generer un message Temporisation
					// toutes les 100 millisecondes (lent)
					demandeTemporisation(100);
					break;

				case 'S':
				case 's':
					// Configure le systeme pour ne plus generer de message Temporisation
					demandeTemporisation(-1);
					break;
				case 'Q':
				case 'q':
					Player.playerPos.x -= 10;
					break;
				case 'D':
				case 'd':
					Player.playerPos.x += 10;
					break;
				case 'Z':
				case 'z':
					vyPerso = 10;
					break;
			}
			break;
			
		case ClavierSpecial:
			printf("ASCII %d\n", toucheClavier());
			break;

		case BoutonSouris:
			switch (etatBoutonSouris())
			{
				case GaucheAppuye:
					printf("Bouton gauche appuye en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
					// Si le bouton gauche de la souris est appuye, faire repartir
					// la balle de la souris
					xBalle = abscisseSouris();
					yBalle = ordonneeSouris();
					break;
				case GaucheRelache:
					printf("Bouton gauche relache en : (%d, %d)\n", abscisseSouris(), ordonneeSouris());
					break;
				case DroiteAppuye:
				case DroiteRelache:
					puts("Bouton droite");
					break;
				case MilieuAppuye:
				case MilieuRelache:
					puts("Bouton milieu");
					break;
				case ScrollDown:
					puts("Scroll down");
					break;
				case ScrollUp:
					puts("Scroll up");
					break;
				case ScrollRight:
					puts("Scroll right");
					break;
				case ScrollLeft:
					puts("Scroll left");
					break;
			}
			break;
		
		case Souris: // Si la souris est deplacee
			break;
		
		case Inactivite: // Quand aucun message n'est disponible
			break;
		
		case Redimensionnement: // La taille de la fenetre a ete modifie ou on est passe en plein ecran
			// Donc le systeme nous en informe
			if (xBalle >= largeurFenetre())
				xBalle = largeurFenetre()-1;
			if (yBalle >= hauteurFenetre())
				yBalle = hauteurFenetre()-1;
			printf("Largeur : %d\t", largeurFenetre());
			printf("Hauteur : %d\n", hauteurFenetre());
			break;
	}
}

void affichePersonnage(Personnage p) {
	ecrisImage(p.playerPos.x, p.playerPos.y, 32, 64, p.donneesImage);
}

void affichePlateforme(Plateforme p) {
	for (int i=0; i<(p.largeur/32); i++) {
		for (int j=0; j<(p.hauteur/32); j++) {
			ecrisImage((p.coinInferieurGauche.x + 32*i + 16), (p.coinInferieurGauche.y + 32*j + 16), 32, 32, p.texture);
		}
	}	
}

Plateforme initPlateforme(int x1, int y1, int larg, int haut, char *lienTexture) {
	Plateforme p;
	p.coinInferieurGauche.x = x1;
	p.coinInferieurGauche.y = y1;
	p.largeur = larg;
	p.hauteur = haut;
			
	DonneesImageRGB * pImagePlat;
	pImagePlat = lisBMPRGB(lienTexture);
	DonneesImageRGB imagePlat = *pImagePlat;
	p.texture = imagePlat.donneesRGB;
	return p;
}
