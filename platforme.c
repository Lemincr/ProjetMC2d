#include <stdlib.h> // Pour pouvoir utiliser exit()
#include <stdio.h> // Pour pouvoir utiliser printf()
#include <math.h> // Pour pouvoir utiliser sin() et cos()
#include "GfxLib.h" // Seul cet include est necessaire pour faire du graphique
#include "BmpLib.h" // Cet include permet de manipuler des fichiers BMP
#include "ESLib.h" // Pour utiliser valeurAleatoire()
#include "platforme.h"
#include "menu.h"


// Largeur et hauteur de fenetre : definies une seule fois dans platforme.h

#define COTE_PLATEFORME 32
#define NB_SPRITES_MARCHE 5
#define NB_SPRITES_SQUELETTE 4
#define MAX_PLATEFORMES 2000
#define MAX_DECORATIONS 100
#define MAX_EMERAUDES 100

#define HAUTEUR_MAP 19
#define LARGEUR_MAP 500

#define MAX_ZOMBIES 5
#define MAX_SQUELETTES 5





Ecran initEcran1() {
	Ecran e;
	int indexSolides = 0;
	int indexDecos = 0;
	int indexPieces = 0;
	int indexZombies = 0;
	int indexSquelettes = 0;

	// 0 = Ciel (Vide), 1 = Stone, 2 = Dirt, 3 = Grass, 4 = Sand
	static int carte[HAUTEUR_MAP][LARGEUR_MAP] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,16,16,16,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,17,17,16,0,0,0,},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,17,17,16,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,17,17,16,0,0},
		{8,8,8,8,8,10,0,0,0,0,0,0,0,0,0,6,0,0,15,15,15,15,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,0,5,0,0,0,16,16,16,16,0,0},
		{7,7,7,7,7,11,10,0,0,0,0,0,0,0,0,0,0,0,0,6,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,0,0,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,3,3,3,3,3,3,0,0,0,0,0},
		{7,12,7,13,7,9,0,0,0,5,0,0,0,5,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,5,0,3,3,3,0,0,0,0,3,3,3,0,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0},
		{7,7,7,14,7,9,5,0,3,3,3,3,3,3,3,3,5,0,0,0,9,0,0,5,18,0,0,0,0,0,99,0,3,3,3,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0},
		{3,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,3,3,3,3,3,3,3,3,3,0,0,0,0,3,3,3,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0},
		{2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,3,0,0,19,5,0,5,0,0,0,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0},
		{2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,2,2,2,2,2,2,2,2,2,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,3,3,3,3,3,3,3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3,3,3,3,3,3,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0}, 
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,2,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0}, 
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
	};

	// Génération automatique des blocs du jeu à partir de ta matrice
	for (int ligne = 0; ligne < HAUTEUR_MAP; ligne++) {
		for (int col = 0; col < LARGEUR_MAP; col++) {
			int typeBloc = carte[ligne][col];
			
			// Si c'est du ciel (0), on passe pour économiser la mémoire et les performances
			if (typeBloc == 0) continue; 
			
			int posX = col * COTE_PLATEFORME;
			int posY = (HAUTEUR_MAP - 1 - ligne) * COTE_PLATEFORME;
			switch (typeBloc) {
				case 1:
					if (indexSolides < MAX_PLATEFORMES) {
						e.solides[indexSolides++] = initPlateforme(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/stone.bmp");
					}
					break;
				case 2:
					if (indexSolides < MAX_PLATEFORMES) {
						e.solides[indexSolides++] = initPlateforme(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/dirt.bmp");
					}
					break;
				case 3:
					if (indexSolides < MAX_PLATEFORMES) {
						e.solides[indexSolides++] = initPlateforme(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/grass.bmp");
					}
					break;
				case 4:
					if (indexSolides < MAX_PLATEFORMES) {
						e.solides[indexSolides++] = initPlateforme(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/sand.bmp");
					}
					break;
				case 5:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/short_grass.bmp");
					}
					break;
				case 6:
					if (indexPieces < MAX_EMERAUDES) {
						e.emeraudes[indexPieces++] = initPiece(posX, posY, "images/emerald.bmp");
					}
					break;
				case 7:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/bois.bmp");
					}
					break;
				case 8:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/boisNoir.bmp");
					}
					break;
				case 9:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/buche.bmp");
					}
					break;
				case 10:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/escalier.bmp");
					}
					break;
				case 11:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/escalier2.bmp");
					}
					break;
				case 12:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/vitre.bmp");
					}
					break;
				case 13:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/porte1.bmp");
					}
					break;
				case 14:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/porte2.bmp");
					}
					break;
				case 15:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/feuille.bmp");
					}
					break;
				case 16:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/obsidienne.bmp");
					}
					break;
				case 17:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/nether.bmp");
					}
					break;
				case 18:
					if (indexZombies < MAX_ZOMBIES) {
						e.zombies[indexZombies++] = initZombie(posX, posY);
					}
					break;
				case 19:
					if (indexSquelettes < MAX_SQUELETTES) {
						e.squelettes[indexSquelettes++] = initSquelette(posX, posY);
					}
					break;
				case 99:
					if (indexDecos < MAX_DECORATIONS){
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/short_grass.bmp");
					}
					break;
				default:
					break;
			}
		}
	}

	// Remplissage du reste du tableau de blocs vides
	for (int i = indexSolides; i < MAX_PLATEFORMES; i++) {
		e.solides[i] = initPlateformeVide();
	}
	for (int i=indexDecos; i<MAX_DECORATIONS; i++) {
		e.non_solides[i] = initDecorationVide();
	}
	for (int i=indexPieces; i<MAX_EMERAUDES; i++) {
		e.emeraudes[i] = initPieceVide();
	}
	for (int i=indexZombies; i<MAX_ZOMBIES; i++) {
		e.zombies[i] = initZombieVide();
	}
	for (int i=indexSquelettes; i<MAX_SQUELETTES; i++) {
		e.squelettes[i] = initSqueletteVide();
	}
	return e;
}

Ecran initEcran2() {
	Ecran e;
	int indexSolides = 0;
	int indexDecos = 0;
	int indexPieces = 0;
	int indexZombies = 0;
	int indexSquelettes = 0;


	// 0 = Ciel (Vide), 1 = Stone, 2 = Dirt, 3 = Grass, 4 = Sand
	static int carte[HAUTEUR_MAP][LARGEUR_MAP] = {
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{16,16,16,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{16,17,17,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{16,17,17,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0},
		{16,17,17,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0},
		{16,16,16,16,0,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,9,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0},
		{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0},
		{4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,0,0,0,0,0,0,0,6,4,4,0,0,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0},
		{4,4,4,4,4,4,4,4,1,1,1,1,1,1,1,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0}, 
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,0,0,0,0}, 
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,4,4,4,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
	};

	// Génération automatique des blocs du jeu à partir de ta matrice
	for (int ligne = 0; ligne < HAUTEUR_MAP; ligne++) {
		for (int col = 0; col < LARGEUR_MAP; col++) {
			int typeBloc = carte[ligne][col];
			
			// Si c'est du ciel (0), on passe pour économiser la mémoire et les performances
			if (typeBloc == 0) continue; 
			
			int posX = col * COTE_PLATEFORME;
			int posY = (HAUTEUR_MAP - 1 - ligne) * COTE_PLATEFORME;
			switch (typeBloc) {
				case 1:
					if (indexSolides < MAX_PLATEFORMES) {
						e.solides[indexSolides++] = initPlateforme(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/stone.bmp");
					}
					break;
				case 2:
					if (indexSolides < MAX_PLATEFORMES) {
						e.solides[indexSolides++] = initPlateforme(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/dirt.bmp");
					}
					break;
				case 3:
					if (indexSolides < MAX_PLATEFORMES) {
						e.solides[indexSolides++] = initPlateforme(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/grass.bmp");
					}
					break;
				case 4:
					if (indexSolides < MAX_PLATEFORMES) {
						e.solides[indexSolides++] = initPlateforme(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/sand.bmp");
					}
					break;
				case 5:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/short_grass.bmp");
					}
					break;
				case 6:
					if (indexPieces < MAX_EMERAUDES) {
						e.emeraudes[indexPieces++] = initPiece(posX, posY, "images/emerald.bmp");
					}
					break;
				case 16:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/obsidienne.bmp");
					}
					break;
				case 17:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/nether.bmp");
					}
					break;
				default:
					break;
			}
		}
	}

	// Remplissage du reste du tableau de blocs vides
	for (int i = indexSolides; i < MAX_PLATEFORMES; i++) {
		e.solides[i] = initPlateformeVide();
	}
	for (int i=indexDecos; i<MAX_DECORATIONS; i++) {
		e.non_solides[i] = initDecorationVide();
	}
	for (int i=indexPieces; i<MAX_EMERAUDES; i++) {
		e.emeraudes[i] = initPieceVide();
	}
	for (int i=indexZombies; i<MAX_ZOMBIES; i++) {
		e.zombies[i] = initZombieVide();
	}
	for (int i=indexSquelettes; i<MAX_SQUELETTES; i++) {
		e.squelettes[i] = initSqueletteVide();
	}
	return e;
}

Ecran initEcran3() {
	Ecran e;
	int indexSolides = 0;
	int indexDecos = 0;
	int indexPieces = 0;
	int indexZombies = 0;
	int indexSquelettes = 0;

	static int carte[HAUTEUR_MAP][LARGEUR_MAP] = {
		{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,6,6,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,6,6,0,1,1,1,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,6,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,1,1,1,1,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	};
	// Génération automatique des blocs du jeu à partir de ta matrice
	for (int ligne = 0; ligne < HAUTEUR_MAP; ligne++) {
		for (int col = 0; col < LARGEUR_MAP; col++) {
			int typeBloc = carte[ligne][col];
			
			// Si c'est du ciel (0), on passe pour économiser la mémoire et les performances
			if (typeBloc == 0) continue; 
			
			int posX = col * COTE_PLATEFORME;
			int posY = (HAUTEUR_MAP - 1 - ligne) * COTE_PLATEFORME;
			switch (typeBloc) {
				case 1:
					if (indexSolides < MAX_PLATEFORMES) {
						e.solides[indexSolides++] = initPlateforme(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/stone.bmp");
					}
					break;
				case 2:
					if (indexSolides < MAX_PLATEFORMES) {
						e.solides[indexSolides++] = initPlateforme(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/dirt.bmp");
					}
					break;
				case 3:
					if (indexSolides < MAX_PLATEFORMES) {
						e.solides[indexSolides++] = initPlateforme(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/grass.bmp");
					}
					break;
				case 4:
					if (indexSolides < MAX_PLATEFORMES) {
						e.solides[indexSolides++] = initPlateforme(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/sand.bmp");
					}
					break;
				case 5:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/short_grass.bmp");
					}
					break;
				case 6:
					if (indexPieces < MAX_EMERAUDES) {
						e.emeraudes[indexPieces++] = initPiece(posX, posY, "images/emerald.bmp");
					}
					break;
				case 7:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/bois.bmp");
					}
					break;
				case 8:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/boisNoir.bmp");
					}
					break;
				case 9:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/buche.bmp");
					}
					break;
				case 10:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/escalier.bmp");
					}
					break;
				case 11:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/escalier2.bmp");
					}
					break;
				case 12:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/vitre.bmp");
					}
					break;
				case 13:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/porte1.bmp");
					}
					break;
				case 14:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/porte2.bmp");
					}
					break;
				case 15:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/feuille.bmp");
					}
					break;
				case 16:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/obsidienne.bmp");
					}
					break;
				case 17:
					if (indexDecos < MAX_DECORATIONS) {
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/nether.bmp");
					}
					break;
				case 18:
					if (indexZombies < MAX_ZOMBIES) {
						e.zombies[indexZombies++] = initZombie(posX, posY);
					}
					break;
				case 19:
					if (indexSquelettes < MAX_SQUELETTES) {
						e.squelettes[indexSquelettes++] = initSquelette(posX, posY);
					}
					break;
				case 99:
					if (indexDecos < MAX_DECORATIONS){
						e.non_solides[indexDecos++] = initDecoration(posX, posY, COTE_PLATEFORME, COTE_PLATEFORME, "images/short_grass.bmp");
					}
					break;
				default:
					break;
			}
		}
	}

	// Remplissage du reste du tableau de blocs vides
	for (int i = indexSolides; i < MAX_PLATEFORMES; i++) {
		e.solides[i] = initPlateformeVide();
	}
	for (int i=indexDecos; i<MAX_DECORATIONS; i++) {
		e.non_solides[i] = initDecorationVide();
	}
	for (int i=indexPieces; i<MAX_EMERAUDES; i++) {
		e.emeraudes[i] = initPieceVide();
	}
	for (int i=indexZombies; i<MAX_ZOMBIES; i++) {
		e.zombies[i] = initZombieVide();
	}
	for (int i=indexSquelettes; i<MAX_SQUELETTES; i++) {
		e.squelettes[i] = initSqueletteVide();
	}
    return e; 
}

Plateforme initPlateforme(int x1, int y1, int larg, int haut, char *lienTexture) {
	Plateforme p; 
	p.coinInferieurGauche.x = x1; 
	p.coinInferieurGauche.y = y1; 
	p.largeur = larg; 
	p.hauteur = haut; 
	p.texture = NULL;
	DonneesImageRGB * img = lisBMPRGB(lienTexture);
	    if (img != NULL) p.texture = img->donneesRGB;
	return p;
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

Decoration initDecoration(int x1, int y1, int larg, int haut, char *lienTexture) {
    Decoration d; 
    d.coinInferieurGauche.x = x1; 
    d.coinInferieurGauche.y = y1; 
    d.largeur = larg; 
    d.hauteur = haut; 
    d.texture = NULL;
    DonneesImageRGB * img = lisBMPRGB(lienTexture);
    if (img != NULL) {
    	d.texture = img->donneesRGB;
    }
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


Piece initPiece(int x1, int y1, char *lienTexture) {
	Piece p;
	p.pos.x = x1;
	p.pos.y = y1;
	DonneesImageRGB * img = lisBMPRGB(lienTexture);
    if (img != NULL) {
    	p.image = img->donneesRGB;
    }
    return p;
}

Piece initPieceVide() {
	Piece p;
	p.pos.x = 0;
	p.pos.y = 0;
	p.image = NULL;
	return p;
}

Zombie initZombie(int x1, int y1) {
    Zombie z;
    char chemin[50];
    z.pos.x = x1;
    z.pos.y = y1+16;
    z.origineX = x1; 
    z.range = 200; 
    z.vx = 2;
    z.frameActuelle = 0;
    z.timerAnim = 0; 
    z.regardeADroite = false;
    z.actif = true;
    for (int i = 0; i < NB_SPRITES_ZOMBIE; i++) {
        if (i == 0) sprintf(chemin, "images/zombie.bmp");
        else sprintf(chemin, "images/zombie%d.bmp", i + 1);
        DonneesImageRGB *img = lisBMPRGB(chemin);
        if (img != NULL) {
            z.sprites[i] = img->donneesRGB;
            z.largeurs[i] = img->largeurImage;
            z.hauteurs[i] = img->hauteurImage;
            
        }
    }
    return z;
}

Zombie initZombieVide() {
	Zombie z;
    z.pos.x = 0;
    z.pos.y = 0;
    z.origineX = 0; 
    z.range = 0; 
    z.vx = 0;
    z.frameActuelle = 0;
    z.timerAnim = 0; 
    z.regardeADroite = false;
    z.actif = false;
    for (int i = 0; i < NB_SPRITES_ZOMBIE; i++) {
        z.sprites[i] = NULL;
        z.largeurs[i] = 0;
        z.hauteurs[i] = 0;
    }
    return z;
}

Squelette initSquelette(int x1, int y1) {
    Squelette s;
    char chemin[50];
    s.pos.x = x1;
    s.pos.y = y1+16;
    s.origineX = x1; 
    s.range = 200; 
    s.vx = 2;
    s.frameActuelle = 0;
    s.timerAnim = 0; 
    s.regardeADroite = false;
    s.actif = true;
    for (int i = 0; i < NB_SPRITES_SQUELETTE; i++) {
        if (i == 0) sprintf(chemin, "images/squelette.bmp");
        else sprintf(chemin, "images/squelette%d.bmp", i+1);
        DonneesImageRGB *img = lisBMPRGB(chemin);
        if (img != NULL) {
            s.sprites[i] = img->donneesRGB;
            s.largeurs[i] = img->largeurImage;
            s.hauteurs[i] = img->hauteurImage;
            printf("%dx%d\n", img->largeurImage, img->hauteurImage);
        }
    }
    return s;
}

Squelette initSqueletteVide() {
	Squelette s;
    s.pos.x = 0;
    s.pos.y = 0;
    s.origineX = 0;
    s.range = 0; 
    s.vx = 0;
    s.frameActuelle = 0;
    s.timerAnim = 0; 
    s.regardeADroite = false;
    s.actif = false;
    for (int i = 0; i < NB_SPRITES_SQUELETTE; i++) {
        s.sprites[i] = NULL;
        s.largeurs[i] = 0;
        s.hauteurs[i] = 0;
    }
    return s;
}

void ecrisImageInversee(int x, int y, int largeur, int hauteur, const unsigned char *donnees) {
    unsigned char *pixels = (unsigned char*)malloc(largeur * hauteur * 4);
    if (pixels == NULL) return;
    for (int j = 0; j < hauteur; j++) {
        for (int i = 0; i < largeur; i++) {
            int indexSource = (j * largeur + (largeur - 1 - i)) * 3;
            int indexDest = (j * largeur + i) * 4;
            unsigned char b = donnees[indexSource], v = donnees[indexSource + 1], r = donnees[indexSource + 2];
            pixels[indexDest] = b; pixels[indexDest + 1] = v; pixels[indexDest + 2] = r;
            if (r == 255 && v == 0 && b == 254) pixels[indexDest + 3] = 0;
            else pixels[indexDest + 3] = 255;
        }
    }
    extern void ecrisImageARVB(int x, int y, int largeur, int hauteur, const int *donneesARVB);
    ecrisImageARVB(x, y, largeur, hauteur, (const int*)pixels);
    free(pixels);
}

void enleveContourRose(unsigned char *donnees, int largeur, int hauteur) {
    if (donnees == NULL) {
    	return;
    }
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
    int l = p.largeurs[p.frameActuelle];
    int h = p.hauteurs[p.frameActuelle];
    int drawX = (p.playerPos.x - cam.x) + (p.largeurs[0] / 2 - l / 2);
    int drawY = p.playerPos.y;
    if (sprite != NULL) {
        if (p.regardeADroite) {
        	ecrisImageTransparente(drawX, drawY, l, h, sprite);
        }
        else {
        	ecrisImageInversee(drawX, drawY, l, h, sprite);
        }
    }
}

void affichePlateforme(Plateforme p, Camera cam) {
    if (p.texture != NULL) {
        for (int i=0; i<(p.largeur/32); i++) {
            for (int j=0; j<(p.hauteur/32); j++) {
                int posX = (p.coinInferieurGauche.x + 32*i + 16) - cam.x;
                int posY = (p.coinInferieurGauche.y + 32*j + 16) - cam.y; 
                if (posX + 16 >= 0 && posX - 16 <= LargeurFenetre) {
                	ecrisImage(posX, posY, 32, 32, p.texture);
                }
            }
        }
    }
}

void afficheDecoration(Decoration d, Camera cam) {
	if (d.texture != NULL) {
	    for (int i=0; i<(d.largeur/32); i++) {
	        for (int j=0; j<(d.hauteur/32); j++) {
	            int posX = (d.coinInferieurGauche.x + 32*i + 16) - cam.x;
	            int posY = (d.coinInferieurGauche.y + 32*j + 16) - cam.y; 
	            if (posX + 16 >= 0 && posX - 16 <= LargeurFenetre) {
	            	ecrisImageTransparente(posX, posY, 32, 32, d.texture);
	            }
	        }
	    }
	}
}

void affichePiece(Piece p, Camera cam) {
	unsigned char *sprite = p.image;
    int l = COTE_PLATEFORME;
    int h = COTE_PLATEFORME;
    int drawX = p.pos.x - cam.x;
    int drawY = p.pos.y - cam.y;
    if (sprite != NULL) {
        ecrisImageInversee(drawX, drawY, l, h, sprite);
    }
}

void afficheZombie(Zombie z, Camera cam) {
    if (!z.actif) {
    	return;
    }
    unsigned char *sprite = z.sprites[z.frameActuelle];
    int l = z.largeurs[z.frameActuelle];
    int h = z.hauteurs[z.frameActuelle];
    if (sprite != NULL) {
        if (z.regardeADroite) {
        	ecrisImageInversee(z.pos.x - cam.x, z.pos.y, l, h, sprite);
        }
        else {
        	ecrisImageTransparente(z.pos.x - cam.x, z.pos.y, l, h, sprite);
        }
    }
}

void afficheSquelette(Squelette s, Camera cam) {
    if (!s.actif) {
    	return;
    } 
    unsigned char *sprite = s.sprites[s.frameActuelle];
    int l = s.largeurs[s.frameActuelle], h = s.hauteurs[s.frameActuelle];
    if (sprite != NULL) {
        if (s.regardeADroite) {
        	ecrisImageInversee(s.pos.x - cam.x, s.pos.y, l, h, sprite);
        }
        else {
        	ecrisImageTransparente(s.pos.x - cam.x, s.pos.y, l, h, sprite);
        }
    }
}

void afficheBackground(Background bg, Camera cam) {
    if (bg.texture != NULL) {
    	ecrisImage(0, 0, bg.largeur, bg.hauteur, bg.texture);
    }
}

void afficheImageNiveau(ImageNiveau image) {
    if (image.texture != NULL) {
        int x = (LargeurFenetre - image.largeur) / 2;
        int y = HauteurFenetre - image.hauteur - 20;
        ecrisImageTransparente(x, y, image.largeur, image.hauteur, image.texture);
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

int checkCollision(Personnage perso, Plateforme plat) {
    int l = perso.largeurs[0], h = perso.hauteurs[0];
	if (perso.playerPos.x + l/4 < plat.coinInferieurGauche.x) {
		return 0;
	}
	if (perso.playerPos.x - l/4 > (plat.coinInferieurGauche.x + plat.largeur)) {
		return 0;
	}
	if (perso.playerPos.y + h/4 < plat.coinInferieurGauche.y) {
		return 0;
	}
	if (perso.playerPos.y - h/4 - 2 > (plat.coinInferieurGauche.y + plat.hauteur)) {
		return 0;
	}
	return 1;
}

int checkCollisionEmeraude(Personnage perso, Piece p) {
    if (p.image == NULL) {
    	return 0;
    }

    int lPerso = perso.largeurs[perso.frameActuelle], hPerso = perso.hauteurs[perso.frameActuelle]; 

    if (perso.playerPos.x + lPerso/2 < p.pos.x - 16) {
    	return 0;
    }
    if (perso.playerPos.x - lPerso/2 > (p.pos.x + 16)) {
    	return 0;
    }
    if (perso.playerPos.y + hPerso/2 < p.pos.y - 16) {
    	return 0;
    }
    if (perso.playerPos.y - hPerso/2 > (p.pos.y + 16)) {
    	return 0;
    }
    return 1;
}

int checkCollisionZombie(Personnage p, Zombie z) {
    if (!z.actif) {
    	return 0;
    }
    if (p.playerPos.x < z.pos.x - 22) {
    	return 0;
    }
    if (p.playerPos.x > z.pos.x + 22) {
    	return 0;
    }
    if (p.playerPos.y + 32 < z.pos.y - 33) {
    	return 0;
    }
    if (p.playerPos.y < z.pos.y + 50) {
        if (p.playerPos.y > z.pos.y) {
            return 1;
        }
        else {
            return -1;
        }
    }
    return -1;
}

int checkCollisionSquelette(Personnage p, Squelette s) {
    if (!s.actif) {
    	return 0;
    }
    if (p.playerPos.x < s.pos.x - 22) {
    	return 0;
    }
    if (p.playerPos.x > s.pos.x + 22) {
    	return 0;
    }
    if (p.playerPos.y + 32 < s.pos.y - 33) {
    	return 0;
    }
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

int checkCollisionEcran(Personnage perso, Ecran e) {
	for (int i=0; i<MAX_PLATEFORMES; i++) {
		if (checkCollision(perso, e.solides[i]) == 1) {
			return 1;
		}
	}
	return 0;
}

void gereCollisionPlateforme(Personnage *perso, Plateforme plat, int *vy, int *jumps, int *coyote) {
    if (checkCollision(*perso, plat) == 1) {
        if ((caractereClavier() == 'z') || (caractereClavier() == 'Z')) { 
        	if (*vy >= 0) {
        		return;
        	}
        }
        *vy = 0; 
        perso->playerPos.y = plat.coinInferieurGauche.y + plat.hauteur + perso->hauteurs[0]/4 + 1; 
        *jumps = 1;
        *coyote = 5;
    }
}

void gereCollisionZombie(Personnage p, Zombie *z, int *inv, int *vies, int *vy) {
    if (!z->actif) {
    	return;
    }
    int c = checkCollisionZombie(p, *z);
    if (c == 1) {
        *vy = 10;
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
    if (!s->actif) {
    	return;
    }
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

void gereMobs(Ecran *e) {
    for (int i = 0; i<MAX_ZOMBIES; i++) {
        if (!e->zombies[i].actif) continue;
        e->zombies[i].pos.x += e->zombies[i].vx;
        if (e->zombies[i].pos.x > e->zombies[i].origineX + e->zombies[i].range) { e->zombies[i].vx = -2; e->zombies[i].regardeADroite = true; }
        else if (e->zombies[i].pos.x < e->zombies[i].origineX - e->zombies[i].range) { e->zombies[i].vx = 2; e->zombies[i].regardeADroite = false; }
        e->zombies[i].timerAnim++;
        if (e->zombies[i].timerAnim >= 5) {
            e->zombies[i].frameActuelle = (e->zombies[i].frameActuelle + 1) % NB_SPRITES_ZOMBIE;
            e->zombies[i].timerAnim = 0;
        }
    }

    for (int i = 0; i<MAX_SQUELETTES; i++) {
        if (!e->squelettes[i].actif) continue;
        e->squelettes[i].pos.x += e->squelettes[i].vx;
        if (e->squelettes[i].pos.x > e->squelettes[i].origineX + e->squelettes[i].range) { e->squelettes[i].vx = -2; e->squelettes[i].regardeADroite = true; }
        else if (e->squelettes[i].pos.x < e->squelettes[i].origineX - e->squelettes[i].range) { e->squelettes[i].vx = 2; e->squelettes[i].regardeADroite = false; }
        e->squelettes[i].timerAnim++;
        if (e->squelettes[i].timerAnim >= 5) {
            e->squelettes[i].frameActuelle = (e->squelettes[i].frameActuelle + 1) % NB_SPRITES_SQUELETTE;
            e->squelettes[i].timerAnim = 0;
        }
    }
    
}






void libereMemoireMonde(Ecran *e) {
    for (int i = 0; i < MAX_PLATEFORMES; i++) {
        if (e->solides[i].texture != NULL) {
            free(e->solides[i].texture);
            e->solides[i].texture = NULL; 
        }
    }
    for (int i = 0; i < MAX_DECORATIONS; i++) {
        if (e->non_solides[i].texture != NULL) {
            free(e->non_solides[i].texture);
            e->non_solides[i].texture = NULL;
        }
    }
    for (int i = 0; i < MAX_EMERAUDES; i++) {
        if (e->emeraudes[i].image != NULL) {
            free(e->emeraudes[i].image);
            e->emeraudes[i].image = NULL;
        }
    }
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        for (int j = 0; j < NB_SPRITES_ZOMBIE; j++) {
            if (e->zombies[i].sprites[j] != NULL) {
                free(e->zombies[i].sprites[j]);
                e->zombies[i].sprites[j] = NULL;
            }
        }
    }
    for (int i = 0; i < MAX_SQUELETTES; i++) {
        for (int j = 0; j < NB_SPRITES_SQUELETTE; j++) {
            if (e->squelettes[i].sprites[j] != NULL) {
                free(e->squelettes[i].sprites[j]);
                e->squelettes[i].sprites[j] = NULL;
            }
        }
    }
}







































































