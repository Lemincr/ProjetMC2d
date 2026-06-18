#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()
#include "menu.h" // Pour le menu principal
#include "platforme.h"

// Largeur et hauteur de fenetre : definies une seule fois dans platforme.h

#define ETAT_GAMEOVER 2
#define VIE 4

#define COTE_PLATEFORME 32

#define NB_SPRITES_MARCHE 5
#define NB_SPRITES_ZOMBIE 3
#define NB_SPRITES_SQUELETTE 4

#define MAX_PLATEFORMES 2000
#define MAX_DECORATIONS 100
#define MAX_ZOMBIES 5
#define MAX_SQUELETTES 5

// La fonction de gestion des evenements, appelee automatiquement par le systeme des qu'une evenement survient
void gestionEvenement(EvenementGfx evenement);

int main(int argc, char **argv) {
    initialiseGfx(argc, argv);
    prepareFenetreGraphique("Minecraft 2D", LargeurFenetre, HauteurFenetre);
    lanceBoucleEvenements();
    return 0;
}
void gestionEvenement(EvenementGfx evenement)
{
static EtatJeu etat = ETAT_MENU;

static Personnage Player;

static Ecran nv1;
static Ecran nv2;
static Ecran nv3;

static int niveauActuel = 1;
static unsigned char *textureFondMenu = NULL;
static Camera cam = {0};
static BoutonImg bJouerMenu, bQuitterMenu;
static Background bgJeu;
static ImageNiveau imagesNiveaux[3];
static int vyPerso = 0;
static int jumps = 0;
static int vie = VIE;
static unsigned char *textureCoeur = NULL;
static int largeurCoeur = 0, hauteurCoeur = 0;
static int time = 500;
static int emeraudes = 0;
static int frameCounter = 0;
static char chrono[3];
static unsigned char *textureIconeEmeraude = NULL;
static int coyoteFrame = 5;
static int invincibilityFrame = 0;

switch (evenement)
    {
        case Initialisation:
            Player.playerPos.x = LargeurFenetre/15;
            Player.playerPos.y = HauteurFenetre/5;
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

            nv1 = initEcran1();
            nv2 = initEcran2();
            nv3 = initEcran3();

            DonneesImageRGB *pImageFondMenu = lisBMPRGB("images/debut1850.bmp");
            if (pImageFondMenu != NULL) {
                textureFondMenu = pImageFondMenu->donneesRGB;
            }
            
            DonneesImageRGB *pImgJouer = lisBMPRGB("images/jouer2.bmp");
            if (pImgJouer != NULL) {
                bJouerMenu.image = pImgJouer->donneesRGB;
                bJouerMenu.largeur = pImgJouer->largeurImage;
                bJouerMenu.hauteur = pImgJouer->hauteurImage;
                bJouerMenu.x = LargeurFenetre / 2 - bJouerMenu.largeur / 2;
                bJouerMenu.y = HauteurFenetre / 2 -60;
            }
            
            DonneesImageRGB *pImgLeave = lisBMPRGB("images/leave2.bmp");
            if (pImgLeave != NULL) {
                bQuitterMenu.image = pImgLeave->donneesRGB;
                bQuitterMenu.largeur = pImgLeave->largeurImage;
                bQuitterMenu.hauteur = pImgLeave->hauteurImage;
                bQuitterMenu.x = LargeurFenetre / 2 - bQuitterMenu.largeur / 2;
                bQuitterMenu.y = HauteurFenetre / 2 - 80;
            }

            DonneesImageRGB *pImageBgJeu = lisBMPRGB("images/background1850.bmp");
            if (pImageBgJeu != NULL) {
                bgJeu.texture = pImageBgJeu->donneesRGB;
                bgJeu.largeur = pImageBgJeu->largeurImage;
                bgJeu.hauteur = pImageBgJeu->hauteurImage;
            }
            
            char cheminImageNiveau[50];
            for (int i = 0; i < 3; i++) {
                sprintf(cheminImageNiveau, "images/niveau%d.bmp", i + 1);
                DonneesImageRGB *pImageNiveau = lisBMPRGB(cheminImageNiveau);
                imagesNiveaux[i].texture = NULL;
                imagesNiveaux[i].largeur = 0;
                imagesNiveaux[i].hauteur = 0;
                if (pImageNiveau != NULL) {
                    enleveContourRose(pImageNiveau->donneesRGB, pImageNiveau->largeurImage, pImageNiveau->hauteurImage);
                    imagesNiveaux[i].texture = pImageNiveau->donneesRGB;
                    imagesNiveaux[i].largeur = pImageNiveau->largeurImage;
                    imagesNiveaux[i].hauteur = pImageNiveau->hauteurImage;
                }
            }

            DonneesImageRGB *pImgCoeur = lisBMPRGB("images/heart.bmp"); 
            if (pImgCoeur != NULL) {
                textureCoeur = pImgCoeur->donneesRGB;
                largeurCoeur = pImgCoeur->largeurImage;
                hauteurCoeur = pImgCoeur->hauteurImage;
            }
            DonneesImageRGB *pImgIcone = lisBMPRGB("images/emerald.bmp");
            if (pImgIcone != NULL) {
                textureIconeEmeraude = pImgIcone->donneesRGB;
            }
    
            demandeTemporisation(20);
            break;

        case Temporisation:
            if (etat == ETAT_JEU) {
                Personnage PlayerColCheck = Player;
                PlayerColCheck.playerPos.x = Player.playerPos.x + Player.vx;
                PlayerColCheck.playerPos.y = Player.playerPos.y + 10;
                
                switch (niveauActuel) {
                    case 1:
                        if (checkCollisionEcran(PlayerColCheck, nv1) == 1) {
                            Player.vx = 0;
                        }
                        break;
                    case 2:
                        if (checkCollisionEcran(PlayerColCheck, nv2) == 1) {
                            Player.vx = 0;
                        }
                        break;
                    case 3:
                        if (checkCollisionEcran(PlayerColCheck, nv3) == 1) {
                            Player.vx = 0;
                        }
                        break;
                }   
                
                Player.playerPos.x += Player.vx;

                vyPerso -= 1;
                Player.playerPos.y += vyPerso;
                
                switch (niveauActuel) {
                    case 1:
                        gereCollisionEcran(&Player, &nv1, &vyPerso, &jumps, &coyoteFrame, &invincibilityFrame, &vie);
                        break;
                    case 2:
                        gereCollisionEcran(&Player, &nv2, &vyPerso, &jumps, &coyoteFrame, &invincibilityFrame, &vie);
                        break;
                    case 3:
                        gereCollisionEcran(&Player, &nv3, &vyPerso, &jumps, &coyoteFrame, &invincibilityFrame, &vie);
                        break;
                }

                if (Player.playerPos.y < -100) {
                    vie--;
                    Player.playerPos.x = 100;
                    Player.playerPos.y = 300;
                }
                else if (vie == 0){
                    etat = ETAT_GAMEOVER;
                }

                if (Player.vx != 0) {
                    Player.timerAnim++;
                    if (Player.timerAnim >= 5) {
                        Player.frameActuelle = (Player.frameActuelle + 1) % NB_SPRITES_MARCHE;
                        Player.timerAnim = 0;
                    }
                } else {
                    Player.frameActuelle = 0;
                }
                
                cam.x = Player.playerPos.x - LargeurFenetre / 2;
                if (cam.x < 0) cam.x = 0;

                frameCounter++;
                if (frameCounter >= 50) {
                    time--;
                    frameCounter = 0;
                }
                sprintf(chrono, "%d", time);

                if (time <= 0) {
                    etat = ETAT_GAMEOVER;
                }
                switch (niveauActuel) {
                    case 1:
                        for (int i = 0; i < MAX_EMERAUDES; i++) {
                            if (checkCollisionEmeraude(Player, nv1.emeraudes[i]) == 1) {
                                emeraudes++;
                                nv1.emeraudes[i].image = NULL;
                                if (emeraudes >= 10) {
                                    vie++;
                                    emeraudes = 0;
                                }
                            }
                        }
                        break;
                    case 2:
                        for (int i = 0; i < MAX_EMERAUDES; i++) {
                            if (checkCollisionEmeraude(Player, nv2.emeraudes[i]) == 1) {
                                emeraudes++;
                                nv2.emeraudes[i].image = NULL;
                                if (emeraudes >= 10) {
                                    vie++;
                                    emeraudes = 0;
                                }
                            }
                        }
                        break;
                    case 3:
                        for (int i = 0; i < MAX_EMERAUDES; i++) {
                            if (checkCollisionEmeraude(Player, nv3.emeraudes[i]) == 1) {
                                emeraudes++;
                                nv3.emeraudes[i].image = NULL;
                                if (emeraudes >= 10) {
                                    vie++;
                                    emeraudes = 0;
                                }
                            }
                        }
                }

                switch (niveauActuel) {
                    case 1:
                        gereMobs(&nv1, Player);
                        break;
                    case 2:
                        gereMobs(&nv2, Player);
                        break;
                    case 3:
                        gereMobs(&nv3, Player);
                        break;
                }


                switch (niveauActuel) {
                    case 1:
                        if (Player.playerPos.x > 4200) { // 4200 normalement
                            niveauActuel++;
                            Player.playerPos.x = LargeurFenetre/15;
                            Player.playerPos.y = HauteurFenetre/5;
                            Player.vx = 0;
                            vyPerso = 0;
                            cam.x = 0;
                            for (int i=0; i<MAX_ZOMBIES; i++) {
                                nv1.zombies[i].actif = false;
                                for (int j=0; j<NB_SPRITES_ZOMBIE; j++) {
                                    nv1.zombies[i].sprites[j] = NULL;
                                }
                            }
                            for (int i=0; i<MAX_SQUELETTES; i++) {
                                nv1.squelettes[i].actif = false;
                                for (int j=0; j<NB_SPRITES_SQUELETTE; j++) {
                                    nv1.squelettes[i].sprites[j] = NULL;
                                }
                            }
                            break; 
                        }
                        break;
                    case 2:
                        if (Player.playerPos.x > 3000) {
                            niveauActuel++; 
                            Player.playerPos.x = LargeurFenetre/15;
                            Player.playerPos.y = HauteurFenetre/5;
                            Player.vx = 0;
                            vyPerso = 0;
                            cam.x = 0;
                            break; 
                        }
                        break;
                    case 3:
                        if (Player.playerPos.x > 9999) {
                            niveauActuel++;
                            Player.playerPos.x = LargeurFenetre/15;
                            Player.playerPos.y = HauteurFenetre/5;
                            Player.vx = 0;
                            vyPerso = 0;
                            cam.x = 0;
                            break; 
                        }
                        break;
                }

                coyoteFrame--;

                if (coyoteFrame <= 0) {
                    jumps = 0;
                }
            }

            if (invincibilityFrame > 0) {
                invincibilityFrame--;
            }

            rafraichisFenetre();
            break;

        case Affichage:
            effaceFenetre(255, 255, 255);
            if (etat == ETAT_MENU) {
                afficheMenu(LargeurFenetre, HauteurFenetre, textureFondMenu, bJouerMenu, bQuitterMenu);
            }
            else if (etat == ETAT_GAMEOVER) {
                effaceFenetre(0, 0, 0); 
                couleurCourante(255, 0, 0); 
                afficheChaine("GAME OVER", 40, LargeurFenetre / 2 - 100, HauteurFenetre / 2 + 20);
                couleurCourante(255, 255, 255);
                afficheChaine("Appuyez sur R pour revivre ou ECHAP pour quitter", 18, LargeurFenetre / 2 - 200, HauteurFenetre / 2 - 40);
            }
            else {
                afficheBackground(bgJeu, cam);
                
                switch (niveauActuel) {
                    case 1:
                        afficheEcran(nv1, cam);
                        break;
                    case 2:
                        afficheEcran(nv2, cam);
                        break;
                    case 3:
                        afficheEcran(nv3, cam);
                        break;
                }
                affichePersonnage(Player, cam);
                if (niveauActuel >= 1 && niveauActuel <= 3) {
                    afficheImageNiveau(imagesNiveaux[niveauActuel - 1]);
                }
                afficheChaine(chrono, 24, 30, 830);
                
                int emeraudeX = 100; 
                int emeraudeY = 825; 

                if (textureIconeEmeraude != NULL) {
                    ecrisImageTransparente(emeraudeX, emeraudeY, COTE_PLATEFORME, COTE_PLATEFORME, textureIconeEmeraude);
                }
                
                char texteNombre[10];
                sprintf(texteNombre, "x %d", emeraudes);
                afficheChaine(texteNombre, 24, emeraudeX + 42, emeraudeY + 5);

                if (textureCoeur != NULL) {
                    for (int i = 0; i < vie; i++) {
                        int posX = (1800) - (i * 40); 
                        int posY = 820; 
                        ecrisImageTransparente(posX, posY, largeurCoeur, hauteurCoeur, textureCoeur);
                    }
                } 
            }
            break;

        case Clavier:
            switch (caractereClavier())
            {
                case 'Q':
                case 'q':
                    if (etat == ETAT_JEU) { Player.vx = -6; Player.regardeADroite = false; }
                    break;
                case 'R':
                case 'r':
                    if (etat == ETAT_GAMEOVER) {
                        vie = VIE;
                        emeraudes = 0;
                        time = 500;
                        frameCounter = 0;
                        
                        Player.playerPos.x = LargeurFenetre/15;
                        Player.playerPos.y = HauteurFenetre/5;
                        vyPerso = 0;
                        Player.vx = 0;
                        
                        cam.x = 0;
                        nv1 = initEcran1();
                        
                        etat = ETAT_JEU; 
                    }
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
                case ' ':
                    if (etat == ETAT_JEU && jumps != 0) { vyPerso = 12; jumps = 0; }
                    break;
            }
            break;

        case BoutonSouris:
            if (etatBoutonSouris() == GaucheAppuye && etat == ETAT_MENU) {
                if (estSurBoutonImg(abscisseSouris(), ordonneeSouris(), bJouerMenu)) etat = ETAT_JEU;
                else if (estSurBoutonImg(abscisseSouris(), ordonneeSouris(), bQuitterMenu)) termineBoucleEvenements();
            }
            break;

        default: 
            break;
    }
}
