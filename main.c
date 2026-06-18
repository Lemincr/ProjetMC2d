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



// Fonction de trace de cercle

void cercle(float centreX, float centreY, float rayon);

/* La fonction de gestion des evenements, appelee automatiquement par le systeme

des qu'une evenement survient */

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













typedef struct background {

    unsigned char *texture;

    int largeur;

    int hauteur;

} Background;

typedef struct imageNiveau {

    unsigned char *texture;

    int largeur;

    int hauteur;

} ImageNiveau;











void affichePersonnage(Personnage p, Camera cam);

void afficheZombie(Zombie z, Camera cam);

void afficheSquelette(Squelette s, Camera cam);

void affichePlateforme(Plateforme p, Camera cam);

void afficheDecoration(Decoration d, Camera cam);

void afficheBackground(Background bg, Camera cam);

void afficheImageNiveau(ImageNiveau image);

void enleveContourRose(unsigned char *donnees, int largeur, int hauteur);

int checkCollisionEmeraude(Personnage perso, Piece p);

int checkCollision(Personnage perso, Plateforme plat);

void gereCollisionPlateforme(Personnage *perso, Plateforme plat, int *vy, int *jumps, int *coyote);


void afficheEcran(Ecran e, Camera cam);

void gereCollisionEcran(Personnage *perso, Ecran *e, int *vy, int *jumps, int *coyote, int *invincibilityFrame, int *vie);

void gereMobs(Ecran *e);



int checkCollisionEcran(Personnage perso, Ecran e);

Decoration initDecoration(int x1, int y1, int largeur, int hauteur, char *lienTexture);
Decoration initDecorationVide();


int checkCollisionZombie(Personnage p, Zombie z);
int checkCollisionSquelette(Personnage p, Squelette s);

void gereCollisionZombie(Personnage p, Zombie *z, int *inv, int *vies, int *vy);
void gereCollisionSquelette(Personnage p, Squelette *s, int *inv, int *vies, int *vy);




// ECRANS (ajouter à chaque écran)

Ecran initEcran1();
Ecran initEcran2();
Ecran initEcran3();





int main(int argc, char **argv)

{

initialiseGfx(argc, argv);

prepareFenetreGraphique("Minecraft 2D", LargeurFenetre, HauteurFenetre);

lanceBoucleEvenements();

return 0;

}



void cercle(float centreX, float centreY, float rayon)

{

const int Pas = 20;

const double PasAngulaire = 2.*M_PI/Pas;

for (int index = 0; index < Pas; ++index)

{

const double angle = 2.*M_PI*index/Pas;

triangle(centreX, centreY,

centreX+rayon*cos(angle), centreY+rayon*sin(angle),

centreX+rayon*cos(angle+PasAngulaire), centreY+rayon*sin(angle+PasAngulaire));

}

}



void gestionEvenement(EvenementGfx evenement)

{

static bool pleinEcran = false; 

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
            if (pImageFondMenu != NULL) textureFondMenu = pImageFondMenu->donneesRGB;
            
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
                Player.playerPos.y += vyPerso;

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
                        gereMobs(&nv1);
                        break;
                    case 2:
                        gereMobs(&nv2);
                        break;
                    case 3:
                        gereMobs(&nv2);
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
                case 'F':
                case 'f':
                    pleinEcran = !pleinEcran; 
                    if (pleinEcran) modePleinEcran();
                    else redimensionneFenetre(LargeurFenetre, HauteurFenetre);
                    break;
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






void afficheBackground(Background bg, Camera cam) {

    if (bg.texture != NULL) ecrisImage(0, 0, bg.largeur, bg.hauteur, bg.texture);

}

void afficheImageNiveau(ImageNiveau image) {

    if (image.texture != NULL) {
        int x = (LargeurFenetre - image.largeur) / 2;
        int y = HauteurFenetre - image.hauteur - 20;
        ecrisImageTransparente(x, y, image.largeur, image.hauteur, image.texture);
    }

}

void enleveContourRose(unsigned char *donnees, int largeur, int hauteur) {

    if (donnees == NULL) return;

    for (int i = 0; i < largeur * hauteur; i++) {
        unsigned char b = donnees[i * 3];
        unsigned char v = donnees[i * 3 + 1];
        unsigned char r = donnees[i * 3 + 2];

        if (r > 50 && v < 35 && b > 50 && abs(r - b) <= 15) {
            donnees[i * 3] = 254;
            donnees[i * 3 + 1] = 0;
            donnees[i * 3 + 2] = 255;
        }
    }

}







void affichePersonnage(Personnage p, Camera cam) {

    unsigned char *sprite = p.sprites[p.frameActuelle];

    int l = p.largeurs[p.frameActuelle], h = p.hauteurs[p.frameActuelle];

    int drawX = (p.playerPos.x - cam.x) + (p.largeurs[0] / 2 - l / 2), drawY = p.playerPos.y;

    if (sprite != NULL) {

        if (p.regardeADroite) ecrisImageTransparente(drawX, drawY, l, h, sprite);

        else ecrisImageInversee(drawX, drawY, l, h, sprite);

    }

}



void afficheZombie(Zombie z, Camera cam) {
    if (!z.actif) return;
    unsigned char *sprite = z.sprites[z.frameActuelle];

    int l = z.largeurs[z.frameActuelle], h = z.hauteurs[z.frameActuelle];

    if (sprite != NULL) {

        if (!z.regardeADroite) ecrisImageInversee(z.pos.x - cam.x, z.pos.y, l, h, sprite);

        else ecrisImageTransparente(z.pos.x - cam.x, z.pos.y, l, h, sprite);

    }

}



void affichePlateforme(Plateforme p, Camera cam) {

    if (p.texture != NULL) {

        for (int i=0; i<(p.largeur/32); i++) {

            for (int j=0; j<(p.hauteur/32); j++) {

                int posX = (p.coinInferieurGauche.x + 32*i + 16) - cam.x;

                int posY = (p.coinInferieurGauche.y + 32*j + 16) - cam.y; 

                if (posX + 16 >= 0 && posX - 16 <= LargeurFenetre) ecrisImage(posX, posY, 32, 32, p.texture);

            }

        }

    }

}



Plateforme initPlateforme(int x1, int y1, int larg, int haut, char *lienTexture) {

Plateforme p; p.coinInferieurGauche.x = x1; p.coinInferieurGauche.y = y1; p.largeur = larg; p.hauteur = haut; p.texture = NULL;

DonneesImageRGB * img = lisBMPRGB(lienTexture);

    if (img != NULL) p.texture = img->donneesRGB;

return p;

}



int checkCollision(Personnage perso, Plateforme plat) {

    int l = perso.largeurs[0], h = perso.hauteurs[0];

if (perso.playerPos.x + l/4 < plat.coinInferieurGauche.x) return 0;

if (perso.playerPos.x - l/4 > (plat.coinInferieurGauche.x + plat.largeur)) return 0;

if (perso.playerPos.y + h/4 < plat.coinInferieurGauche.y) return 0;

if (perso.playerPos.y - h/4 - 2 > (plat.coinInferieurGauche.y + plat.hauteur)) return 0;

return 1;

}



void gereCollisionPlateforme(Personnage *perso, Plateforme plat, int *vy, int *jumps, int *coyote) {

    if (checkCollision(*perso, plat) == 1) {

        if ((caractereClavier() == 'z') || (caractereClavier() == 'Z')) { if (*vy >= 0) return; }

            *vy = 0; 
            perso->playerPos.y = plat.coinInferieurGauche.y + plat.hauteur + perso->hauteurs[0]/4 + 1; 
            *jumps = 1;
            *coyote = 5;

    }

}



void afficheEcran(Ecran e, Camera cam) {

    for (int i=0; i<MAX_PLATEFORMES; i++) {

        affichePlateforme(e.solides[i], cam);

    }

    for (int i=0; i<MAX_DECORATIONS; i++) {

        afficheDecoration(e.non_solides[i], cam);

    }

    for (int i = 0; i < MAX_EMERAUDES; i++) {

        affichePiece(e.emeraudes[i], cam);

    }

    for (int i=0; i<MAX_ZOMBIES; i++) {
        afficheZombie(e.zombies[i], cam);
    }

    for (int i=0; i<MAX_SQUELETTES; i++) {
        afficheSquelette(e.squelettes[i], cam);
    }

}



void gereCollisionEcran(Personnage *perso, Ecran *e, int *vy, int *jumps, int *coyote, int *invincibilityFrame, int *vie) {

    for (int i=0; i<MAX_PLATEFORMES; i++) {

        gereCollisionPlateforme(perso, e->solides[i], vy, jumps, coyote);

    }

    for (int i=0; i<MAX_ZOMBIES; i++) {
        gereCollisionZombie(*perso, &(e->zombies[i]), invincibilityFrame, vie, vy);
    }

    for (int i=0; i<MAX_SQUELETTES; i++) {
        gereCollisionSquelette(*perso, &(e->squelettes[i]), invincibilityFrame, vie, vy);
    }

}



Plateforme initPlateformeVide() {

Plateforme p;

p.coinInferieurGauche.x = 0;

p.coinInferieurGauche.y = 0;

p.largeur = 0;

p.hauteur = 0;

p.texture = NULL;

return p;

}



int checkCollisionEcran(Personnage perso, Ecran e) {

for (int i=0; i<MAX_PLATEFORMES; i++) {

if (checkCollision(perso, e.solides[i]) == 1) {

return 1;

}

}

return 0;

}



void afficheDecoration(Decoration d, Camera cam) {

if (d.texture != NULL) {

        for (int i=0; i<(d.largeur/32); i++) {

            for (int j=0; j<(d.hauteur/32); j++) {

                int posX = (d.coinInferieurGauche.x + 32*i + 16) - cam.x;

                int posY = (d.coinInferieurGauche.y + 32*j + 16) - cam.y; 

                if (posX + 16 >= 0 && posX - 16 <= LargeurFenetre) ecrisImageTransparente(posX, posY, 32, 32, d.texture);

            }

        }

    }

}



Decoration initDecoration(int x1, int y1, int larg, int haut, char *lienTexture) {

    Decoration d; d.coinInferieurGauche.x = x1; d.coinInferieurGauche.y = y1; d.largeur = larg; d.hauteur = haut; d.texture = NULL;

    DonneesImageRGB * img = lisBMPRGB(lienTexture);

    if (img != NULL) d.texture = img->donneesRGB;

    return d;

}



Decoration initDecorationVide() {

    Decoration d;

    d.coinInferieurGauche.x = 0;

    d.coinInferieurGauche.y = 0;

    d.largeur = 0;

    d.hauteur = 0;

    d.texture = NULL;

    return d;

} 

int checkCollisionEmeraude(Personnage perso, Piece p) {
    if (p.image == NULL) return 0;

    int lPerso = perso.largeurs[perso.frameActuelle], hPerso = perso.hauteurs[perso.frameActuelle];
    
    if (perso.playerPos.x + lPerso/2 < p.pos.x - 16) return 0;
    if (perso.playerPos.x - lPerso/2 > (p.pos.x + 16)) return 0;
    if (perso.playerPos.y + hPerso/2 < p.pos.y - 16) return 0;
    if (perso.playerPos.y - hPerso/2 > (p.pos.y + 16)) return 0;

    return 1;
}

void afficheSquelette(Squelette s, Camera cam) {
    if (!s.actif) return;
    unsigned char *sprite = s.sprites[s.frameActuelle];

    int l = s.largeurs[s.frameActuelle], h = s.hauteurs[s.frameActuelle];

    if (sprite != NULL) {

        if (!s.regardeADroite) ecrisImageInversee(s.pos.x - cam.x, s.pos.y, l, h, sprite);

        else ecrisImageTransparente(s.pos.x - cam.x, s.pos.y, l, h, sprite);

    }

}

void gereMobs(Ecran *e) {
    for (int i = 0; i<MAX_ZOMBIES; i++) {
        if (!e->zombies[i].actif) continue;
        if (e->zombies[i].pos.x > e->zombies[i].origineX + e->zombies[i].range) { e->zombies[i].vx = -2; e->zombies[i].regardeADroite = false; }
        else if (e->zombies[i].pos.x < e->zombies[i].origineX - e->zombies[i].range) { e->zombies[i].vx = 2; e->zombies[i].regardeADroite = false; }
        e->zombies[i].timerAnim++;
        if (e->zombies[i].timerAnim >= 5) {
            e->zombies[i].frameActuelle = (e->zombies[i].frameActuelle + 1) % NB_SPRITES_ZOMBIE;
            e->zombies[i].timerAnim = 0;
        }
    }

    for (int i = 0; i<MAX_SQUELETTES; i++) {
        if (!e->squelettes[i].actif) continue;
        if (e->squelettes[i].pos.x > e->squelettes[i].origineX + e->squelettes[i].range) { e->squelettes[i].vx = -2; e->squelettes[i].regardeADroite = false; }
        else if (e->squelettes[i].pos.x < e->squelettes[i].origineX - e->squelettes[i].range) { e->squelettes[i].vx = 2; e->squelettes[i].regardeADroite = false; }
        e->squelettes[i].timerAnim++;
        if (e->squelettes[i].timerAnim >= 5) {
            e->squelettes[i].frameActuelle = (e->squelettes[i].frameActuelle + 1) % NB_SPRITES_SQUELETTE;
            e->squelettes[i].timerAnim = 0;
        }
    }
    
}

int checkCollisionZombie(Personnage p, Zombie z) {
    if (!z.actif) return 0;
    if (p.playerPos.x < z.pos.x - 22) return 0;
    if (p.playerPos.x > z.pos.x + 22) return 0;
    if (p.playerPos.y + 32 < z.pos.y - 33) return 0;
    puts("Pas trop bas");
    if (p.playerPos.y < z.pos.y + 50) {
        if (p.playerPos.y > z.pos.y) {
            puts("Parfait!");
            return 1;
        }
        else {
            puts("??");
            return -1;
        }
    }
    return -1;
}



int checkCollisionSquelette(Personnage p, Squelette s) {
    if (!s.actif) return 0;
    if (p.playerPos.x < s.pos.x - 22) return 0;
    if (p.playerPos.x > s.pos.x + 22) return 0;
    if (p.playerPos.y + 32 < s.pos.y - 33) return 0;
    if (p.playerPos.y < s.pos.y + 87) {
        if (p.playerPos.y > s.pos.y + 37) {
            return 1;
        }
        else {
            return -1;
        }
    }
    return 0;
}

void gereCollisionZombie(Personnage p, Zombie *z, int *inv, int *vies, int *vy) {
    if (!z->actif) return;
    int c = checkCollisionZombie(p, *z);
    if (c == 1) {
        *vy = 10;
        puts("Détruit les trucs");
        z->actif = false;
        for (int i=0; i<NB_SPRITES_ZOMBIE; i++) {
            z->sprites[i] = NULL;
        }
    }
    else if (c == -1) {
        if (*inv <= 0) {
            (*vies)--;
            *inv = 100;
        }
    }  
}




void gereCollisionSquelette(Personnage p, Squelette *s, int *inv, int *vies, int *vy) {
    if (!s->actif) return;
    int c = checkCollisionSquelette(p, *s);
    if (c == 1) {
        *vy = 10;
        s->actif = false;
        for (int i=0; i<NB_SPRITES_SQUELETTE; i++) {
            s->sprites[i] = NULL;
        }
    }
    else if (c == -1) {
        if (*inv <= 0) {
            (*vies)--;
            *inv = 100;
        }
    }  
}