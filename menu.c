#include "menu.h"
#include <stddef.h>
#include <stdlib.h>

void ecrisImageTransparente(int x, int y, int largeur, int hauteur, const unsigned char *donnees) {
    unsigned char *pixels = (unsigned char*)malloc(largeur * hauteur * 4);
    if (pixels == NULL) return;

    for (int i = 0; i < largeur * hauteur; ++i) {
        // donnees est en BVR (Bleu, Vert, Rouge) d'après GfxLib.c
        unsigned char b = donnees[i * 3];
        unsigned char v = donnees[i * 3 + 1];
        unsigned char r = donnees[i * 3 + 2];

        pixels[i * 4] = b;
        pixels[i * 4 + 1] = v;
        pixels[i * 4 + 2] = r;

        // Couleur de transparence demandée : 255 0 254 (R V B)
        // Attention : l'ordre dans le tableau 'donnees' est BVR d'après GfxLib.c
        if (r == 255 && v == 0 && b == 254) {
            pixels[i * 4 + 3] = 0; // Transparent
        } else {
            pixels[i * 4 + 3] = 255; // Opaque
        }
    }
    
    // On utilise ecrisImageARVB de GfxLib pour envoyer les données avec alpha
    // Sous Windows, ecrisImageARVB n'est pas dans GfxLib.h mais elle existe dans GfxLib.c
    // Si on est sous Windows, on peut avoir besoin de la déclarer ou d'utiliser une alternative
    // Mais d'après GfxLib.h, ecrisImageARVB est derrière un #ifndef _WIN32.
    // Vérifions comment faire sous Windows.
#ifdef _WIN32
    // Sous Windows, on peut essayer d'utiliser glDrawPixels directement
    // car GfxLib.c inclut windows.h et GL/gl.h (via GLUT probablement)
    // Mais menu.c n'inclut pas forcément les headers OpenGL.
    // Utilisons une approche compatible si possible.
    // En fait, ecrisImageARVB est définie dans GfxLib.c même sous Windows, 
    // elle n'est juste pas exportée dans le .h
    extern void ecrisImageARVB(int x, int y, int largeur, int hauteur, const int *donneesARVB);
    ecrisImageARVB(x, y, largeur, hauteur, (const int*)pixels);
#else
    ecrisImageARVB(x, y, largeur, hauteur, (const int*)pixels);
#endif

    free(pixels);
}

Bouton getBoutonJouer(int largeurFenetre, int hauteurFenetre) {
    Bouton b = {largeurFenetre/2 - 100, hauteurFenetre/2 + 20, 200, 40, "JOUER"};
    return b;
}

Bouton getBoutonQuitter(int largeurFenetre, int hauteurFenetre) {
    Bouton b = {largeurFenetre/2 - 100, hauteurFenetre/2 - 40, 200, 40, "QUITTER"};
    return b;
}

void afficheMenu(int largeur, int hauteur, unsigned char *textureFond) {
    if (textureFond != NULL) {
        for (int i = 0; i < largeur; i += 32) {
            for (int j = 0; j < hauteur; j += 32) {
                ecrisImage(i, j, 32, 32, textureFond);
            }
        }
    } else {
        effaceFenetre(50, 50, 50);
    }

    // Titre Minecraft-ish
    couleurCourante(0, 0, 0);
    afficheChaine("MINECRAFT 2D", 42, largeur/2 - 152, hauteur - 98);
    couleurCourante(255, 255, 255);
    afficheChaine("MINECRAFT 2D", 40, largeur/2 - 150, hauteur - 100);

    Bouton bJouer = getBoutonJouer(largeur, hauteur);
    Bouton bQuitter = getBoutonQuitter(largeur, hauteur);

    // Dessin Bouton Jouer
    couleurCourante(0, 0, 0); // Bordure
    rectangle(bJouer.x - 2, bJouer.y - 2, bJouer.x + bJouer.largeur + 2, bJouer.y + bJouer.hauteur + 2);
    couleurCourante(120, 120, 120); // Fond gris
    rectangle(bJouer.x, bJouer.y, bJouer.x + bJouer.largeur, bJouer.y + bJouer.hauteur);
    couleurCourante(255, 255, 255);
    afficheChaine(bJouer.texte, 20, bJouer.x + 60, bJouer.y + 10);

    // Dessin Bouton Quitter
    couleurCourante(0, 0, 0); // Bordure
    rectangle(bQuitter.x - 2, bQuitter.y - 2, bQuitter.x + bQuitter.largeur + 2, bQuitter.y + bQuitter.hauteur + 2);
    couleurCourante(120, 120, 120); // Fond gris
    rectangle(bQuitter.x, bQuitter.y, bQuitter.x + bQuitter.largeur, bQuitter.y + bQuitter.hauteur);
    couleurCourante(255, 255, 255);
    afficheChaine(bQuitter.texte, 20, bQuitter.x + 50, bQuitter.y + 10);
}

bool estSurBouton(int x, int y, Bouton b) {
    return (x >= b.x && x <= b.x + b.largeur && y >= b.y && y <= b.y + b.hauteur);
}
