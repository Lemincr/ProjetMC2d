#ifndef PLATFORME_H
#define PLATFORME_H

#include "GfxLib.h"
#include "BmpLib.h"

#define MAX_PLATEFORMES 2000
#define MAX_DECORATIONS 100
#define MAX_EMERAUDES 100

typedef struct {
    int x, y;
} Coord;

typedef struct {
    int x;
    int y; // On ajoute y pour la caméra si besoin de scrolling vertical futur
} Camera;

typedef struct {
    Coord coinInferieurGauche;
    int largeur, hauteur;
    unsigned char *texture;
} Plateforme;

typedef struct {
    Coord coinInferieurGauche;
    int largeur, hauteur;
    unsigned char *texture;
} Decoration;

typedef struct {
    Coord pos;
    unsigned char *image;
} Piece;

typedef struct {
    Plateforme solides[MAX_PLATEFORMES];
    Decoration non_solides[MAX_DECORATIONS];
    Piece emeraudes[MAX_EMERAUDES];
} Ecran;

Ecran initEcran1();
Ecran initEcran2();
Plateforme initPlateforme(int x1, int y1, int larg, int haut, char *lienTexture);
Plateforme initPlateformeVide();
Decoration initDecoration(int x1, int y1, int larg, int haut, char *lienTexture);
Decoration initDecorationVide();
Piece initPiece(int x1, int y1, char *lienTexture);
Piece initPieceVide();
void affichePiece(Piece p, Camera cam);
void ecrisImageInversee(int x, int y, int largeur, int hauteur, const unsigned char *donnees);

#endif
