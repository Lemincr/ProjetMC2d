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
#define NB_SPRITES_MARCHE 5

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
	unsigned char *sprites[NB_SPRITES_MARCHE];
    int largeurs[NB_SPRITES_MARCHE];
    int hauteurs[NB_SPRITES_MARCHE];
    int frameActuelle;
    int timerAnim;
    bool regardeADroite;
    bool enMouvement;
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

typedef struct ecran {
	Plateforme solides[20];
} Ecran;

void affichePersonnage(Personnage p, Camera cam);
void affichePlateforme(Plateforme p, Camera cam);
Plateforme initPlateforme(int x1, int y1, int largeur, int hauteur, char *lienTexture);
int checkCollision(Personnage perso, Plateforme plat);
void gereCollisionPlateforme(Personnage *perso, Plateforme plat, int *vy, int *jumps);

void afficheEcran(Ecran e, Camera cam);
void gereCollisionEcran(Personnage *perso, Ecran e, int *vy, int *jumps);

Plateforme initPlateformeVide();


// LES ECRANS (ajouter une fonction pour chaque ecran)
Ecran initEcran1();


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
	static Ecran e1;
    static unsigned char *textureFondMenu = NULL;
    static Camera cam = {0};
	
	static int vyPerso = 0;
	static int jumps = 0;
	
	switch (evenement)
	{
		case Initialisation:
			Player.playerPos.x = largeurFenetre()/2;
			Player.playerPos.y = hauteurFenetre()/2;
            Player.frameActuelle = 0;
            Player.timerAnim = 0;
            Player.regardeADroite = true;
            Player.enMouvement = false;

			char chemin[50];
            for (int i = 0; i < NB_SPRITES_MARCHE; i++) {
                if (i == 0) sprintf(chemin, "images/steve.bmp");
                else sprintf(chemin, "images/steve%d.bmp", i + 1);
                
                DonneesImageRGB *img = lisBMPRGB(chemin);
                if (img != NULL) {
                    Player.sprites[i] = img->donneesRGB;
                    Player.largeurs[i] = img->largeurImage;
                    Player.hauteurs[i] = img->hauteurImage;
                } else {
                    Player.sprites[i] = NULL;
                    Player.largeurs[i] = 0;
                    Player.hauteurs[i] = 0;
                }
            }
			
			e1 = initEcran1();
			
            DonneesImageRGB *pImageFond = lisBMPRGB("images/dirt.bmp");
            if (pImageFond != NULL)
                textureFondMenu = pImageFond->donneesRGB;

			demandeTemporisation(20);
			break;
		
		case Temporisation:
            if (etat == ETAT_JEU) {
                // GRAVITE
                vyPerso -= 1;
                gereCollisionEcran(&Player, e1, &vyPerso, &jumps);
                Player.playerPos.y += vyPerso;

                // Animation
                if (Player.enMouvement) {
                    Player.timerAnim++;
                    if (Player.timerAnim >= 5) { // Change de frame toutes les 100ms
                        Player.frameActuelle = (Player.frameActuelle + 1) % NB_SPRITES_MARCHE;
                        Player.timerAnim = 0;
                    }
                } else {
                    Player.frameActuelle = 0; // Frame d'arrêt
                }

                // Reset mouvement pour le prochain cycle
                Player.enMouvement = false;

                // Caméra
                cam.x = Player.playerPos.x - LargeurFenetre / 2;
                if (cam.x < 0) cam.x = 0;
            }
			rafraichisFenetre();
			break;
			
		case Affichage:
			effaceFenetre(255, 255, 255);
            if (etat == ETAT_MENU) {
                afficheMenu(largeurFenetre(), hauteurFenetre(), textureFondMenu);
            } else {
                afficheEcran(e1, cam);
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
                    if (etat == ETAT_JEU) {
                        Player.playerPos.x -= 10;
                        Player.regardeADroite = false;
                        Player.enMouvement = true;
                    }
					break;
				case 'D':
				case 'd':
					if (etat == ETAT_JEU) {
                        Player.playerPos.x += 10;
                        Player.regardeADroite = true;
                        Player.enMouvement = true;
                    }
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

// Fonction utilitaire pour retourner une image horizontalement
void ecrisImageInversee(int x, int y, int largeur, int hauteur, const unsigned char *donnees) {
    if (largeur <= 0 || hauteur <= 0) return;
    unsigned char *pixels = (unsigned char*)malloc(largeur * hauteur * 4);
    if (pixels == NULL) return;

    for (int j = 0; j < hauteur; j++) {
        for (int i = 0; i < largeur; i++) {
            int indexSource = (j * largeur + (largeur - 1 - i)) * 3;
            int indexDest = (j * largeur + i) * 4;

            unsigned char b = donnees[indexSource];
            unsigned char v = donnees[indexSource + 1];
            unsigned char r = donnees[indexSource + 2];

            pixels[indexDest] = b;
            pixels[indexDest + 1] = v;
            pixels[indexDest + 2] = r;
            
            if (r == 255 && v == 0 && b == 254) pixels[indexDest + 3] = 0;
            else pixels[indexDest + 3] = 255;
        }
    }
    
    extern void ecrisImageARVB(int x, int y, int largeur, int hauteur, const int *donneesARVB);
    ecrisImageARVB(x, y, largeur, hauteur, (const int*)pixels);
    free(pixels);
}

void affichePersonnage(Personnage p, Camera cam) {
    unsigned char *sprite = p.sprites[p.frameActuelle];
    int l = p.largeurs[p.frameActuelle];
    int h = p.hauteurs[p.frameActuelle];
    
    if (sprite != NULL && l > 0 && h > 0) {
        if (p.regardeADroite) {
	        ecrisImageTransparente(p.playerPos.x - cam.x, p.playerPos.y, l, h, sprite);
        } else {
            ecrisImageInversee(p.playerPos.x - cam.x, p.playerPos.y, l, h, sprite);
        }
    }
}

void affichePlateforme(Plateforme p, Camera cam) {
    if (p.texture != NULL) {
        for (int i=0; i<(p.largeur/32); i++) {
            for (int j=0; j<(p.hauteur/32); j++) {
                int posX = (p.coinInferieurGauche.x + 32*i + 16) - cam.x;
                int posY = (p.coinInferieurGauche.y + 32*j + 16);
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
	DonneesImageRGB * pImagePlat = lisBMPRGB(lienTexture);
    if (pImagePlat != NULL) p.texture = pImagePlat->donneesRGB;
	return p;
}

int checkCollision(Personnage perso, Plateforme plat) {
    // On utilise les dimensions de la frame 0 pour la collision par simplicité
    int l = perso.largeurs[0];
    int h = perso.hauteurs[0];
	if (perso.playerPos.x + l/4 < plat.coinInferieurGauche.x) return 0;
	if (perso.playerPos.x - l/4 > (plat.coinInferieurGauche.x + plat.largeur)) return 0;
	if (perso.playerPos.y + h/4 < plat.coinInferieurGauche.y) return 0;
	if (perso.playerPos.y - h/4 - 2 > (plat.coinInferieurGauche.y + plat.hauteur)) return 0;
	return 1;
}

void gereCollisionPlateforme(Personnage *perso, Plateforme plat, int *vy, int *jumps) {
	if (checkCollision(*perso, plat) == 1) {
		if ((caractereClavier() == 'z') || (caractereClavier() == 'Z')) {
			if (*vy >= 0) return;
		}
		*vy = 0;
		perso->playerPos.y = plat.coinInferieurGauche.y + plat.hauteur + perso->hauteurs[0]/4 + 1;
		*jumps = 1;
	}
}

void afficheEcran(Ecran e, Camera cam) {
	for (int i=0; i<20; i++) {
		affichePlateforme(e.solides[i], cam);
	}
}

void gereCollisionEcran(Personnage *perso, Ecran e, int *vy, int *jumps) {
	for (int i=0; i<20; i++) {
		gereCollisionPlateforme(perso, e.solides[i], vy, jumps);
	}
}

Plateforme initPlateformeVide() {
	Plateforme p;
	p.coinInferieurGauche.x = 0;
	p.coinInferieurGauche.y = 0;
	p.largeur = 0;
	p.hauteur = 0;
	p.texture = NULL;
	return p;
}

Ecran initEcran1() {
	Ecran e;
	e.solides[0] = initPlateforme(0, 0, 25*COTE_PLATEFORME, 2*COTE_PLATEFORME, "images/dirt.bmp");
	e.solides[1] = initPlateforme(0, 2*COTE_PLATEFORME, 25*COTE_PLATEFORME, 1*COTE_PLATEFORME, "images/grass.bmp");
	for (int i=2; i<20; i++) {
		e.solides[i] = initPlateformeVide();
	}
	return e;
}
