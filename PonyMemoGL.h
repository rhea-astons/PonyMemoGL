/*
* OpenGL Project
* HEIG-VD IL3
* Raphaël Santos
* github.com/raphsnts/PonyMemoGL
* tested on Linux with compilation command:
* g++ PonyMemoGL.cpp -o PonyMemoGL.o -lglut -lGLU
*/

#include <math.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Structure pour les données de la caméra
typedef struct cameras {
	float pos[3];
	float lookAt[3];
	float lookUp[3];
} camera;


// Fonction d'initialisation des caméras
void initCameras();

// Fonction d'initialisation
void init();

// Fonction d'initialisation des lumières
void lightUpdate();

// Fonction de configuration des lumières
void light();

// Fonction d'initialisation du brouillard
void fogUpdate();

// Fonction de configuration du brouillard
void fog();

// Fonction d'initialisation des textures
void texturing();

// Fonction de définition du matériau
void material(float r, float b, float g);

// Fonction pour le chargement des textures
void loadTexture (const char *nomFichier, int largeur, int hauteur,
					int profondeur, GLenum typeCouleur, GLenum typeFiltre);

// Fonction de configuration de la caméra
void look();

// Fonction de gestion du menu
void gestionMenu(int value);

// Fonction d'initialisation du menu
void menu();

// Traitement des touches clavier
void processKeys(unsigned char key, int x, int y);

// Fonction pour les mouvements de souris
void mouseProcess(int button, int state, int x, int y);

// Affichage des axes
void axes();

// Dessin d'une carte
void drawCard(int n);

// Dessiner toutes les cartes
void drawCards();

//	Fonction de redimensionnement
void reshape(int w1, int h1);

// Fonction pour la selection à la souris
void startPicking();

// Fonction pour le traitement des selections
void processHits2 (GLint hits, GLuint buffer[]);

// Fonction pour l'arret de la selection
void stopPicking();

// Fonction d'affichage
void display();