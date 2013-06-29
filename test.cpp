/*
*	Projet d'infographie
*	Auteur: Olivier Francillon
*	Projet codé sur Linux
*	Ligne de commande: g++ -fpermissive -lGLU -lGL -lglut ShufflePuck.cpp && ./a.out 
*
* _______________________________________________________________________________________________
* Projet en Infographie
* 
* Objectifs
* Permettre à l’étudiant-­‐e de:
* 	• aller plus loin avec OpenGL
* 	• modéliser un petit monde virtuel autonome en 3D
* 	• réaliser un jeu simple par l’animation
* 	• réaliser une interface utilisateur par le menu, la souris et touches clavier
* 	• appliquer plusieurs types de lumières à la scène 3D
* 	• appliquer les textures différentes à chaque objet 3D de la scène
* Cahier de charges
* Chaque étudiant-­‐e :
* 	• programme individuellement et apporte l’entête et des commentaires au code
* 	• défini un petit monde virtuel en 3D, nécessaire pour développer un jeu simple
* 	• réalise un simple jeu non-­‐existante (à imaginer) en utilisant les animations
* 	• utilise le bouton de la souris afin d’enclencher l’animation
* 	• place bien la camera afin d’avoir une visualisation correcte de la scène et ajout 
* 		l’option de changer le point de vue de la scène par les touches clavier
* 	• applique au moins trois types de lumières de la couleur différente chaque une
* 	• ajute l’effet de transparence ou le brouillard
* 	• applique des textures sur tous les objets de la scène
* 	• défini un menu avec les sous-­‐menus permettant de choisir différentes options de
* 	visualisation de la scène comme:
* 		o un affichage en sommets, fil-­‐de-­‐fer et à facettes pleines o activer ou désactiver
* 		chaque lumière séparément 
* 		o activer ou désactiver la transparence (brouillard)
* 		o activer ou désactiver les textures et
* 		o quitter l’application
* 		• se satisfait avec son travail qui réalisera
* Délai: 
* dernière séance du cours, démo en classe.
* _______________________________________________________________________________________________
* 
*/


#include "GL/freeglut.h"
#include "GL/gl.h"
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define MAX_NO_TEXTURES 2
#define TERRAIN_TEXTURE   0
#define BORDURE_TEXTURE   1

GLuint  texture_id[MAX_NO_TEXTURES];

int affichageMode = 0;
float incr = 0.1;
int modeSolide = 0;
int identMenu;
int animation = 0;
int started = 0;
int winner = 0;

/*************************
* Paramètres de la fenètre
*/
int Wwidth = 1200;
int Wheight = 900;

/*************************
* Paramètre de la caméra
*/
float up = 1.3;
float left = 0;
float near = 2.5;

/*************************
* Paramètres de la table
*/
float tableL = 1;
float tableP = 2;

/*************************
* Paramètres des palets
*/ 
float palet1x=0.0;
float palet1z=0.9;
float palet1DX=0.0;
float palet1DZ=0.0;
float paletL = 0.2;
float paletP = 0.1;
float deplacementX, deplacementPX = 0;
float deplacementZ, deplacementPZ = 0;
float maxDroite = tableL/2 - paletL/2;
float maxGauche = -tableL/2 + paletL/2;
int stick = 0;

/*************************
* Paramètres de la balle
*/
float ballSize = 0.02;
float balleX = 0.0, balleZ = 0.2;
float balleDX = 0.0 , balleDZ = 0.0;
float oldBDZ, oldBDX;

/*************************
* Paramètres des lumières
*/

int switch1 = 1; //Switch on/off lumière 1
int switch2 = 1; //Switch on/off lumière 2
int switch3 = 1; //Switch on/off lumière 3

//Paramètres lumière 1
GLfloat lumiere_position[ ]= {3.0, 1.0, 0.0, 1.0};	
GLfloat lum_ambiante[ ]={01, 0.2, 0.2, 0.0};
GLfloat lum_diffuse[ ]={1, 1, 1, 1.0};

//Paramètres lumière 2
GLfloat lumiere_position1[ ]= {0.0, 2.0, -1.0, 1.0};	
GLfloat lum_diffuse1[ ]={1.0, 0.5, 0.5, 1.0};

//Paramètres lumière 3
GLfloat lumiere_position2[ ]= {2.0, 0.0, 2.0, 1.0};	
GLfloat lum_diffuse2[ ]={0.5, 0.5, 1, 1.0};


/*************************
* Paramètres du fog
*/
int switchFog = 1; //Switch on/off fog
GLfloat fogColor[4] ={0.8,0.8,0.8,1.};




void fogInit();
void lightInit();
void menu();

void resetMouse(){
	glutWarpPointer(Wwidth/2, Wheight/2);
	glutWarpPointer(Wwidth/2, Wheight/2);
}
void lookAt(){
	gluLookAt(	left,up,near, 	//Où je suis
				0.0,0.0,0.0, 	//Oû je regarde
				0.0,1.0,0.0);	//Comment je regarde
	
}

void RenderString(float x, float y, void *font, const char* string, float r, float g, float b)
{  
  //char *c;
  glDisable(GL_FOG);
  glDisable(GL_LIGHTING);
  glDisable(GL_TEXTURE_2D);
  glPushMatrix();

  glOrtho(-10,10,-10,10,-10,10);
  glLoadIdentity();
  gluLookAt(	0,0,-1, 	//Où je suis
				0.0,0.0,0.0, 	//Oû je regarde
				0.0,1.0,0.0);	//Comment je regarde
  glColor3f(r, g, b); 
  glRasterPos2f(x, y);

  glutBitmapString(font, string);
  lookAt();
  glPopMatrix();

  fogInit();
  glEnable(GL_LIGHTING);
  glEnable(GL_TEXTURE_2D);

}
void win(int value){
	if(value==1){
		RenderString(0.7,0.4, GLUT_BITMAP_TIMES_ROMAN_24, "You Win", 1,1,1);
	} else if( value ==2){
		RenderString(0.7,0.4, GLUT_BITMAP_TIMES_ROMAN_24, "You Loose", 1,1,1);
	} else{
		RenderString(0.7,0.4, GLUT_BITMAP_TIMES_ROMAN_24, "", 1,1,1);
	}
	
}


void deplacementPalet(){
	palet1DX = deplacementX - deplacementPX;
	palet1x += palet1DX;
	float limite = tableL/2 - paletL/2 -0.02;
	if (palet1x >= limite){
		palet1x = limite;
	} else if (palet1x <= -limite){
		palet1x = -limite;
	}
	deplacementPX = deplacementX;	
	float limiteSup, limiteInf;
	
	
	palet1DZ = deplacementZ - deplacementPZ;
	palet1z += palet1DZ;
	limiteSup = (tableP/6 + paletP/2);
	limiteInf = (tableP/2 - paletP/2);

	if (palet1z >= limiteInf){
		palet1z = limiteInf;
	} else if (palet1z <= limiteSup){
		palet1z = limiteSup;
	}
	deplacementPZ = deplacementZ;	

}



void initGame(int value){
	
	started=0;
	balleX=0.0;
	balleZ = 0.2;
	balleDX=0.0;
	balleDZ=0.0;
	palet1x=0.0;
	palet1z=0.9;
	palet1DZ=0.0;
	palet1DX=0.0;
	deplacementPX = deplacementX;
	deplacementPZ = deplacementZ;
	animation = value;
	if(value == 1){
		glutSetCursor(GLUT_CURSOR_NONE);
		winner = 0;
	}else{
		glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
	}

}

void boudingPalet(float posBX, float posBZ){
	float impactPos, posPaletX,posPaletZ;
	int impact = 0;

	if(balleDZ != 0){
		oldBDZ = balleDZ;
		oldBDX = balleDX;
	}
	
		posPaletX = palet1x;
		posPaletZ = palet1z - paletP/2;

	
	if(((posBZ >= posPaletZ && 
		 posBZ <= posPaletZ + paletP)  &&
		posBX >= posPaletX - paletL/2 &&
		posBX <= posPaletX + paletL/2)){

		if(palet1DZ < 0 && palet1DZ <= balleDZ){
			if(palet1z - balleZ < paletP/2){
				balleZ = palet1z - paletP/2;
			}
			balleDZ = palet1DZ;
			balleDX = palet1DX;
			stick = 1;
		}else if(palet1DZ == 0 && stick != 1){
			balleDZ = palet1DZ;
			balleDX = palet1DX;
			stick = 1;
		}else {
			balleDZ=-(oldBDZ+palet1DZ);
			balleDX=oldBDX+palet1DX;
			stick = 0;
		}
		

		impact = 1;
	}

	if(impact && !stick ){
		
		
			if(balleDZ > 0){
				balleDZ += palet1DZ;
			}else{
				balleDZ -= palet1DZ;
			}
			
				
				balleDX += palet1DX/9 ;
			
		
	}
	
	
}
void deplacementBalle(int start){
	if (start){
		srand ( time(NULL) );
		balleDX = (rand() % 10 )/1000.0;
		balleDZ = ((rand() % 10) + 1 )/1000.0;
	}

	//Collisions bordures
	//Droite
	if (balleX >= tableL/2 - 0.02 - ballSize/2){ balleDX *= -1 ;}
	//Gauche
	if (balleX <= -(tableL/2 - 0.02 - ballSize/2)){ balleDX *= -1 ;}
	
	//Bas
	if (balleZ >= tableP/2){ winner = 2;resetMouse();initGame(0);};
	//Haut avec gestion de l'espace de but
	if (balleZ <= -tableP/2){ 
		if(balleX > -(tableL/6) && balleX < (tableL/6)){
			winner = 1;resetMouse();initGame(0);
		} else {
			balleDZ *= -1;
		}
	}
		
	//Collision palet
	boudingPalet(balleX,balleZ);

	//Déplacement
	balleX += balleDX;
	balleZ += balleDZ;
}

void material(float r, float b, float g){
	
	GLfloat mat_diffuse[ ]={r,b,g,1.0};
	GLfloat mat_specular[ ]={1,1,1,1};
	GLfloat surf_shininess[ ]={30};
	glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diffuse);			//Matérieau Diffu
	glMaterialfv(GL_FRONT,GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS, surf_shininess);


	
}

void texturing(){
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    glEnable(GL_TEXTURE_2D);
}

void chargeTexture (char *nomFichier,				// nom du fichier en format .raw
	int largeur,					// largeur de l'image en pixels
	int hauteur,					// hauteur de l'image en pixels
	int profondeur,					// profondeur de l'image en bytes
	GLenum typeCouleur,				// type de couleur OpenGL à utiliser
	GLenum typeFiltre )				// type de filtre à utiliser
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

	//  construit les mipmaps 
	//  ***********************
	//  * Attention, pour linux, il faut mettre GL_BGR en 5 ème position, sinon le bleu et le rouge
	//  * sont inversé
	gluBuild2DMipmaps ( GL_TEXTURE_2D, typeCouleur, largeur, hauteur,
		GL_BGR, GL_UNSIGNED_BYTE, texture );

	//  libère la mémoire
	free ( texture );
}

//Génération d'un cube avec UV
 void drawBox(GLfloat size, GLfloat repeatTextU,GLfloat repeatTextV )
{
   GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
   GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {3, 2, 6, 7},
    {7, 6, 5, 4},
    {4, 5, 1, 0},
    {5, 6, 2, 1},
    {7, 4, 0, 3}
  };
  GLfloat v[8][3];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

  for (i = 5; i >= 0; i--) {
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);
    glTexCoord2f(0, 0);						glVertex3fv(&v[faces[i][0]][0]);
    glTexCoord2f(repeatTextU, 0);			glVertex3fv(&v[faces[i][1]][0]);
    glTexCoord2f(repeatTextU, repeatTextV);	glVertex3fv(&v[faces[i][2]][0]);
    glTexCoord2f(0, repeatTextV);			glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}



void plateau()
{
	glPushMatrix();
	glScalef(tableL,0.02,-tableP);
	drawBox(1,1,1);
	glPopMatrix();

}

//Dessin de bordure position x,y,z et dimension longueur, hauteur, profondeur
void bordure(float x, float y, float z, float l, float h, float p, float u, float v)
{
	//material(0,0,1);
	glPushMatrix();
	glTranslatef(x,y,z);
	glScalef(l,h,p);
	drawBox(1,u,v);
	glPopMatrix();
}

void tableJeu(){

	glPushMatrix();

	material(1,1,1);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	chargeTexture ( "recto.bmp",128, 128, 3, GL_RGB, GL_LINEAR);
	plateau();

	chargeTexture ( "recto.bmp",128, 128, 3, GL_RGB, GL_LINEAR );
	bordure(-0.49,0.06,0,0.02,0.1,-2,8,1);
	bordure(0.49,0.06,0,0.02,0.1,-2,8,1);
	chargeTexture ( "recto.bmp",128, 128, 3, GL_RGB, GL_LINEAR );
	bordure(tableL/3,0.06,-(tableP/2)-0.01,tableL/3,0.1,0.02,.5,2);
	bordure(-tableL/3,0.06,-(tableP/2)-0.01,tableL/3,0.1,0.02,.5,2);
	glPopMatrix();
}

void paletJoueur1(float x, float y, float z){
	float largeur = 0.2;
	float hauteur = 0.05;
	float profondeur = 0.1;
	material(.5,.5,.5);
	glPushMatrix();
	glTranslatef(x,y + hauteur/2,z);
	glScalef(largeur,hauteur,profondeur);
	drawBox(1,1,1);
	glPopMatrix();
}



void balle(float x, float y, float z){
	material(1,0.2,0.2);
	glPushMatrix();
	glTranslatef(x,0.03,z);
	glutSolidSphere(ballSize,10,10);
	glPopMatrix();

}

void axes(){
	glLineWidth(5);
        material(1,0,0);
        glBegin(GL_LINES);   
          glVertex3f(0.0,0.0,0.0); 
          glVertex3f(1.0,0.0,0.0); 
        glEnd();
        material(0,1,0);
        glBegin(GL_LINES);   
          glVertex3f(0.0,0.0,0.0); 
          glVertex3f(0.0,1.0,0.0); 
        glEnd();
        material(0,0,1);
        glBegin(GL_LINES);   
          glVertex3f(0.0,0.0,0.0); 
          glVertex3f(0.0,0.0,1.0); 
        glEnd();
        glLineWidth(1);
}


void light(){

	glLightfv(GL_LIGHT0, GL_POSITION, lumiere_position);  	//Position Lumière
	glLightfv(GL_LIGHT0, GL_AMBIENT, lum_ambiante);			//Lumière ambiante
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lum_diffuse);			//Lumière diffuse

	glLightfv(GL_LIGHT1, GL_POSITION, lumiere_position1);  	//Position Lumière
	//glLightfv(GL_LIGHT1, GL_AMBIENT, lum_ambiante1);			//Lumière ambiante
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lum_diffuse1);			//Lumière diffuse

	glLightfv(GL_LIGHT2, GL_POSITION, lumiere_position2);  	//Position Lumière
	//glLightfv(GL_LIGHT2, GL_AMBIENT, lum_ambiante2);			//Lumière ambiante
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lum_diffuse2);			//Lumière diffuse

}



void fog(){

	//enclenchereffetbrouillard
	glFogi(GL_FOG_MODE,GL_EXP);
	glFogfv(GL_FOG_COLOR,fogColor);
	//la couleur du brouillard
	glFogf(GL_FOG_DENSITY, 0.10);
	//la densité du bruillard
}





void display(){		
	int i;		

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);		// efface contenu de la fenêtre (couleur et profondeur)
	glCullFace(GL_BACK);						//Enclenche le back face culling
	glEnable(GL_DEPTH_TEST);					//Enclenche la estion de la profondeur
	//axes();					//Axes pour la conception/debug
	
	tableJeu();					//Dessin de la table de jeu
	glDisable(GL_TEXTURE_2D);	//Pas de textures sur les éléments suivant
	paletJoueur1(palet1x,0,palet1z);	//Dessin du palet
	balle(balleX,0,balleZ);				//Dessin de la balle
	
	glEnable(GL_TEXTURE_2D);
					//Dépile la matrice des coordonnées
	light();							//Ajout des lumières
	fog();								//Ajout du fog
	win(winner);
	
	glutSwapBuffers(); 			//Echange les buffers
 
}


void reshape(int w, int h) // fenétre pour l'affichage de la scène 3D
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	Wwidth=glutGet(GLUT_SCREEN_WIDTH);
	Wheight=glutGet(GLUT_SCREEN_HEIGHT);

	//Projection perspective
	gluPerspective	(	50.0,		// angle d'ouverture
						(float)Wwidth/(float)Wheight,	// rapport
						0.1,		// distance avant-plan
						1000.0);	// distance arrière-plan

	glMatrixMode (GL_MODELVIEW);

}

void lightInit(){

	if(switch1){
		glEnable(GL_LIGHT0);
	}else{
		glDisable(GL_LIGHT0);
	}
	if(switch2){
		glEnable(GL_LIGHT1);
	}else{
		glDisable(GL_LIGHT1);
	}
	if(switch3){
		glEnable(GL_LIGHT2);
	}else{
		glDisable(GL_LIGHT2);
	}
}

void fogInit(){
	if(switchFog){
		glEnable(GL_FOG);
	}else{
		glDisable(GL_FOG);
	}
}
void init(void){
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(0.0, 0.0, 0.0, 0.0);		// Fond Blanc
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_NORMALIZE);
	lightInit();
	fogInit();
	texturing();
	glMatrixMode(GL_MODELVIEW);					// matrice de modélisation
	glLoadIdentity();							// matrice d'identité
	if(affichageMode == 0){
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL); 	//Mode Polygone
	}else if(affichageMode == 1){
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);	//Mode Ligne
	}else{
		glPolygonMode(GL_FRONT_AND_BACK,GL_POINT); 	//Mode Point
	}
	lookAt();
	menu();								// Apelle la fonction du menu
	
}


void gestionMenu(int value)
{
  switch(value)		//Gestion des menus
  {
	case 1: 
		initGame(1); 
		animation = 1; 
		break;
	case 2: 
		initGame(1);
		animation = 0; 
		break;
	case 3: 
		left = 0;
		up = 1.3;
		near = 2.5;
		//init();
		break;
	case 4: 
		left = 0;
		up = 3;
		near = 0.1;
		//init();
		break;
	case 5:
		affichageMode = 0;
		//init();
		break;
	case 6:
		affichageMode = 1;
		//init();
		break;
	case 7:
		affichageMode = 2;
		//init();
		break;
	case 8:
		if(switch1){
			switch1 = 0;
		}else{
			switch1 = 1;
		}
		//lightInit();
		break;
	case 9:
		if(switch2){
			switch2 = 0;
		}else{
			switch2 = 1;
		}
		//lightInit();
		break;
	case 10:
		if(switch3){
			switch3 = 0;
		}else{
			switch3 = 1;
		}
		//lightInit();
		break;
	case 11:
		if(switchFog){
			switchFog = 0;
		}else{
			switchFog = 1;
		}
		//fogInit();
		break;
	case 12: 
		exit(0); 
		break;

	
  }
  init();
  glutPostRedisplay();
}

void menu(){
	identMenu=glutCreateMenu(gestionMenu); //Ajout des menus
	glutAddMenuEntry("Start/restart game (1)", 1);
	glutAddMenuEntry("pause game (2)", 2);
	glutAddMenuEntry("Camera 3D (3)", 3);
	glutAddMenuEntry("Camera top-down (4)", 4);
	glutAddMenuEntry("Mode Solide",5);
	glutAddMenuEntry("Mode Ligne",6);
	glutAddMenuEntry("Mode Point",7);
	if(switch1){
		glutAddMenuEntry("Lumiere 1 OFF",8);
	}else{
		glutAddMenuEntry("Lumiere 1 ON",8);
	}
	if(switch2){
		glutAddMenuEntry("Lumiere 2 OFF",9);
	}else{
		glutAddMenuEntry("Lumiere 2 ON",9);
	}
	if(switch3){
		glutAddMenuEntry("Lumiere 3 OFF",10);
	}else{
		glutAddMenuEntry("Lumiere 3 ON",10);
	}
	if(switchFog){
		glutAddMenuEntry("Fog OFF",11);
	}else{
		glutAddMenuEntry("Fog ON",11);
	}
	glutAddMenuEntry("quitter (q)", 12);
	glutAttachMenu(GLUT_RIGHT_BUTTON); //On attache le menu au bouton gauche de la souris
}

void  keyboard (unsigned char key, int x, int y)
{
    switch (key) { 	  //Traitement des touches
            
        case '1':    
			gestionMenu(1);
            break;
        case '2':    
            gestionMenu(2);
            break;
        case '3':    
			gestionMenu(3);
            break;
        case '4':
			gestionMenu(4);
        	break;
        case 'q':
        case 'Q':
        case 27 :
	        gestionMenu(12);
        	break;
        case 'w':  
        case 'W':
        	up+=0.1;
        	init();
        	break;
        case 's':
        case 'S':
        	up-=0.1;
        	init();
        	break;
        case 'a':
        case 'A':
        	left+=0.1;
        	init();
        	break;
        case 'd':
        case 'D':
        	left-=0.1;
        	init();
        	break;
    }

    glutPostRedisplay(); //Relance l'affichage
}



void mouseMove(int x, int y){

	deplacementX = ((float)x - Wwidth) /((float)(Wwidth/2)-100);
	deplacementZ = ((float)y - Wheight) /((float)(Wheight/2)-100*(Wwidth/Wheight));
}


void anime(void)
{
    
    sleep(.01);
    if(animation){
    	if (!started){
    		started = 1;
    		deplacementPX = deplacementX;
			deplacementPZ = deplacementZ;
    		deplacementBalle(started);	
    	}
    	deplacementPalet();
    	deplacementBalle(!started);

	    
	}
	
	
    glutPostRedisplay();
}
int main(int argc, char** argv)			
{
	glutInit(&argc, argv);				// initialise GLUT et traite les éventuels
	Wwidth=glutGet(GLUT_SCREEN_WIDTH);
	Wheight=glutGet(GLUT_SCREEN_HEIGHT);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_DEPTH);	// double tampon, z-buffer;
	glutInitWindowSize(Wwidth, Wheight);					// largeur et hauteur de la fenêtre
	
	glutInitWindowPosition(0, 0);				// position de la fenêtre
	glutCreateWindow("Shuffle Puck");			// Création d'une fenêtre
	glutPassiveMotionFunc(mouseMove); 

	reshape(1, 1);						// Apelle la fonction de reshape

	init();								// Apelle la fonction d'initialisation
	glutDisplayFunc(display);			// Apelle la fonction d'affichage
	
	glutKeyboardFunc(keyboard);			// Apelle la fonction de gestion du clavier
	glutIdleFunc(anime);				// Apelle la fonction d'animation
	glutMainLoop();						// Lance la boucle
}