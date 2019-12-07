#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#include "draw.h"

static int window_height, window_width;

static void on_reshape(int width, int height);
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_special_key_press(int key, int x, int y);

const static float pi = 3.141592653589793;

// Angle and delta for camera
static float phi;
static float delta_phi;

// Angle and delta for ball
static float delta_phi_ball;
static float phi_ball;

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
	glutSpecialFunc(on_special_key_press);

	// Initialize angle of the camera
	phi = pi / 2;
	delta_phi = pi / 90;
	
	// Initialize angle of the ball
	phi_ball = pi / 2;
	// Special delta so the ball could follow camera and stay in front of it
	delta_phi_ball = pi / 88 - pi / 1260;

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

static void on_special_key_press(int key, int x, int y)
{
	switch(key)
	{
		// Moving to the left and increasing the angle
		case GLUT_KEY_LEFT:
			
			phi += delta_phi;
			
			// Boundary for the camera
			if(phi > pi - pi / 18)
				phi = pi - pi / 18;
			
			phi_ball += delta_phi_ball;
			
			// Boundary for the ball
			if(phi_ball > pi - pi / 13)
				phi_ball = pi - pi / 13;
				
			glutPostRedisplay();
			break;

		// Moving to the right and decreasing the angle
		case GLUT_KEY_RIGHT:
		
			phi -= delta_phi;
			
			
			// Boundary for the camera
			if(phi < pi / 18)
				phi = pi / 18;
			
			phi_ball -= delta_phi_ball;
		
			// Boundary for the ball
			if(phi_ball < pi / 13)
				phi_ball = pi / 13;
				
			glutPostRedisplay();
			break;
	}
}

static void on_display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// At the begging you are standing in front of basket
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(
			//0, 25, 10,
			15 * cos(phi), 2, 15 * sin(phi),
			//10, 10, 15,
			//10, 3, 2,
			0, 3, 2.6, // always look at the same point
			0, 1, 0
		);

	/* x - axis */
    glColor3f(1, 0, 0);
        
    glBegin(GL_LINES);
    	glVertex3f(50, 0, 0);
        glVertex3f(-50, 0, 0);
    glEnd();
    
	/* y - axis */
    glColor3f(0, 1, 0);
    
    glBegin(GL_LINES); 
        glVertex3f(0, 50, 0);
        glVertex3f(0, 0, 0);
    glEnd();

    /* z - axis*/
    glColor3f(0, 0, 1);
    
    glBegin(GL_LINES);
        glVertex3f(0, 0, 50);
        glVertex3f(0, 0, 0);
    glEnd();
    
    draw_walls();
    draw_basket();
	

	// Ball and movement of the ball
	
	glPushMatrix();
	
		glTranslatef(11 * cos(phi_ball), 1, 11 * sin(phi_ball));
		draw_ball();

	glPopMatrix();


	glutSwapBuffers();
}
