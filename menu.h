#ifndef MENU_H
#define MENU_H

#include "GfxLib.h"
#include "BmpLib.h"

typedef enum {
    ETAT_MENU,
    ETAT_JEU
} EtatJeu;

typedef struct {
    int x, y;
    int largeur, hauteur;
    char texte[50];
} Bouton;

void afficheMenu(int largeur, int hauteur, unsigned char *textureFond);
bool estSurBouton(int x, int y, Bouton b);
Bouton getBoutonJouer(int largeurFenetre, int hauteurFenetre);
Bouton getBoutonQuitter(int largeurFenetre, int hauteurFenetre);
void ecrisImageTransparente(int x, int y, int largeur, int hauteur, const unsigned char *donnees);

#endif
