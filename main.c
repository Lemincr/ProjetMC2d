#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()
#include "menu.h" // Pour le menu principal

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

typedef struct camera {
    int x;
} Camera;

void affichePersonnage(Personnage p, Camera cam);
void affichePlateforme(Plateforme p, Camera cam);
Plateforme initPlateforme(int x1, int y1, int largeur, int hauteur, char *lienTexture);
int checkCollision(Personnage perso, Plateforme plat);
void gereCollisionPlateforme(Personnage *perso, Plateforme plat, int *vy, int *jumps);

int main(int argc, char **argv)
{
	initialiseGfx(argc, argv);
	
	prepareFenetreGraphique("Minecraft 2D", LargeurFenetre, HauteurFenetre);
	
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
	static bool pleinEcran = false; 
    static EtatJeu etat = ETAT_MENU;

	static Personnage Player;
	static Plateforme p1;
    static unsigned char *textureFondMenu = NULL;
    static Camera cam = {0};
	
	static int vyPerso = 0;
	static int jumps = 0;
	
	switch (evenement)
	{
		case Initialisation:
			Player.playerPos.x = largeurFenetre()/2;
			Player.playerPos.y = hauteurFenetre()/2;
			DonneesImageRGB * pImagePerso;
			pImagePerso = lisBMPRGB("images/steve.bmp");
            if (pImagePerso != NULL)
			    Player.donneesImage = pImagePerso->donneesRGB;
			
			p1 = initPlateforme(16, 16, 20*COTE_PLATEFORME, 2*COTE_PLATEFORME, "images/grass.bmp");
			
            DonneesImageRGB *pImageFond = lisBMPRGB("images/dirt.bmp");
            if (pImageFond != NULL)
                textureFondMenu = pImageFond->donneesRGB;

			demandeTemporisation(20);
			break;
		
		case Temporisation:
            if (etat == ETAT_JEU) {
                // GRAVITE
                vyPerso -= 1;
                gereCollisionPlateforme(&Player, p1, &vyPerso, &jumps);
                Player.playerPos.y += vyPerso;

                // Mise à jour de la caméra pour suivre Steve
                // Steve reste au centre si possible (x - Largeur/2)
                cam.x = Player.playerPos.x - LargeurFenetre / 2;
                if (cam.x < 0) cam.x = 0; // On ne scrolle pas avant le début du niveau
            }
			rafraichisFenetre();
			break;
			
		case Affichage:
			effaceFenetre(255, 255, 255);
            if (etat == ETAT_MENU) {
                afficheMenu(largeurFenetre(), hauteurFenetre(), textureFondMenu);
            } else {
                // On dessine le fond ciel ou autre si besoin, ici blanc par défaut
                affichePlateforme(p1, cam);
                affichePersonnage(Player, cam);
            }
			break;
			
		case Clavier:
			switch (caractereClavier())
			{
				case 'F':
				case 'f':
					pleinEcran = !pleinEcran; 
					if (pleinEcran)
						modePleinEcran();
					else
						redimensionneFenetre(LargeurFenetre, HauteurFenetre);
					break;

				case 'Q':
				case 'q':
                    if (etat == ETAT_JEU) Player.playerPos.x -= 10;
					break;
				case 'D':
				case 'd':
					if (etat == ETAT_JEU) Player.playerPos.x += 10;
					break;
				case 'Z':
				case 'z':
					if (etat == ETAT_JEU && jumps != 0) {
						vyPerso = 10;
						jumps = 0;
					}
					break;
                case 27: // Echap
                    if (etat == ETAT_JEU) etat = ETAT_MENU;
                    else termineBoucleEvenements();
                    break;
			}
			break;
			
		case ClavierSpecial:
			break;

		case BoutonSouris:
			if (etatBoutonSouris() == GaucheAppuye)
			{
                if (etat == ETAT_MENU) {
                    Bouton bJouer = getBoutonJouer(largeurFenetre(), hauteurFenetre());
                    Bouton bQuitter = getBoutonQuitter(largeurFenetre(), hauteurFenetre());
                    
                    if (estSurBouton(abscisseSouris(), ordonneeSouris(), bJouer)) {
                        etat = ETAT_JEU;
                    } else if (estSurBouton(abscisseSouris(), ordonneeSouris(), bQuitter)) {
                        termineBoucleEvenements();
                    }
                }
			}
			break;
		
		case Souris:
			break;
		
		case Inactivite:
			break;
		
		case Redimensionnement:
			break;
	}
}

void affichePersonnage(Personnage p, Camera cam) {
    if (p.donneesImage != NULL)
	    ecrisImageTransparente(p.playerPos.x - cam.x, p.playerPos.y, 32, 64, p.donneesImage);
}

void affichePlateforme(Plateforme p, Camera cam) {
    if (p.texture != NULL) {
        for (int i=0; i<(p.largeur/32); i++) {
            for (int j=0; j<(p.hauteur/32); j++) {
                int posX = (p.coinInferieurGauche.x + 32*i + 16) - cam.x;
                int posY = (p.coinInferieurGauche.y + 32*j + 16);
                
                // On ne dessine que si c'est visible à l'écran
                if (posX + 16 >= 0 && posX - 16 <= LargeurFenetre) {
                    ecrisImage(posX, posY, 32, 32, p.texture);
                }
            }
        }	
    }
}

Plateforme initPlateforme(int x1, int y1, int larg, int haut, char *lienTexture) {
	Plateforme p;
	p.coinInferieurGauche.x = x1;
	p.coinInferieurGauche.y = y1;
	p.largeur = larg;
	p.hauteur = haut;
	p.texture = NULL;
			
	DonneesImageRGB * pImagePlat;
	pImagePlat = lisBMPRGB(lienTexture);
    if (pImagePlat != NULL) {
	    DonneesImageRGB imagePlat = *pImagePlat;
	    p.texture = imagePlat.donneesRGB;
    }
	return p;
}

int checkCollision(Personnage perso, Plateforme plat) {
	// trop à gauche
	if (perso.playerPos.x + 8 < plat.coinInferieurGauche.x) {
		return 0;
	} 
	
	// trop à droite
	else if (perso.playerPos.x - 8 > (plat.coinInferieurGauche.x + plat.largeur)) {
		return 0;
	}
	
	// en dessous
	else if (perso.playerPos.y + 16 < plat.coinInferieurGauche.y) {
		return 0;
	}
	
	// trop haut (avec une marge)
	else if (perso.playerPos.y - 18 > (plat.coinInferieurGauche.y + plat.hauteur)) {
		return 0;
	}
	return 1;
}

void gereCollisionPlateforme(Personnage *perso, Plateforme plat, int *vy, int *jumps) {
	if (checkCollision(*perso, plat) == 1) {
		// check si on saute pas
		if ((caractereClavier() == 'z') || (caractereClavier() == 'Z')) {
			if (*vy >= 0) {
				return;
			}
		}
		*vy = 0;
		perso->playerPos.y = plat.coinInferieurGauche.y + plat.hauteur + 17;
		*jumps = 1;
	}
}
