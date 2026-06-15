#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "GfxLib.h"
#include "BmpLib.h"
#include "ESLib.h"
#include "menu.h"
#include "platforme.h"
#include <string.h>

#define LargeurFenetre 800
#define HauteurFenetre 600
#define ETAT_GAMEOVER 2
#define VIE 2

#define COTE_PLATEFORME 32
#define NB_SPRITES_MARCHE 5
#define NB_SPRITES_SQUELETTE 4

void gestionEvenement(EvenementGfx evenement);

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

typedef struct background {
    unsigned char *texture;
    int largeur;
    int hauteur;
} Background;

void affichePersonnage(Personnage p, Camera cam);
void afficheSquelette(Squelette s, Camera cam);
void affichePlateforme(Plateforme p, Camera cam);
void afficheDecoration(Decoration d, Camera cam);
void afficheBackground(Background bg, Camera cam);
int checkCollisionEmeraude(Personnage perso, Piece p);
int checkCollision(Personnage perso, Plateforme plat);
void gereCollisionPlateforme(Personnage *perso, Plateforme plat, int *vy, int *jumps);
int checkCollisionPortail(Personnage perso, Decoration d);
void afficheEcran(Ecran e, Camera cam);
void gereCollisionEcran(Personnage *perso, Ecran e, int *vy, int *jumps);
int checkCollisionEcran(Personnage perso, Ecran e);

int main(int argc, char **argv) {
    initialiseGfx(argc, argv);
    prepareFenetreGraphique("Minecraft 2D", LargeurFenetre, HauteurFenetre);
    lanceBoucleEvenements();
    return 0;
}

void gestionEvenement(EvenementGfx evenement) {
    static EtatJeu etat = ETAT_MENU;
    static Personnage Player;
    static Ecran nv1;
    static unsigned char *textureFondMenu = NULL;
    static Camera cam = {0, 0};
    static Background bgJeu = {NULL, 0, 0};
    static Squelette mob1;
    static BoutonImg bJouerMenu, bQuitterMenu;

    static int vyPerso = 0, jumps = 0;
    static int vie = VIE;
    static unsigned char *textureCoeur = NULL, *textureIconeEmeraude = NULL;
    static int largeurCoeur = 0, hauteurCoeur = 0;

    static int timeVal = 500, emeraudes = 0, frameCounter = 0;
    static char chrono[16];
    static bool keyD = false, keyQ = false;

    switch (evenement) {
        case Initialisation:
            Player.playerPos.x = LargeurFenetre/6.8; Player.playerPos.y = HauteurFenetre/2;
            Player.frameActuelle = 0; Player.timerAnim = 0; Player.regardeADroite = true; Player.vx = 0;

            for (int i = 0; i < NB_SPRITES_MARCHE; i++) {
                char chemin[50]; sprintf(chemin, i == 0 ? "images/steve.bmp" : "images/steve%d.bmp", i + 1);
                DonneesImageRGB *img = lisBMPRGB(chemin);
                if (img) { Player.sprites[i] = img->donneesRGB; Player.largeurs[i] = img->largeurImage; Player.hauteurs[i] = img->hauteurImage; }
            }

            mob1.pos.x = 600; mob1.pos.y = 100; mob1.origineX = 600; mob1.range = 200; mob1.vx = 2;
            mob1.frameActuelle = 0; mob1.timerAnim = 0; mob1.regardeADroite = false;
            for (int i = 0; i < NB_SPRITES_SQUELETTE; i++) {
                char chemin[50]; sprintf(chemin, i == 0 ? "images/squelette.bmp" : "images/squelette%d.bmp", i + 1);
                DonneesImageRGB *img = lisBMPRGB(chemin);
                if (img) { mob1.sprites[i] = img->donneesRGB; mob1.largeurs[i] = img->largeurImage; mob1.hauteurs[i] = img->hauteurImage; }
            }

            nv1 = initEcran1();
            DonneesImageRGB *ifm = lisBMPRGB("images/background.bmp"); if (ifm) textureFondMenu = ifm->donneesRGB;
            DonneesImageRGB *pij = lisBMPRGB("images/jouer.bmp"); 
            if (pij) { bJouerMenu.image = pij->donneesRGB; bJouerMenu.largeur = pij->largeurImage; bJouerMenu.hauteur = pij->hauteurImage; bJouerMenu.x = LargeurFenetre/2 - pij->largeurImage/2; bJouerMenu.y = HauteurFenetre/2 - 60; }
            DonneesImageRGB *pil = lisBMPRGB("images/leave.bmp");
            if (pil) { bQuitterMenu.image = pil->donneesRGB; bQuitterMenu.largeur = pil->largeurImage; bQuitterMenu.hauteur = pil->hauteurImage; bQuitterMenu.x = LargeurFenetre/2 - pil->largeurImage/2; bQuitterMenu.y = HauteurFenetre/2 - 80; }
            DonneesImageRGB *ibg = lisBMPRGB("images/backgroundlevel1.bmp"); if (ibg) { bgJeu.texture = ibg->donneesRGB; bgJeu.largeur = ibg->largeurImage; bgJeu.hauteur = ibg->hauteurImage; }
            DonneesImageRGB *ph = lisBMPRGB("images/heart.bmp"); if (ph) { textureCoeur = ph->donneesRGB; largeurCoeur = ph->largeurImage; hauteurCoeur = ph->hauteurImage; }
            DonneesImageRGB *pi = lisBMPRGB("images/emerald.bmp"); if (pi) textureIconeEmeraude = pi->donneesRGB;
            demandeTemporisation(20);
            break;

        case Temporisation:
            if (etat == ETAT_JEU) {
                Player.vx = keyD ? 6 : (keyQ ? -6 : 0);
                if (keyD) Player.regardeADroite = true; else if (keyQ) Player.regardeADroite = false;

                Personnage check = Player; check.playerPos.x += Player.vx; check.playerPos.y += 10;
                if (checkCollisionEcran(check, nv1)) Player.vx = 0;
                Player.playerPos.x += Player.vx;
                vyPerso -= 1;
                gereCollisionEcran(&Player, nv1, &vyPerso, &jumps, &coyoteFrame);
                Player.playerPos.y += vyPerso;

                if (Player.playerPos.y < -100) { vie--; Player.playerPos.x = LargeurFenetre/6.8; Player.playerPos.y = HauteurFenetre/2; vyPerso = 0; Player.vx = 0; }
                else if (vie == 0) etat = ETAT_GAMEOVER;

                if (Player.vx != 0 && jumps > 0) {
                    Player.timerAnim++; if (Player.timerAnim >= 5) { Player.frameActuelle = (Player.frameActuelle + 1) % NB_SPRITES_MARCHE; Player.timerAnim = 0; }
                } else Player.frameActuelle = 0;

                mob1.pos.x += mob1.vx;
                if (mob1.pos.x > mob1.origineX + mob1.range || mob1.pos.x < mob1.origineX - mob1.range) mob1.vx = -mob1.vx;
                mob1.regardeADroite = (mob1.vx < 0);
                mob1.timerAnim++; if (mob1.timerAnim >= 5) { mob1.frameActuelle = (mob1.frameActuelle + 1) % NB_SPRITES_SQUELETTE; mob1.timerAnim = 0; }

                cam.x = Player.playerPos.x - LargeurFenetre/2; if (cam.x < 0) cam.x = 0;
                frameCounter++; if (frameCounter >= 50) { timeVal--; frameCounter = 0; }
                if (timeVal <= 0) termineBoucleEvenements();

                for (int i = 0; i < MAX_EMERAUDES; i++) if (checkCollisionEmeraude(Player, nv1.emeraudes[i])) { emeraudes++; nv1.emeraudes[i].image = NULL; }
                for (int i = 0; i < MAX_DECORATIONS; i++) {
                    if (nv1.non_solides[i].texture && strstr((const char *)nv1.non_solides[i].texture, "short")) {
                        if (checkCollisionPortail(Player, nv1.non_solides[i])) { nv1 = initEcran2(); Player.playerPos.x = LargeurFenetre/2; Player.playerPos.y = HauteurFenetre/2; Player.vx = 0; vyPerso = 0; cam.x = 0; break; }
                    }
                }
            }
            sprintf(chrono, "%d", timeVal); rafraichisFenetre();
            break;

        case Affichage:
            effaceFenetre(255, 255, 255);
            if (etat == ETAT_MENU) afficheMenu(LargeurFenetre, HauteurFenetre, textureFondMenu, bJouerMenu, bQuitterMenu);
            else if (etat == ETAT_GAMEOVER) {
                effaceFenetre(0, 0, 0); couleurCourante(255, 0, 0); afficheChaine("GAME OVER", 40, LargeurFenetre/2 - 100, HauteurFenetre/2 + 20);
                couleurCourante(255, 255, 255); afficheChaine("Appuyez sur R pour revivre ou ECHAP pour quitter", 18, LargeurFenetre/2 - 200, HauteurFenetre/2 - 40);
            } else {
                afficheBackground(bgJeu, cam); afficheEcran(nv1, cam); afficheSquelette(mob1, cam); affichePersonnage(Player, cam);
                couleurCourante(0, 0, 0); afficheChaine(chrono, 24, 30, 550);
                if (textureIconeEmeraude) ecrisImageTransparente(30, 500, 32, 32, textureIconeEmeraude);
                char t[10]; sprintf(t, "x %d", emeraudes); afficheChaine(t, 24, 72, 505);
                if (textureCoeur) for (int i = 0; i < vie; i++) ecrisImageTransparente((LargeurFenetre-60)-(i*40), HauteurFenetre-50, largeurCoeur, hauteurCoeur, textureCoeur);
            }
            break;

        case Clavier:
            switch (caractereClavier()) {
                case 'F': case 'f': modePleinEcran(); break;
                case 'Q': case 'q': if (etat == ETAT_JEU) keyQ = true; break;
                case 'D': case 'd': if (etat == ETAT_JEU) keyD = true; break;
                case ' ': if (etat == ETAT_JEU && jumps != 0) { vyPerso = 12; jumps = 0; } break;
                case 'R': case 'r': if (etat == ETAT_GAMEOVER) { vie = VIE; emeraudes = 0; timeVal = 500; Player.playerPos.x = LargeurFenetre/2; Player.playerPos.y = HauteurFenetre/2; vyPerso = 0; Player.vx = 0; keyD = keyQ = false; cam.x = 0; nv1 = initEcran1(); etat = ETAT_JEU; } break;
                case 27: if (etat == ETAT_JEU) { etat = ETAT_MENU; Player.vx = 0; keyD = keyQ = false; } else termineBoucleEvenements(); break;
            }
            break;

        case ClavierRelache:
            if (caractereClavier() == 'q' || caractereClavier() == 'Q') keyQ = false;
            if (caractereClavier() == 'd' || caractereClavier() == 'D') keyD = false;
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

void afficheBackground(Background bg, Camera cam) { if (bg.texture) ecrisImage(0, 0, bg.largeur, bg.hauteur, bg.texture); }
void affichePersonnage(Personnage p, Camera cam) {
    unsigned char *s = p.sprites[p.frameActuelle]; int l = p.largeurs[p.frameActuelle], h = p.hauteurs[p.frameActuelle];
    int dx = (p.playerPos.x - cam.x) + (p.largeurs[0]/2 - l/2), dy = p.playerPos.y;
    if (s) { if (p.regardeADroite) ecrisImageTransparente(dx, dy, l, h, s); else ecrisImageInversee(dx, dy, l, h, s); }
}
void afficheSquelette(Squelette s, Camera cam) {
    unsigned char *sp = s.sprites[s.frameActuelle]; int l = s.largeurs[s.frameActuelle], h = s.hauteurs[s.frameActuelle];
    if (sp) { if (s.regardeADroite) ecrisImageInversee(s.pos.x-cam.x, s.pos.y, l, h, sp); else ecrisImageTransparente(s.pos.x-cam.x, s.pos.y, l, h, sp); }
}
void affichePlateforme(Plateforme p, Camera cam) {
    if (p.texture) for (int i=0; i<(p.largeur/32); i++) for (int j=0; j<(p.hauteur/32); j++) {
        int px = (p.coinInferieurGauche.x + 32*i + 16) - cam.x, py = (p.coinInferieurGauche.y + 32*j + 16) - cam.y;
        if (px + 16 >= 0 && px - 16 <= LargeurFenetre) ecrisImage(px, py, 32, 32, p.texture);
    }
}
int checkCollision(Personnage pe, Plateforme pl) {
    int l = pe.largeurs[0], h = pe.hauteurs[0];
    if (pe.playerPos.x + l/4 < pl.coinInferieurGauche.x || pe.playerPos.x - l/4 > (pl.coinInferieurGauche.x + pl.largeur)) return 0;
    if (pe.playerPos.y + h/4 < pl.coinInferieurGauche.y || pe.playerPos.y - h/4 - 2 > (pl.coinInferieurGauche.y + pl.hauteur)) return 0;
    return 1;
}
void gereCollisionPlateforme(Personnage *pe, Plateforme pl, int *vy, int *j) { if (checkCollision(*pe, pl)) { if (*vy >= 0) return; *vy = 0; pe->playerPos.y = pl.coinInferieurGauche.y + pl.hauteur + pe->hauteurs[0]/4 + 1; *j = 1; } }
void afficheEcran(Ecran e, Camera cam) {
    for (int i=0; i<MAX_PLATEFORMES; i++) affichePlateforme(e.solides[i], cam);
    for (int i=0; i<MAX_DECORATIONS; i++) afficheDecoration(e.non_solides[i], cam);
    for (int i = 0; i < MAX_EMERAUDES; i++) affichePiece(e.emeraudes[i], cam);
}
void gereCollisionEcran(Personnage *pe, Ecran e, int *vy, int *j) { for (int i=0; i<MAX_PLATEFORMES; i++) gereCollisionPlateforme(pe, e.solides[i], vy, j); }
int checkCollisionEcran(Personnage pe, Ecran e) { for (int i=0; i<MAX_PLATEFORMES; i++) if (checkCollision(pe, e.solides[i])) return 1; return 0; }
void afficheDecoration(Decoration d, Camera cam) {
    if (d.texture) for (int i=0; i<(d.largeur/32); i++) for (int j=0; j<(d.hauteur/32); j++) {
        int px = (d.coinInferieurGauche.x + 32*i + 16) - cam.x, py = (d.coinInferieurGauche.y + 32*j + 16) - cam.y;
        if (px + 16 >= 0 && px - 16 <= LargeurFenetre) ecrisImage(px, py, 32, 32, d.texture);
    }
}
int checkCollisionEmeraude(Personnage pe, Piece p) { if (!p.image) return 0; int lp = pe.largeurs[0], hp = pe.hauteurs[0]; if (pe.playerPos.x + lp/4 < p.pos.x || pe.playerPos.x - lp/4 > (p.pos.x + 32) || pe.playerPos.y + hp/4 < p.pos.y || pe.playerPos.y - hp/4 > (p.pos.y + 32)) return 0; return 1; }
int checkCollisionPortail(Personnage pe, Decoration d) { if (!d.texture) return 0; int lp = pe.largeurs[0], hp = pe.hauteurs[0]; if (pe.playerPos.x + lp < d.coinInferieurGauche.x || pe.playerPos.x > (d.coinInferieurGauche.x + d.largeur) || pe.playerPos.y + hp < d.coinInferieurGauche.y || pe.playerPos.y > (d.coinInferieurGauche.y + d.hauteur)) return 0; return 1; }
