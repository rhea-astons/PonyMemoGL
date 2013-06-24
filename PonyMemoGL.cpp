#include <GL/glut.h>

int displayMode = 1;
int identMenu;

void init() {
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	gluLookAt(5.0, 3.0, 4.0,		// Eye
			  0.0, 0.0, 0.0,		// Reference
			  0.0, 1.0, 0.0			// direction up vector
			  );
}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(90,				// Field of view angle
    			   1.0,				// aspect ration
    			   0.1,				// zNear
    			   100.0			// zFar
    			   );
    glMatrixMode(GL_MODELVIEW);
}

void idle() {
	glutPostRedisplay();
}

void gestionMenu(int value)
{
  switch(value)
  {
	case 1: displayMode = 1;  break;
	case 2: displayMode = 0;  break;
	case 3: exit(0); break;
  }
  glutPostRedisplay();
}

void  keyboard (unsigned char key, int x, int y)
{
    switch (key) {
        case 's':
        case 'S':
            displayMode = 1;
            break;
        case 'w':
        case 'W':
			displayMode = 0;
            break;
        case 'q':
        case 'Q':
        case 27 :
            exit(0);
            break;
    }
    glutPostRedisplay();
}

void axes() {
	glBegin(GL_LINES);
		//axe x en rouge
		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(10.0, 0.0, 0.0);

		//axe y en bleu
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 10.0, 0.0);
		
		//axe z en vert
		glColor3f(0.0, 1.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
		glVertex3f(0.0, 0.0, 10.0);
	glEnd();
}

void menu(){
	identMenu=glutCreateMenu(gestionMenu);
	glutAddMenuEntry("Solid Display (s)", 1);
	glutAddMenuEntry("Wire Display (w)", 2);
	glutAddMenuEntry("quitter (q)", 3);
	glutAttachMenu(GLUT_LEFT_BUTTON);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);		// efface contenu de la fenêtre (couleur et profondeur)
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glClearColor(1.0, 1.0, 1.0, 0.0);
	axes();

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH);
	glutInitWindowSize(600,600);
    glutInitWindowPosition(100,100);
    glutCreateWindow("PonyMemoryGL");
    
    reshape(1, 1);
    init();
    glutDisplayFunc(display);
    glutIdleFunc(idle);
    menu();
    glutKeyboardFunc(keyboard);
    glutMainLoop();
}