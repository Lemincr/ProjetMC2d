#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()
#include <string.h>

#define LargeurFenetre 1850
#define HauteurFenetre 891

#define COTE_PLATEFORME 32

#define NB_SPRITES_MARCHE 5
#define NB_SPRITES_SQUELETTE 4
#define NB_SPRITES_ZOMBIE 3

#define MAX_PLATEFORMES 2000
#define MAX_DECORATIONS 100
#define MAX_EMERAUDES 100
#define MAX_ZOMBIES 5
#define MAX_SQUELETTES 5

#define HAUTEUR_MAP 19
#define LARGEUR_MAP 500


typedef struct coordonnees {
	int x;
	int y;
} Coord;

typedef struct camera {
    int x;
	int y;
} Camera;


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

typedef struct zombie {
    Coord pos;
    unsigned char *sprites[NB_SPRITES_ZOMBIE];
    int largeurs[NB_SPRITES_ZOMBIE];
    int hauteurs[NB_SPRITES_ZOMBIE];
    int frameActuelle;
    int timerAnim;
    bool regardeADroite;
    int vx;
    int range;
    int origineX;
    bool actif;
} Zombie;


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
    bool actif;
} Squelette;


typedef struct ecran {
	Plateforme solides[MAX_PLATEFORMES];
	Decoration non_solides[MAX_DECORATIONS];
	Piece emeraudes[MAX_EMERAUDES];
	Zombie zombies[MAX_ZOMBIES];
	Squelette squelettes[MAX_SQUELETTES];
} Ecran;

Ecran initEcran1();
Ecran initEcran2();
Ecran initEcran3();

Plateforme initPlateforme(int x1, int y1, int larg, int haut, char *lienTexture);
Plateforme initPlateformeVide();

Decoration initDecoration(int x1, int y1, int largeur, int hauteur, char *lienTexture);
Decoration initDecorationVide();

Piece initPiece(int x1, int y1, char *lienTexture);
Piece initPieceVide();

Zombie initZombie(int x1, int y1);
Zombie initZombieVide();

Squelette initSquelette(int x1, int y1);
Squelette initSqueletteVide();

void ecrisImageInversee(int x, int y, int largeur, int hauteur, const unsigned char *donnees);
void enleveContourRose(unsigned char *donnees, int largeur, int hauteur);

void affichePersonnage(Personnage p, Camera cam);
void affichePlateforme(Plateforme p, Camera cam);
void afficheDecoration(Decoration d, Camera cam);
void affichePiece(Piece p, Camera cam);
void afficheZombie(Zombie z, Camera cam);
void afficheSquelette(Squelette s, Camera cam);
void afficheBackground(Background bg, Camera cam);
void afficheImageNiveau(ImageNiveau image);
void afficheEcran(Ecran e, Camera cam);

int checkCollision(Personnage perso, Plateforme plat);
int checkCollisionEmeraude(Personnage perso, Piece p);
int checkCollisionZombie(Personnage p, Zombie z);
int checkCollisionSquelette(Personnage p, Squelette s);
int checkCollisionEcran(Personnage perso, Ecran e);

void gereCollisionPlateforme(Personnage *perso, Plateforme plat, int *vy, int *jumps, int *coyote);
void gereCollisionZombie(Personnage p, Zombie *z, int *inv, int *vies, int *vy);
void gereCollisionSquelette(Personnage p, Squelette *s, int *inv, int *vies, int *vy);
void gereCollisionEcran(Personnage *perso, Ecran *e, int *vy, int *jumps, int *coyote, int *invincibilityFrame, int *vie);

void gereMobs(Ecran *e);