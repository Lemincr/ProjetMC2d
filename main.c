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

void affichePersonnage(Personnage p);
void affichePlateforme(Plateforme p);
Plateforme initPlateforme(int x1, int y1, int largeur, int hauteur, char *lienTexture);

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

	static int xBalle;
	static int yBalle;
	static int vxBalle = 7;
	static int vyBalle = -7;
	
	static Personnage Player;
	static Plateforme p1;
	static unsigned char *textureFondMenu = NULL;
	
	static int vyPerso = 0;
	
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
            }
			rafraichisFenetre();
			break;
			
		case Affichage:
			effaceFenetre(255, 255, 255);
            if (etat == ETAT_MENU) {
                afficheMenu(largeurFenetre(), hauteurFenetre(), textureFondMenu);
            } else {
                rectangle(0, 0, largeurFenetre(), hauteurFenetre());
                couleurCourante(0, 0, 0);
                affichePersonnage(Player);
                affichePlateforme(p1);
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
					if (etat == ETAT_JEU) vyPerso = 10;
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
                } else {
                    xBalle = abscisseSouris();
                    yBalle = ordonneeSouris();
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

void affichePersonnage(Personnage p) {
    if (p.donneesImage != NULL)
	    ecrisImage(p.playerPos.x, p.playerPos.y, 32, 64, p.donneesImage);
}

void affichePlateforme(Plateforme p) {
    if (p.texture != NULL) {
        for (int i=0; i<(p.largeur/32); i++) {
            for (int j=0; j<(p.hauteur/32); j++) {
                ecrisImage((p.coinInferieurGauche.x + 32*i + 16), (p.coinInferieurGauche.y + 32*j + 16), 32, 32, p.texture);
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
