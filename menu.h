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
    unsigned char *image;
} BoutonImg;

void afficheMenu(int largeur, int hauteur, unsigned char *fond, BoutonImg bJouer, BoutonImg bQuitter);
bool estSurBoutonImg(int x, int y, BoutonImg b);
void ecrisImageTransparente(int x, int y, int largeur, int hauteur, const unsigned char *donnees);

#endif
