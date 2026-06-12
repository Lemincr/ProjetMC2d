#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()

#define LargeurFenetre 800
#define HauteurFenetre 600

#define COTE_PLATEFORME 32
#define NB_SPRITES_MARCHE 5
#define NB_SPRITES_SQUELETTE 4
#define MAX_PLATEFORMES 2000
#define MAX_DECORATIONS 100

#define HAUTEUR_MAP 19
#define LARGEUR_MAP 100000
typedef struct coordonnees {
	int x;
	int y;
} Coord;

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


typedef struct ecran {
	Plateforme solides[MAX_PLATEFORMES];
	Decoration non_solides[MAX_DECORATIONS];
} Ecran;
Ecran initEcran1();
Plateforme initPlateforme(int x1, int y1, int larg, int haut, char *lienTexture);
Plateforme initPlateformeVide();
Decoration initDecoration(int x1, int y1, int largeur, int hauteur, char *lienTexture);
Decoration initDecorationVide();

Piece initPiece(int x1, int y1, char *lienTexture);
void affichePiece(Piece p, Camera cam);
