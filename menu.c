#include "menu.h"
#include <stddef.h>
#include <stdlib.h>

void ecrisImageTransparente(int x, int y, int largeur, int hauteur, const unsigned char *donnees) {
    if (donnees == NULL) return;
    unsigned char *pixels = (unsigned char*)malloc(largeur * hauteur * 4);
    if (pixels == NULL) return;

    for (int i = 0; i < largeur * hauteur; ++i) {
        unsigned char b = donnees[i * 3];
        unsigned char v = donnees[i * 3 + 1];
        unsigned char r = donnees[i * 3 + 2];

        pixels[i * 4] = b;
        pixels[i * 4 + 1] = v;
        pixels[i * 4 + 2] = r;

        if (r == 255 && v == 0 && b == 254) {
            pixels[i * 4 + 3] = 0;
        } else {
            pixels[i * 4 + 3] = 255;
        }
    }
    
#ifdef _WIN32
    extern void ecrisImageARVB(int x, int y, int largeur, int hauteur, const int *donneesARVB);
    ecrisImageARVB(x, y, largeur, hauteur, (const int*)pixels);
#else
    ecrisImageARVB(x, y, largeur, hauteur, (const int*)pixels);
#endif

    free(pixels);
}

void afficheMenu(int largeur, int hauteur, unsigned char *fond, BoutonImg bJouer, BoutonImg bQuitter) {
    if (fond != NULL) {
        ecrisImage(0, 0, largeur, hauteur, fond);
    } else {
        effaceFenetre(50, 50, 50);
    }

    // Affichage des boutons images
    if (bJouer.image != NULL) {
        ecrisImageTransparente(bJouer.x, bJouer.y, bJouer.largeur, bJouer.hauteur, bJouer.image);
    }
    
    if (bQuitter.image != NULL) {
        ecrisImageTransparente(bQuitter.x, bQuitter.y, bQuitter.largeur, bQuitter.hauteur, bQuitter.image);
    }
}

bool estSurBoutonImg(int x, int y, BoutonImg b) {
    return (x >= b.x && x <= b.x + b.largeur && y >= b.y && y <= b.y + b.hauteur);
}
