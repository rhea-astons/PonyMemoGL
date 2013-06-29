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


// HEADERS
#include "PonyMemoGL.h"


// Statut des cartes
#define HIDDEN 0
#define SHOW 1
#define SHOWANDHIDE 2
#define SHOWANDTURNED 3
#define HIDE 4
#define TURNED 5

// Statut des caméras
#define CUSTOMCAM 0
#define NORMALCAM 1
#define CHEATCAM 2

// Statut du picking
#define RENDER					1
#define SELECT					2
#define BUFSIZE 1024



// Paramètres pour le picking
GLuint selectBuf[BUFSIZE];
GLint hits;
int mode = RENDER;
int previousSelection = -1;


// Paramètres de la souris
int cursorX,cursorY;


// Parametres de la fenêtre
int winWidth = 600;
int winHeight = 600;


// Parametres Lumière
bool light1Enabled = true;
GLfloat light1_pos[] = { 0.0,  8.0,  0.0,  1.0};	
GLfloat light1_amb[] = { 1.0,  1.0,  1.0,  0.0};
GLfloat light1_dif[] = { 1.0,  1.0,  1.0,  1.0};

bool light2Enabled = true;
GLfloat light2_pos[] = { -3.0,  8.0, 4.0,  1.0};
GLfloat light2_dif[] = { 0.5,  0.5,  1.0,  1.0};

bool light3Enabled = true;
GLfloat light3_pos[] = { -12.0,  1.0,  20.0,  1.0};	
GLfloat light3_dif[] = { 1.0,  0.5,  0.5,  1.0};


// Parametres brouillard
bool fogEnabled = true;
GLfloat fogColor[] = {0.5, 0.5, 0.5, 1.0};


// Parametres d'affichage
int displayMode = 2;


// Paramètres de Camera
camera normalCam, cheatCam, cam;
int activeCam = NORMALCAM;


// Cartes {numéro, statut, orientation}
int cards[20][3] = {
	{3, 0, 0}, {8, 0, 0}, {7, 0, 0}, {1, 0, 0}, {10, 0, 0},
	{1, 0, 0}, {4, 0, 0}, {6, 0, 0}, {9, 0, 0}, {8, 0, 0},
	{6, 0, 0}, {5, 0, 0}, {2, 0, 0}, {2, 0, 0}, {3, 0, 0},
	{7, 0, 0}, {10, 0, 0}, {9, 0, 0}, {5, 0, 0}, {4, 0, 0}
};



// Fonction d'initialisation des caméras
void initCameras()
{
	normalCam.pos[0] =			 2.0;
	normalCam.pos[1] =			 8.5;
	normalCam.pos[2] =			-6.0;
	normalCam.lookAt[0] =		-3.0;
	normalCam.lookAt[1] =		-3.0;
	normalCam.lookAt[2] =		-6.0;
	normalCam.lookUp[0] =		 0.0;
	normalCam.lookUp[1] =		 1.0;
	normalCam.lookUp[2] =		 0.0;

	cheatCam.pos[0] =			 2.0;
	cheatCam.pos[1] =			-8.5;
	cheatCam.pos[2] =			-6.0;
	cheatCam.lookAt[0] =		-3.0;
	cheatCam.lookAt[1] =		-3.0;
	cheatCam.lookAt[2] =		-6.0;
	cheatCam.lookUp[0] =		 0.0;
	cheatCam.lookUp[1] =		 1.0;
	cheatCam.lookUp[2] =		 0.0;
}



// Fonction d'initialisation
void init()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);

	lightUpdate();
	fogUpdate();
	texturing();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(displayMode == 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	else if(displayMode == 1)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	look();
	menu();
}



// Fonction d'initialisation des lumières
void lightUpdate()
{
	if(light1Enabled)
		glEnable(GL_LIGHT0);
	else
		glDisable(GL_LIGHT0);

	if(light2Enabled)
		glEnable(GL_LIGHT2);
	else
		glDisable(GL_LIGHT2);

	if(light3Enabled)
		glEnable(GL_LIGHT3);
	else
		glDisable(GL_LIGHT3);
}



// Fonction de configuration des lumières
void light()
{
	glLightfv(GL_LIGHT0, GL_POSITION, light1_pos);
	glLightfv(GL_LIGHT0, GL_AMBIENT,  light1_amb);
	glLightfv(GL_LIGHT0, GL_DIFFUSE,  light1_dif);

	glLightfv(GL_LIGHT2, GL_POSITION, light2_pos);
	glLightfv(GL_LIGHT2, GL_DIFFUSE,  light2_dif);

	glLightfv(GL_LIGHT3, GL_POSITION, light3_pos);
	glLightfv(GL_LIGHT3, GL_DIFFUSE,  light3_dif);
}



// Fonction d'initialisation du brouillard
void fogUpdate()
{
	if(fogEnabled)
		glEnable(GL_FOG);
	else
		glDisable(GL_FOG);
}



// Fonction de configuration du brouillard
void fog()
{
	glFogi(GL_FOG_MODE,GL_EXP);
	glFogfv(GL_FOG_COLOR,fogColor);
	glFogf(GL_FOG_DENSITY, 0.10);
}



// Fonction d'initialisation des textures
void texturing()
{
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glEnable(GL_TEXTURE_2D);
}



// Fonction de définition du matériau
void material(float r, float b, float g)
{
	GLfloat mat_diffuse[] = {r, b, g, 1.0};
	GLfloat mat_specular[] = {1, 1, 1, 1};
	GLfloat surf_shininess[] = {30.0};
	glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS, surf_shininess);	
}



// Fonction pour le chargement des textures
void loadTexture (const char *nomFichier, int largeur, int hauteur,
				int profondeur, GLenum typeCouleur, GLenum typeFiltre)
{
	GLubyte *texture ;
	FILE *file;

	if ((file = fopen(nomFichier, "rb"))==NULL )
	{
		printf ( "File Not Found : %s\n", nomFichier );
		exit   ( 1 );
	}

	texture = (GLubyte *) malloc ( largeur * hauteur * profondeur * ( sizeof(GLubyte)) );

	if (texture == NULL)
	{
		printf ( "Cannot allocate memory for texture\n" );
		fclose ( file);
		exit   ( 1 );
	}

	fread  ( texture , largeur * hauteur * profondeur, 1 , file );
	fclose ( file);

	//  définit type de filtrage
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, typeFiltre );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, typeFiltre );

	//  définit environnement
	glTexEnvf ( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	gluBuild2DMipmaps ( GL_TEXTURE_2D, typeCouleur, largeur, hauteur,
		GL_BGR, GL_UNSIGNED_BYTE, texture );

	//  libère la mémoire
	free ( texture );
}



// Fonction de configuration de la caméra
void look()
{
	switch(activeCam) {
		case NORMALCAM:
			cam = normalCam;
			break;
		case CHEATCAM:
			cam = cheatCam;
			break;
		default:
			break;
	}
	gluLookAt(
		cam.pos[0],
		cam.pos[1],
		cam.pos[2],
		cam.lookAt[0],
		cam.lookAt[1],
		cam.lookAt[2],
		cam.lookUp[0],
		cam.lookUp[1],
		cam.lookUp[2]
		);
}



// Fonction de gestion du menu
void gestionMenu(int value)
{
  switch(value)
  {
	case 1:
		displayMode = (displayMode+1)%3;
		break;
	case 2:
		if(light1Enabled)
			light1Enabled = 0;
		else
			light1Enabled = 1;
		break;
	case 3:
		if(light2Enabled)
			light2Enabled = 0;
		else
			light2Enabled = 1;
		break;
	case 4:
		if(light3Enabled)
			light3Enabled = 0;
		else
			light3Enabled = 1;
		break;
	case 5:
		if(fogEnabled)
			fogEnabled = false;
		else
			fogEnabled = true;
		break;
	case 6:
		if(activeCam == NORMALCAM)
			activeCam = CHEATCAM;
		else
			activeCam = NORMALCAM;
		break;
	case 9: exit(0); break;
	default:
		printf("");
		break;
  }
  lightUpdate();
  fogUpdate();
  glutPostRedisplay();
}



// Fonction d'initialisation du menu
void menu()
{
	glutCreateMenu(gestionMenu);
	glutAddMenuEntry("Switch Display Mode (m)", 1);
	glutAddMenuEntry("Light 1 ON/OFF (1)",2);
	glutAddMenuEntry("Light 2 ON/OFF (2)",3);
	glutAddMenuEntry("Light 3 ON/OFF (3)",4);
	glutAddMenuEntry("Fog ON/OFF (f)",5);
	glutAddMenuEntry("Change view (c)",6);
	glutAddMenuEntry("quitter (q)", 9);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}



// Traitement des touches clavier
void processKeys(unsigned char key, int x, int y)
{
	switch (key) {
		case 27:
			exit(0);
			break;
		case 'w':  
        case 'W':
        	cam.pos[1] += 0.5;
        	activeCam = CUSTOMCAM;
        	//init();
        	break;
        case 's':
        case 'S':
        	cam.pos[1] -= 0.5;
        	activeCam = CUSTOMCAM;
        	//init();
        	break;
        case 'a':
        case 'A':
        	cam.pos[2] += 0.5;
        	activeCam = CUSTOMCAM;
        	//init();
        	break;
        case 'd':
        case 'D':
        	cam.pos[2] -= 0.5;
        	activeCam = CUSTOMCAM;
        	//init();
        	break;
        case 'm':
        	displayMode = (displayMode+1)%3;
			//init();
			break;
		case 'c':
			if(activeCam == NORMALCAM)
				activeCam = CHEATCAM;
			else
				activeCam = NORMALCAM;
			break;
		case 'f':
			if(fogEnabled)
				fogEnabled = false;
			else
				fogEnabled = true;
		break;
        default:
        	//processKeyboard(key, x, y);
        	break;
	}
	glutPostRedisplay();
}



// Fonction pour les mouvements de souris
void mouseProcess(int button, int state, int x, int y)
{
	if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)
		return;
	cursorX = x;
	cursorY = y;
	mode = SELECT;
}



// Affichage des axes
void axes()
{
	glBegin(GL_LINES);
		//axe x en rouge
		material(1,0,0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);

		//axe y en bleu
		material(0,1,0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);
		
		//axe z en vert
		material(0,0,1);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);
	glEnd();
}



// Dessin d'une carte
void drawCard(int n)
{
	static GLfloat sommets[8][3]=
	{
		{ -1.0,  0.0, -1.0},		/* sommet 0 */
		{ -1.0,  0.0,  1.0},		/* sommet 1 */
		{  1.0,  0.0,  1.0},		/* sommet 2 */
		{  1.0,  0.0, -1.0},		/* sommet 3 */
		{ -1.0, -0.2, -1.0},		/* sommet 4 */
		{ -1.0, -0.2,  1.0},		/* sommet 5 */
		{  1.0, -0.2,  1.0},		/* sommet 6 */
		{  1.0, -0.2, -1.0},		/* sommet 7 */
	};

	static GLubyte facettes[6][4] = 
	{
		{ 0,1,2,3 }, { 7,6,5,4 }, { 3,2,6,7 },
		{ 0,4,5,1 }, { 2,1,5,6 }, { 3,7,4,0 }
	};

	// face
	char filename[10];
	sprintf(filename, "textures/pony%d.bmp", n);
	loadTexture( filename, 256, 256, 3, GL_RGB, GL_LINEAR);
	glBegin(GL_POLYGON);
		glTexCoord2f(0, 0);glVertex3fv(sommets[facettes[1][0]]);
		glTexCoord2f(1, 0);glVertex3fv(sommets[facettes[1][1]]);
		glTexCoord2f(1, 1);glVertex3fv(sommets[facettes[1][2]]);
		glTexCoord2f(0, 1);glVertex3fv(sommets[facettes[1][3]]);
	glEnd();

	// dos
	loadTexture( "textures/recto.bmp",256, 256, 3, GL_RGB, GL_LINEAR);
	glBegin(GL_POLYGON);
		glTexCoord2f(1, 1);glVertex3fv(sommets[facettes[0][0]]);
		glTexCoord2f(0, 1);glVertex3fv(sommets[facettes[0][1]]);
		glTexCoord2f(0, 0);glVertex3fv(sommets[facettes[0][2]]);
		glTexCoord2f(1, 0);glVertex3fv(sommets[facettes[0][3]]);
	glEnd();

	//glDisable(GL_TEXTURE_2D);
	//material(1.0, 1.0, 1.0);
	//glColor3f(1.0, 1.0, 1.0);
	//loadTexture( "textures/recto.bmp",256, 256, 3, GL_RGB, GL_LINEAR);
	
	// autres faces
	for (int i=2; i< 6; i++)
	{
		glBegin(GL_POLYGON);
			glVertex3fv(sommets[facettes[i][0]]);
			glVertex3fv(sommets[facettes[i][1]]);
			glVertex3fv(sommets[facettes[i][2]]);
			glVertex3fv(sommets[facettes[i][3]]);
		glEnd();
	}
	glEnable(GL_TEXTURE_2D);
}



// Dessiner toutes les cartes
void drawCards()
{
	glInitNames();
	for (int i = 0; i <= 3 ; i++) {
		for (int j = 0; j <= 4; j++) {
			int cardNB = 5*i+j;
			glPushMatrix();
				material(1.0, 1.0, 1.0);
				glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
				glPushMatrix();
					glPushName(cardNB);
					glTranslatef(0.0+i*-3, 0.0, 0.0+j*-3);
					switch (cards[cardNB][1]) {
						case SHOW:
							if (cards[cardNB][2] >= 180) {
								cards[cardNB][2] = 180;
								cards[cardNB][1] = TURNED;
							} else {
								glPushMatrix();
								cards[cardNB][2] += 5;
								//glTranslatef(0.0, 0.2, 0.0);
								glRotatef(cards[cardNB][2], 1.0, 0.0, 0.0);
							}
							break;
						case SHOWANDHIDE:
							if (cards[cardNB][2] >= 180) {
								cards[cardNB][2] = 180;
								cards[cardNB][1] = HIDE;
							} else {
								glPushMatrix();
								cards[cardNB][2] += 5;
								//glTranslatef(0.0, -0.2, 0.0);
								glRotatef(cards[cardNB][2], 1.0, 0.0, 0.0);
							}
							break;
						case SHOWANDTURNED:
							if (cards[cardNB][2] >= 180) {
								cards[cardNB][2] = 180;
								cards[cardNB][1] = TURNED;
							} else {
								glPushMatrix();
								cards[cardNB][2] += 5;
								//glTranslatef(0.0, -0.2, 0.0);
								glRotatef(cards[cardNB][2], 1.0, 0.0, 0.0);
							}
							break;
						case HIDE:
							if (cards[cardNB][2] <= 0) {
								cards[cardNB][2] = 0;
								cards[cardNB][1] = HIDDEN;
							} else {
								glPushMatrix();
								cards[cardNB][2] -= 5;
								//glTranslatef(0.0, -0.2, 0.0);
								glRotatef(cards[cardNB][2], 1.0, 0.0, 0.0);
							}
							break;
						default:
							//glTranslatef(0.0, -0.2, 0.0);
							glRotatef(cards[cardNB][2], 1.0, 0.0, 0.0);
							break;
					}
					drawCard(cards[cardNB][0]);
					glPopMatrix();
					glPopName();
				glPopMatrix();
			glPopMatrix();
		}
	}
}



//	Fonction de redimensionnement
void reshape(int w1, int h1)
{
	float ratio;

	winHeight = h1;
	winWidth = w1;

	// Eviter la division par 0
	if(winHeight == 0)
		winHeight = 1;

	ratio = 1.0f * winWidth / winHeight;

	// Set the viewport to be the entire window
    glViewport(0, 0, winWidth, winHeight);

	// Reset the coordinate system before modifying
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Set the clipping volume
	gluPerspective(90, ratio, 0.1, 1000);

	// setting the camera now
	glMatrixMode(GL_MODELVIEW);
}



// Fonction pour la selection à la souris
void startPicking()
{
	GLint viewport[4];
	float ratio;

	glSelectBuffer(BUFSIZE,selectBuf);

	glGetIntegerv(GL_VIEWPORT,viewport);

	glRenderMode(GL_SELECT);

	glInitNames();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix(cursorX,viewport[3]-cursorY,5,5,viewport);
	ratio = (viewport[2]+0.0) / viewport[3];
	gluPerspective(90,ratio,0.1,1000);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}



// Fonction pour le traitement des selections
void processHits2 (GLint hits, GLuint buffer[])
{
	GLint i, j, numberOfNames;
	GLuint names, *ptr, minZ,*ptrNames;

	ptr = (GLuint *) buffer;
	minZ = 0xffffffff;
	for (i = 0; i < hits; i++) {	
		names = *ptr;
		ptr++;
		if (*ptr < minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr+2;
		}

		ptr += names+2;
	}
	if (numberOfNames > 0) {
		ptr = ptrNames;
		int selectedCard;
		for (j = 0; j < numberOfNames; j++,ptr++) {
			selectedCard = *ptr;
		}
		if (previousSelection < 0) {
			previousSelection = selectedCard;
			cards[selectedCard][1] = SHOW;
			printf("Card: %d, ", selectedCard);
		} else if (previousSelection != selectedCard) {
			cards[selectedCard][1] = SHOW;
			printf("card: %d: ", selectedCard);
			if (cards[selectedCard][0] == cards[previousSelection][0]) {
				printf("GOOD!\n");
				cards[previousSelection][1] = TURNED;
				cards[selectedCard][1] = SHOWANDTURNED;
			} else {
				printf("Too bad!\n");
				cards[previousSelection][1] = HIDE;
				cards[selectedCard][1] = SHOWANDHIDE;
			}
			previousSelection = -1;
		}
	}
}



// Fonction pour l'arret de la selection
void stopPicking()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();
	hits = glRenderMode(GL_RENDER);
	if (hits != 0){
		processHits2(hits,selectBuf);
	}
	mode = RENDER;
}



// Fonction d'affichage
void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

	if (mode == SELECT) {
		startPicking();
	}

	glLoadIdentity();
	look();
	lightUpdate();

	axes();
	drawCards();
	//test();
	glLoadIdentity();
	fog();
	light();


	if (mode == SELECT) 
		stopPicking();
	else
		glutSwapBuffers();
}



int main(int argc, char** argv)
{
	glutInit(&argc, argv);	// Initialisation GLUT et traitememt des paramètres
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);	// Double tampon et z-buffer
	glutInitWindowPosition(100, 100);				// Position de la fenêtre
	glutInitWindowSize(winWidth, winHeight);		// Taille de la fenêtre
	glutCreateWindow("PonyMemoGL");					// Création de la fenêtre

	initCameras();	// Appel de la fonction d'initialisation des caméras
	init();							// Appel de la fonction d'initialisation
	glutDisplayFunc(display);		// Appel de la fonction d'affichage
	glutKeyboardFunc(processKeys);	// Appel de la fonction de traitement du clavier
	glutMouseFunc(mouseProcess);	// Appel de la fonction de traitement de la souris
	glutReshapeFunc(reshape);		// Appel de la fonction de redimensionnement
	glutIdleFunc(display);			// Appel de la fonction d'attente
	glutMainLoop();					// Lancement de la boucle
}