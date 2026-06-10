#include "menu.h"
#include <stddef.h>

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
