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
#define NB_SPRITES_SQUELETTE 4

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
    int vx;
} Personnage;

typedef struct squelette {
    Coord pos;
    unsigned char *sprites[NB_SPRITES_SQUELETTE];
    int largeurs[NB_SPRITES_SQUELETTE];
    int hauteurs[NB_SPRITES_SQUELETTE];
    int frameActuelle;
    int timerAnim;
    bool regardeADroite;
    int vx;
    int range;
    int origineX;
} Squelette;

typedef struct plateforme {
	Coord coinInferieurGauche;
	int largeur;
	int hauteur;
	unsigned char *texture;
} Plateforme;

typedef struct camera {
    int x;
} Camera;

typedef struct background {
    unsigned char *texture;
    int largeur;
    int hauteur;
} Background;

void affichePersonnage(Personnage p, Camera cam);
void afficheSquelette(Squelette s, Camera cam);
void affichePlateforme(Plateforme p, Camera cam);
void afficheBackground(Background bg, Camera cam);
Plateforme initPlateforme(int x1, int y1, int largeur, int hauteur, char *lienTexture);
int checkCollision(Personnage perso, Plateforme plat);
void gereCollisionPlateforme(Personnage *perso, Plateforme plat, int *vy, int *jumps);
void ecrisImageInversee(int x, int y, int largeur, int hauteur, const unsigned char *donnees);

int main(int argc, char **argv)
{
	initialiseGfx(argc, argv);
	prepareFenetreGraphique("Minecraft 2D", LargeurFenetre, HauteurFenetre);
	lanceBoucleEvenements();
	return 0;
}

void cercle(float centreX, float centreY, float rayon)
{
	const int Pas = 20;
	const double PasAngulaire = 2.*M_PI/Pas;
	for (int index = 0; index < Pas; ++index)
	{
		const double angle = 2.*M_PI*index/Pas;
		triangle(centreX, centreY,
				 centreX+rayon*cos(angle), centreY+rayon*sin(angle),
				 centreX+rayon*cos(angle+PasAngulaire), centreY+rayon*sin(angle+PasAngulaire));
	}
}

void gestionEvenement(EvenementGfx evenement)
{
	static bool pleinEcran = false; 
    static EtatJeu etat = ETAT_MENU;

	static Personnage Player;
	static Plateforme p1;
    static unsigned char *textureFondMenu = NULL;
    static Camera cam = {0};
    static Background bgJeu = {NULL, 0, 0};
    static Squelette mob1;
    static BoutonImg bJouerMenu, bQuitterMenu;
	
	static int vyPerso = 0;
	static int jumps = 0;
	
	switch (evenement)
	{
		case Initialisation:
			Player.playerPos.x = LargeurFenetre/2;
			Player.playerPos.y = HauteurFenetre/2;
            Player.frameActuelle = 0;
            Player.timerAnim = 0;
            Player.regardeADroite = true;
            Player.vx = 0;

			char chemin[50];
            for (int i = 0; i < NB_SPRITES_MARCHE; i++) {
                if (i == 0) sprintf(chemin, "images/steve.bmp");
                else sprintf(chemin, "images/steve%d.bmp", i + 1);
                DonneesImageRGB *img = lisBMPRGB(chemin);
                if (img != NULL) {
                    Player.sprites[i] = img->donneesRGB;
                    Player.largeurs[i] = img->largeurImage;
                    Player.hauteurs[i] = img->hauteurImage;
                }
            }

            mob1.pos.x = 600; mob1.pos.y = 100; mob1.origineX = 600; mob1.range = 200; mob1.vx = 2;
            mob1.frameActuelle = 0; mob1.timerAnim = 0; mob1.regardeADroite = false;
            for (int i = 0; i < NB_SPRITES_SQUELETTE; i++) {
                if (i == 0) sprintf(chemin, "images/squelette.bmp");
                else sprintf(chemin, "images/squelette%d.bmp", i + 1);
                DonneesImageRGB *img = lisBMPRGB(chemin);
                if (img != NULL) {
                    mob1.sprites[i] = img->donneesRGB;
                    mob1.largeurs[i] = img->largeurImage;
                    mob1.hauteurs[i] = img->hauteurImage;
                }
            }
			
			p1 = initPlateforme(16, 16, 300*COTE_PLATEFORME, 2*COTE_PLATEFORME, "images/grass.bmp");
			
            DonneesImageRGB *pImageFondMenu = lisBMPRGB("images/background.bmp");
            if (pImageFondMenu != NULL) textureFondMenu = pImageFondMenu->donneesRGB;
            
            DonneesImageRGB *pImgJouer = lisBMPRGB("images/jouer.bmp");
            if (pImgJouer != NULL) {
                bJouerMenu.image = pImgJouer->donneesRGB;
                bJouerMenu.largeur = pImgJouer->largeurImage;
                bJouerMenu.hauteur = pImgJouer->hauteurImage;
                bJouerMenu.x = LargeurFenetre / 2 - bJouerMenu.largeur / 2;
                bJouerMenu.y = HauteurFenetre / 2 + 20;
            }
            
            DonneesImageRGB *pImgLeave = lisBMPRGB("images/leave.bmp");
            if (pImgLeave != NULL) {
                bQuitterMenu.image = pImgLeave->donneesRGB;
                bQuitterMenu.largeur = pImgLeave->largeurImage;
                bQuitterMenu.hauteur = pImgLeave->hauteurImage;
                bQuitterMenu.x = LargeurFenetre / 2 - bQuitterMenu.largeur / 2;
                bQuitterMenu.y = HauteurFenetre / 2 - 80;
            }

            DonneesImageRGB *pImageBgJeu = lisBMPRGB("images/backgroundlevel1.bmp");
            if (pImageBgJeu != NULL) {
                bgJeu.texture = pImageBgJeu->donneesRGB;
                bgJeu.largeur = pImageBgJeu->largeurImage;
                bgJeu.hauteur = pImageBgJeu->hauteurImage;
            }
			demandeTemporisation(20);
			break;
		
		case Temporisation:
            if (etat == ETAT_JEU) {
                // Mouvement horizontal continu (vitesse réduite de 10 à 6)
                Player.playerPos.x += Player.vx;

                // GRAVITE
                vyPerso -= 1;
                gereCollisionPlateforme(&Player, p1, &vyPerso, &jumps);
                Player.playerPos.y += vyPerso;

                // GameOver : chute dans le vide
                if (Player.playerPos.y < -100) {
                    printf("GAME OVER - Steve est tombe dans le vide.\n");
                    termineBoucleEvenements(); // Tue le programme
                }

                // Animation Player
                if (Player.vx != 0) {
                    Player.timerAnim++;
                    if (Player.timerAnim >= 5) {
                        Player.frameActuelle = (Player.frameActuelle + 1) % NB_SPRITES_MARCHE;
                        Player.timerAnim = 0;
                    }
                } else {
                    Player.frameActuelle = 0;
                }

                // IA Squelette
                mob1.pos.x += mob1.vx;
                if (mob1.pos.x > mob1.origineX + mob1.range) { mob1.vx = -2; mob1.regardeADroite = true; }
                else if (mob1.pos.x < mob1.origineX - mob1.range) { mob1.vx = 2; mob1.regardeADroite = false; }
                mob1.timerAnim++;
                if (mob1.timerAnim >= 5) {
                    mob1.frameActuelle = (mob1.frameActuelle + 1) % NB_SPRITES_SQUELETTE;
                    mob1.timerAnim = 0;
                }

                cam.x = Player.playerPos.x - LargeurFenetre / 2;
                if (cam.x < 0) cam.x = 0;
            }
			rafraichisFenetre();
			break;
			
		case Affichage:
			effaceFenetre(255, 255, 255);
            if (etat == ETAT_MENU) {
                afficheMenu(LargeurFenetre, HauteurFenetre, textureFondMenu, bJouerMenu, bQuitterMenu);
            } else {
                afficheBackground(bgJeu, cam);
                affichePlateforme(p1, cam);
                afficheSquelette(mob1, cam);
                affichePersonnage(Player, cam);
            }
			break;
			
		case Clavier:
			switch (caractereClavier())
			{
				case 'F':
				case 'f':
					pleinEcran = !pleinEcran; 
					if (pleinEcran) modePleinEcran();
					else redimensionneFenetre(LargeurFenetre, HauteurFenetre);
					break;
				case 'Q':
				case 'q':
                    if (etat == ETAT_JEU) { Player.vx = -6; Player.regardeADroite = false; }
					break;
				case 'D':
				case 'd':
					if (etat == ETAT_JEU) { Player.vx = 6; Player.regardeADroite = true; }
					break;
                case 'S':
                case 's':
                    if (etat == ETAT_JEU) Player.vx = 0;
                    break;
				case 'Z':
				case 'z':
					if (etat == ETAT_JEU && jumps != 0) { vyPerso = 12; jumps = 0; }
					break;
                case 27: // Echap
                    if (etat == ETAT_JEU) { etat = ETAT_MENU; Player.vx = 0; }
                    else termineBoucleEvenements();
                    break;
			}
			break;
		case BoutonSouris:
			if (etatBoutonSouris() == GaucheAppuye && etat == ETAT_MENU) {
                if (estSurBoutonImg(abscisseSouris(), ordonneeSouris(), bJouerMenu)) etat = ETAT_JEU;
                else if (estSurBoutonImg(abscisseSouris(), ordonneeSouris(), bQuitterMenu)) termineBoucleEvenements();
			}
			break;
		default: break;
	}
}

void afficheBackground(Background bg, Camera cam) {
    if (bg.texture != NULL) ecrisImage(0, 0, bg.largeur, bg.hauteur, bg.texture);
}

void ecrisImageInversee(int x, int y, int largeur, int hauteur, const unsigned char *donnees) {
    unsigned char *pixels = (unsigned char*)malloc(largeur * hauteur * 4);
    if (pixels == NULL) return;
    for (int j = 0; j < hauteur; j++) {
        for (int i = 0; i < largeur; i++) {
            int indexSource = (j * largeur + (largeur - 1 - i)) * 3;
            int indexDest = (j * largeur + i) * 4;
            unsigned char b = donnees[indexSource], v = donnees[indexSource + 1], r = donnees[indexSource + 2];
            pixels[indexDest] = b; pixels[indexDest + 1] = v; pixels[indexDest + 2] = r;
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
    int l = p.largeurs[p.frameActuelle], h = p.hauteurs[p.frameActuelle];
    int drawX = (p.playerPos.x - cam.x) + (p.largeurs[0] / 2 - l / 2), drawY = p.playerPos.y;
    if (sprite != NULL) {
        if (p.regardeADroite) ecrisImageTransparente(drawX, drawY, l, h, sprite);
        else ecrisImageInversee(drawX, drawY, l, h, sprite);
    }
}

void afficheSquelette(Squelette s, Camera cam) {
    unsigned char *sprite = s.sprites[s.frameActuelle];
    int l = s.largeurs[s.frameActuelle], h = s.hauteurs[s.frameActuelle];
    if (sprite != NULL) {
        if (s.regardeADroite) ecrisImageInversee(s.pos.x - cam.x, s.pos.y, l, h, sprite);
        else ecrisImageTransparente(s.pos.x - cam.x, s.pos.y, l, h, sprite);
    }
}

void affichePlateforme(Plateforme p, Camera cam) {
    if (p.texture != NULL) {
        for (int i=0; i<(p.largeur/32); i++) {
            for (int j=0; j<(p.hauteur/32); j++) {
                int posX = (p.coinInferieurGauche.x + 32*i + 16) - cam.x, posY = (p.coinInferieurGauche.y + 32*j + 16);
                if (posX + 16 >= 0 && posX - 16 <= LargeurFenetre) ecrisImage(posX, posY, 32, 32, p.texture);
            }
        }	
    }
}

Plateforme initPlateforme(int x1, int y1, int larg, int haut, char *lienTexture) {
	Plateforme p; p.coinInferieurGauche.x = x1; p.coinInferieurGauche.y = y1; p.largeur = larg; p.hauteur = haut; p.texture = NULL;
	DonneesImageRGB * img = lisBMPRGB(lienTexture);
    if (img != NULL) p.texture = img->donneesRGB;
	return p;
}

int checkCollision(Personnage perso, Plateforme plat) {
    int l = perso.largeurs[0], h = perso.hauteurs[0];
	if (perso.playerPos.x + l/4 < plat.coinInferieurGauche.x) return 0;
	if (perso.playerPos.x - l/4 > (plat.coinInferieurGauche.x + plat.largeur)) return 0;
	if (perso.playerPos.y + h/4 < plat.coinInferieurGauche.y) return 0;
	if (perso.playerPos.y - h/4 - 2 > (plat.coinInferieurGauche.y + plat.hauteur)) return 0;
	return 1;
}

void gereCollisionPlateforme(Personnage *perso, Plateforme plat, int *vy, int *jumps) {
	if (checkCollision(*perso, plat) == 1) {
		if ((caractereClavier() == 'z') || (caractereClavier() == 'Z')) { if (*vy >= 0) return; }
		*vy = 0; perso->playerPos.y = plat.coinInferieurGauche.y + plat.hauteur + perso->hauteurs[0]/4 + 1; *jumps = 1;
	}
}
