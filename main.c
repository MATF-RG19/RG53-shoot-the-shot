#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

static int window_height, window_width;

static void on_reshape(int width, int height);
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Shoot the shot");
	
	glutFullScreen();
	
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keyboard);
	glutDisplayFunc(on_display);

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST);

	glutMainLoop();

	return 0;
}

static void on_reshape(int width, int height)
{
	window_width = width;
	window_height = height;
	
	glViewport(0, 0, window_width, window_height);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(
			60,
			window_width / (float)window_height,
			0.01, 100);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
			3, 2, 7,
			0, 3, 0,
			0, 1, 0
		);
}

static void on_keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:
			exit(0);
			break;
	}
}

static void on_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* x - osa */
	glPushMatrix();
        glColor3f(1, 0, 0);
        
        glBegin(GL_LINES);
            glVertex3f(50, 0, 0);
            glVertex3f(-50, 0, 0);
        glEnd();
        
    glPopMatrix();
    
    /* y - axis */
    glPushMatrix();
        glColor3f(0, 1, 0);
        
        glBegin(GL_LINES); 
            glVertex3f(0, 50, 0);
            glVertex3f(0, -50, 0);
        glEnd();
        
    glPopMatrix();
    
    /* z - axis*/
    glPushMatrix();
        glColor3f(0, 0, 1);
        
        glBegin(GL_LINES);
            glVertex3f(0, 0, 50);
            glVertex3f(0, 0, -50);
        glEnd();
        
    glPopMatrix();

	glutSwapBuffers();
}
