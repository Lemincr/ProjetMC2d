#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()
#include <string.h>

#define LargeurFenetre 1850
#define HauteurFenetre 891

#define COTE_PLATEFORME 32
#define NB_SPRITES_MARCHE 5
#define NB_SPRITES_SQUELETTE 4
#define NB_SPRITES_ZOMBIE 3
#define MAX_PLATEFORMES 2000
#define MAX_DECORATIONS 100
#define MAX_EMERAUDES 100
#define MAX_ZOMBIES 5
#define MAX_SQUELETTES 5

#define HAUTEUR_MAP 19
#define LARGEUR_MAP 100100


typedef struct coordonnees {
	int x;
	int y;
} Coord;

typedef struct camera {
    int x;
	int y;
} Camera;

typedef struct plateforme {
	Coord coinInferieurGauche;
	int largeur;
	int hauteur;
	unsigned char *texture;
} Plateforme;

typedef struct decoration {
	Coord coinInferieurGauche;
	int largeur;
	int hauteur;
	unsigned char *texture;
} Decoration;

typedef struct piece {
	Coord pos;
	unsigned char *image;
} Piece;

typedef struct zombie {
    Coord pos;
    unsigned char *sprites[NB_SPRITES_ZOMBIE];
    int largeurs[NB_SPRITES_ZOMBIE];
    int hauteurs[NB_SPRITES_ZOMBIE];
    int frameActuelle;
    int timerAnim;
    bool regardeADroite;
    int vx;
    int range;
    int origineX;
    bool actif;
} Zombie;


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
    bool actif;
} Squelette;


typedef struct ecran {
	Plateforme solides[MAX_PLATEFORMES];
	Decoration non_solides[MAX_DECORATIONS];
	Piece emeraudes[MAX_EMERAUDES];
	Zombie zombies[MAX_ZOMBIES];
	Squelette squelettes[MAX_SQUELETTES];
} Ecran;
Ecran initEcran1();
Ecran initEcran3();
Plateforme initPlateforme(int x1, int y1, int larg, int haut, char *lienTexture);
Plateforme initPlateformeVide();
Decoration initDecoration(int x1, int y1, int largeur, int hauteur, char *lienTexture);
Decoration initDecorationVide();

Piece initPiece(int x1, int y1, char *lienTexture);
void affichePiece(Piece p, Camera cam);
Piece initPieceVide();
void ecrisImageInversee(int x, int y, int largeur, int hauteur, const unsigned char *donnees);
Zombie initZombie(int x1, int y1);
Squelette initSquelette(int x1, int y1);
Zombie initZombieVide();
Squelette initSqueletteVide();
