#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <unistd.h>

#include "draw.h"
//#include "shooting.h"

#define TIMER_ID 0
#define TIMER_INTERVAL 40

// Current coordinates of the ball
static float x_curr, y_curr, z_curr; 

// Speed of the ball
static float v_x, v_y, v_z;

static int window_height, window_width;

static void on_reshape(int width, int height);
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_special_key_press(int key, int x, int y);
static void on_timer(int value);

const static float pi = 3.141592653589793;

// Angle and delta for camera
static float phi;
//static float delta_phi;

// Angle and delta for ball
static float phi_ball;
//static float delta_phi_ball;

// Shoot
static int shoot = 0;

// Position of the ball
static int position = 0;

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
	//delta_phi = pi / 4; SPECIAL
	
	// Initialize angle of the ball
	phi_ball = pi / 2;
	// Special delta so the ball could follow camera and stay in front of it
	//delta_phi_ball = pi / 4 - pi / 20;

	x_curr = 11 * cos(phi_ball);
	y_curr = 1;
	z_curr = 11 * sin(phi_ball);
	
	v_x = 0;
	v_y = 0.4;
	v_z = -0.5;

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
		
		case 's':
		case 'S':
			if(!shoot)
			{
				glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
				shoot = 1;
			}
			
			break;
	}
}

static void on_special_key_press(int key, int x, int y)
{
	switch(key)
	{
		// Moving to the left and increasing the angle
		case GLUT_KEY_LEFT:
			
			shoot = 0;
			position -= 1;
			
			if(position < -2)
				position = -2;
			
			// For every position I define angle of the camera, angle of the ball
			// and v_x, v_y and v_z
			if(position == -2)
			{
				phi = pi - pi / 18;
				phi_ball = pi - pi / 13;	
				
				v_x = -0.5;
				v_y = 0.4;
				v_z = 0;		
			}
			else if(position == -1)
			{
				phi = 3 * pi / 4;
				phi_ball = 3 * pi / 4 - pi / 60;
				
				// On this position the position of the ball also depends on the v_x
				v_x = -0.4;
				v_y = 0.4;
				v_z = -0.3;
			}
			else if(position == 0)
			{
				phi = pi / 2;
				phi_ball = pi / 2;
				
				
				v_x = 0;
				v_y = 0.4;
				v_z = -0.5;
			}
			else if(position == 1)
			{
				phi = pi / 4;
				phi_ball = pi / 4 + pi / 60;

				// On this position the position of the ball also depends on the v_x
				v_x = 0.4;
				v_y = 0.4;
				v_z = -0.3;				
			}
			
			/*************** Version with camera on all positions
			phi += delta_phi;
			
			// Boundary for the camera
			if(phi > pi - pi / 18)
				phi = pi - pi / 18;
			
			phi_ball += delta_phi_ball;
			
			// Boundary for the ball
			if(phi_ball > pi - pi / 13)
				phi_ball = pi - pi / 13;
			*/
			
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			glutPostRedisplay();
			break;

		// Moving to the right and decreasing the angle
		case GLUT_KEY_RIGHT:
		
			shoot = 0;
			position += 1;
			
			if(position > 2)
				position = 2;
			
			// For every position I define angle of the camera, angle of the ball
			// and v_x, v_y and v_z
			if(position == 2)
			{
				phi = pi / 18;
				phi_ball = pi / 13;		
				
				v_x = -0.5;
				v_y = 0.4;
				v_z = 0;	
			}
			else if(position == 1)
			{
				phi = pi / 4;
				phi_ball = pi / 4 + pi / 60;
				
				// On this position the position of the ball also depends on the v_x
				v_x = 0.4;
				v_y = 0.4;
				v_z = -0.3;
			}
			else if(position == 0)
			{
				phi = pi / 2;
				phi_ball = pi / 2;
				
				v_x = 0;
				v_y = 0.4;
				v_z = -0.5;
			}
			else if(position == -1)
			{
				phi = 3 * pi / 4;
				phi_ball = 3 * pi / 4 - pi / 60;
				
				// On this position the position of the ball also depends on the v_x
				v_x = -0.4;
				v_y = 0.4;
				v_z = -0.3;
			}
						
			/*************** Version with camera on all positions
			phi -= delta_phi;
			
			
			// Boundary for the camera
			if(phi < pi / 18)
				phi = pi / 18;
			
			phi_ball -= delta_phi_ball;
		
			// Boundary for the ball
			if(phi_ball < pi / 13)
				phi_ball = pi / 13;
			*/
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
				
			glutPostRedisplay();
			break;
	}
}

static void on_timer(int value)
{
	if(value != TIMER_ID)
		return;

	// Different v_x, v_y and v_z for every position
	if(position == 0)
	{
		y_curr += v_y;
		z_curr += v_z;	
		
		if(y_curr >= 5)
		{
			v_y = -0.1;
			v_z = -0.5;
		}
		
		if(y_curr <= 0)
		{
			y_curr = 0;
			shoot = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			phi_ball = pi / 2;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			v_x = 0;
			v_y = 0.4;
			v_z = -0.5;
			
			glutPostRedisplay();
		}
		
		if(y_curr <= 4.5 && y_curr >= 3 && z_curr >= 2 && z_curr <= 3.6)
		{
			v_y = -0.2;
			v_z = -0.1;
			
			if(z_curr >= 2 && z_curr <= 2.6)
				v_z = 0;
		}
	}
	else if(position == -1)
	{
		x_curr -= v_x;
		y_curr += v_y;
		z_curr += v_z;	
		
		if(y_curr >= 5)
		{
			v_y = -0.1;
			v_z = -0.3;
		}
		
		if(y_curr <= 0)
		{
			y_curr = 0;
			shoot = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			phi_ball = 3 * pi / 4 - pi / 60;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			v_x = -0.4;
			v_y = 0.4;
			v_z = -0.3;
			
			glutPostRedisplay();
		}
		
		if(y_curr <= 5 && y_curr >= 2 && z_curr >= 2 && z_curr <= 3 && x_curr >= -1 && x_curr <= 1)
		{
			v_x = -0.2;
			v_y = -0.2;
			v_z = -0.1;
			
			if(x_curr <= 0.5 && x_curr >= 0)
				v_x = 0;
			
			if(z_curr >= 2 && z_curr <= 2.6)
				v_z = 0;
		}
	}
	else if(position == -2)
	{
		y_curr += v_y;
		x_curr -= v_x;	
		
		if(y_curr >= 5)
		{
			v_y = -0.1;
			v_x = -0.5;
		}
		
		if(y_curr <= 0)
		{
			y_curr = 0;
			shoot = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			phi_ball = pi - pi / 13;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			v_x = -0.5;
			v_y = 0.4;
			v_z = 0;
			
			glutPostRedisplay();
		}
		
		if(y_curr <= 4 && y_curr >= 3 && x_curr >= -1 && x_curr <= 1)
		{
			v_y = -0.2;
			v_x = -0.1;
			
			if(x_curr >= -0.5 && x_curr <= 0.6)
				v_x = 0;
		}
	}
	else if(position == 1)
	{
		x_curr -= v_x;
		y_curr += v_y;
		z_curr += v_z;	
		
		if(y_curr >= 5)
		{
			v_y = -0.1;
			v_z = -0.3;
		}
		
		if(y_curr <= 0)
		{
			y_curr = 0;
			shoot = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			phi_ball = pi / 4 + pi / 60;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			v_x = 0.4;
			v_y = 0.4;
			v_z = -0.3;
			
			glutPostRedisplay();
		}
		
		if(y_curr <= 5 && y_curr >= 2 && z_curr >= 2 && z_curr <= 3 && x_curr >= -1 && x_curr <= 1)
		{
			v_x = 0.2;
			v_y = -0.2;
			v_z = -0.1;
			
			if(x_curr <= 0 && x_curr >= -0.5)
				v_x = 0;
			
			if(z_curr >= 2 && z_curr <= 2.6)
				v_z = 0;
		}
	}
	else if(position == 2)
	{
		y_curr += v_y;
		x_curr += v_x;	
		
		if(y_curr >= 5)
		{
			v_y = -0.1;
			v_x = -0.5;
		}
		
		if(y_curr <= 0)
		{
			y_curr = 0;
			shoot = 0;
			
			
			// Delay of returning ball to its position
			sleep(1);
			phi_ball = pi / 13;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			v_x = -0.5;
			v_y = 0.4;
			v_z = 0;
			
			glutPostRedisplay();
		}
		
		if(y_curr <= 4 && y_curr >= 3 && x_curr >= -1 && x_curr <= 1)
		{
			v_y = -0.2;
			v_x = -0.1;
			
			if(x_curr >= -0.5 && x_curr <= 0.6)
				v_x = 0;
		}
	}
	
	glutPostRedisplay();
	
	if(shoot)
		glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
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
			//-10, 10, 15,
			//0, 2, 15,
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
	
		glTranslatef(x_curr, y_curr, z_curr);
		draw_ball();

	glPopMatrix();


	glutSwapBuffers();
}
