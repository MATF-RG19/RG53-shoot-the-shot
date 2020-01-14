#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#include "draw.h"
#include "details.h"
#include "image.h"

#define TIMER_ID 0
#define STRENGTH_ID 1
#define WEAK_ID 2
#define STRONG_ID 3

#define TIMER_INTERVAL 40
#define TIMER_STRENGTH 20

#define FILENAME0 "floor.bmp"
#define FILENAME1 "brick.bmp"
#define FILENAME2 "front.bmp"
#define FILENAME3 "sky.bmp"
#define FILENAME4 "start.bmp"
#define FILENAME5 "end.bmp"

static GLuint names[6];

// Current coordinates of the ball
static float x_curr, y_curr, z_curr; 

// Speed of the ball
static float v_x, v_y, v_z;

static int window_height, window_width;

static void on_reshape(int width, int height);
static void on_display(void);
static void on_keyboard(unsigned char key, int x, int y);
static void on_special_key_press(int key, int x, int y);
static void released_key(unsigned char key, int x, int y);
static void on_timer(int value);
static void strength(int value);
static void strong(int value);
static void weak(int value);
static void startScreen();
static void endScreen();

static void initialize(void);

const static float pi = 3.141592653589793;

// Angle for the camera
static float phi;

// Angle for the ball
static float phi_ball;

// Indicator for shoot
static int shoot = 0;

// Position of the ball
static int position = 0;

// To check if the button is pressed
static int pressed = 0;

// To check if v_x, v_y, v_z is already set
static int set_v = 0;

// Shot strength parameter and speed
static float shot_strength_parameter = 0;
static float v_shot_strength_parameter = 0.1;

// Current score
static int score = 0;

// Still on the opening screen
static int begin = 1;

// End of the game
static int end = 0;

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	
	glutInitWindowSize(300, 300);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Shoot the shot");
	
	glutFullScreen();
	
	glutReshapeFunc(on_reshape);
	glutKeyboardFunc(on_keyboard);
	glutDisplayFunc(on_display);
	glutSpecialFunc(on_special_key_press);
	glutKeyboardUpFunc(released_key);
	
	// Initialize angle of the camera
	phi = pi / 2;
	
	// Initialize angle of the ball
	phi_ball = pi / 2;

	// Initial position of the ball
	x_curr = 11 * cos(phi_ball);
	y_curr = 1;
	z_curr = 11 * sin(phi_ball);
	
	// Initial speed of the ball
	v_x = 0;
	v_y = 0.4;
	v_z = -0.5;

	glClearColor(0, 0, 0, 0);
	glEnable(GL_DEPTH_TEST); 
	
	// Important for antialiasing
	glEnable(GL_MULTISAMPLE_ARB);
	
	// This enables to register just one press on the button and to register when the button is released
	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
	
	initialize();
	
	glutMainLoop();

	return 0;
}

static void initialize(void)
{
	Image *image;
	
	glEnable(GL_TEXTURE_2D);
	
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	image = image_init(0, 0);
	
	// Create floor texture
	image_read(image, FILENAME0);
	
	// Generate identifier
	glGenTextures(6, names);
	
	glBindTexture(GL_TEXTURE_2D, names[0]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
	// Turn off active texture
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	// Create wall textures
	image_read(image, FILENAME1);
	
	glBindTexture(GL_TEXTURE_2D, names[1]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
	// Turn off active texture
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create front wall texture
	image_read(image, FILENAME2);
	
	glBindTexture(GL_TEXTURE_2D, names[2]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
	// Turn off active texture
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Create sky texture
	image_read(image, FILENAME3);
	
	glBindTexture(GL_TEXTURE_2D, names[3]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
	// Turn off active texture
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Create opening screen texture
	image_read(image, FILENAME4);
	
	glBindTexture(GL_TEXTURE_2D, names[4]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
	// Turn off active texture
	glBindTexture(GL_TEXTURE_2D, 0);
	
	// Create ending screen texture
	image_read(image, FILENAME5);
	
	glBindTexture(GL_TEXTURE_2D, names[5]);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, image->pixels);
	
	// Turn off active texture
	glBindTexture(GL_TEXTURE_2D, 0);

	image_done(image);
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

static void startScreen()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, names[4]);
	
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-14, -5, 0);
			
			glTexCoord2f(1, 0);
			glVertex3f(13.5, -5, 0);
			
			glTexCoord2f(1, 1);
			glVertex3f(13.5, 10, 0);
			
			glTexCoord2f(0, 1);
			glVertex3f(-14, 10, 0);
		glEnd();
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

static void endScreen()
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, names[5]);
	
		glBegin(GL_POLYGON);
			glTexCoord2f(0, 0);
			glVertex3f(-14, -5, 0);
			
			glTexCoord2f(1, 0);
			glVertex3f(13.5, -5, 0);
			
			glTexCoord2f(1, 1);
			glVertex3f(13.5, 10, 0);
			
			glTexCoord2f(0, 1);
			glVertex3f(-14, 10, 0);
		glEnd();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	
	glPushMatrix(); 
		glColor3f(1, 1, 1);
    	glRasterPos3f(-3.2, 1.3, 3);
    	
    	char str[50];
  		sprintf(str, "%d", score);
  		int length = strlen(str);
  		
    	for(int i = 0; i < length; i++)
    	{
        	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
    	}
    
    glPopMatrix();
}

static void on_keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		// Exit
		case 27:
			exit(0);
			break;
		
		// Shoot
		case 's':
		case 'S':

			pressed = 1;
			if(!shoot && !begin)
			{
				glutTimerFunc(TIMER_STRENGTH, strength, STRENGTH_ID);
			}
			
			glutPostRedisplay();
			break;
			
		// Start game on ENTER
		case 13:
			if(begin)
			{
				begin = 0;
				glutPostRedisplay();
			}
			
			break;
		
		// If you lose you can restart the game on SPACE
		case 32:
		
			if(end)
			{
				score = 0;
				shoot = 0;
				position = 0;
				pressed = 0;
				set_v = 0;
				shot_strength_parameter = 0;
				v_shot_strength_parameter = 0.1;
				phi = pi / 2;
				phi_ball = pi / 2;
				x_curr = 11 * cos(phi_ball);
				y_curr = 1;
				z_curr = 11 * sin(phi_ball);
				v_x = 0;
				v_y = 0.4;
				v_z = -0.5;
				end = 0;

				glutPostRedisplay();
			}
			
			break;
	}
}

static void released_key(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 's':
		case 'S':
		
			pressed = 0;
			
			if(!shoot && !pressed && !begin)
			{
			
				// If it is a good shot then call on_timer func
				if(shot_strength_parameter > 0.4 && shot_strength_parameter < 0.7)
				{
					score++;
					glutTimerFunc(TIMER_INTERVAL, on_timer, TIMER_ID);
				}	
				// Too weak shot
				else if(shot_strength_parameter <= 0.4)
					glutTimerFunc(TIMER_INTERVAL, weak, WEAK_ID);
					
				// Too strong shot
				else
					glutTimerFunc(TIMER_INTERVAL, strong, STRONG_ID);
					
				shoot = 1;
			}
			
			break;
	}
}

// Timer function for strong shot
static void strong(int value)
{
	if(value != STRONG_ID)
		return;
	
	// Counting how many times I am in the function
	set_v++;
	
	// Different v_x, v_y and v_z for every position
	if(position == 0)
	{
		// Setting speed of the ball first time i am in this function
		if(set_v == 1)
		{
			v_z = -0.8;
			v_y = 0.4;
		}
		
		y_curr += v_y;
		z_curr += v_z;	

		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			set_v = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
			
			// Returning to the original coordinates for this position
			phi_ball = pi / 2;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = 0;
			v_y = 0.4;
			v_z = -0.5;
			
			// End of the game
			end = 1;
			
			glutPostRedisplay();
		}
		
		// In this case the ball hits the board and it needs to repulse
		if(y_curr <= 6 && y_curr >= 3 && z_curr >= 2 && z_curr <= 2.4)
		{
			v_y = -0.2;
			v_z = 0.3;
		}
	}
	
	else if(position == -1)
	{
		// Setting speed of the ball first time i am in this function
		if(set_v == 1)
		{
			v_x = -0.6;
			v_y = 0.4;
			v_z = -0.5;
		}
		
		x_curr -= v_x;
		y_curr += v_y;
		z_curr += v_z;	
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			set_v = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = 3 * pi / 4 - pi / 60;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = -0.4;
			v_y = 0.4;
			v_z = -0.3;
			
			// End of the game
			end = 1;
			
			glutPostRedisplay();
		}
		
		// In this case the ball hits the board and it needs to repulse
		if(y_curr <= 6 && y_curr >= 2 && z_curr >= 2 && z_curr <= 2.4 && x_curr >= -1 && x_curr <= 1)
		{
			v_x = -0.3;
			v_y = -0.25;
			v_z = 0.1;
		}
	}
	
	else if(position == -2)
	{
		// Setting speed of the ball first time i am in this function
		if(set_v == 1)
		{
			v_x = -0.5;
			v_y = 0.4;
		}
		
		y_curr += v_y;
		x_curr -= v_x;	
		
		// When ball reaches this height then ball starts to decline
		if(y_curr >= 7)
		{
			v_y = -0.2;
			v_x = -0.4;
		}
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
			set_v = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = pi - pi / 13;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = -0.5;
			v_y = 0.4;
			v_z = 0;
			
			// End of the game
			end = 1;
			
			glutPostRedisplay();
		}
	}
	
	else if(position == 1)
	{
		// Setting speed of the ball first time i am in this function
		if(set_v == 1)
		{
			v_x = 0.6;
			v_y = 0.4;
			v_z = -0.5;
		}
		
		x_curr -= v_x;
		y_curr += v_y;
		z_curr += v_z;	
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			set_v = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = pi / 4 + pi / 60;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_zs
			v_x = 0.4;
			v_y = 0.4;
			v_z = -0.3;
			
			// End of the game
			end = 1;
			
			glutPostRedisplay();
		}
		
		// In this case the ball hits the board and it needs to repulse
		if(y_curr <= 6 && y_curr >= 2 && z_curr >= 2 && z_curr <= 2.4 && x_curr >= -1 && x_curr <= 1)
		{
			v_x = 0.3;
			v_y = -0.25;
			v_z = 0.1;
		}
	}
	
	else if(position == 2)
	{
		// Setting speed of the ball first time i am in this function
		if(set_v == 1)
		{
			v_x = -0.5;
			v_y = 0.4;
		}
		
		y_curr += v_y;
		x_curr += v_x;	
		
		// When ball reaches this height then ball starts to decline
		if(y_curr >= 7)
		{
			v_y = -0.2;
			v_x = -0.4;
		}
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			set_v = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = pi / 13;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = -0.5;
			v_y = 0.4;
			v_z = 0;
			
			// End of the game
			end = 1;
			
			glutPostRedisplay();
		}
	}
	
	glutPostRedisplay();
	
	if(shoot)
		glutTimerFunc(TIMER_INTERVAL, strong, STRONG_ID);
}

// Timer function for too weak shot
static void weak(int value)
{
	if(value != WEAK_ID)
		return;
	
	// Counting how many times I am in the function
	set_v++;
	
	// Different v_x, v_y and v_z for every position
	if(position == 0)
	{
		// Setting speed of the ball first time i am in this function
		if(set_v == 1)
		{
			v_z = -0.2;
			v_y = 0.3;
		}
		
		y_curr += v_y;
		z_curr += v_z;	
		
		// When ball reaches this height then ball starts to decline
		if(y_curr > 4)
		{
			v_y = -0.2;
			v_z = -0.2;
		}
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			set_v = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = pi / 2;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = 0;
			v_y = 0.4;
			v_z = -0.5;
			
			// End of the game
			end = 1;
			
			glutPostRedisplay();
		}
	}
	
	else if(position == -1)
	{
		// Setting speed of the ball first time i am in this function
		if(set_v == 1)
		{
			v_x = -0.2;
			v_y = 0.2;
			v_z = -0.15;
		}
		
		x_curr -= v_x;
		y_curr += v_y;
		z_curr += v_z;	
		
		// When ball reaches this height then ball starts to decline
		if(y_curr >= 3)
		{
			v_y = -0.1;
			v_z = -0.15;
		}
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			set_v = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = 3 * pi / 4 - pi / 60;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = -0.4;
			v_y = 0.4;
			v_z = -0.3;
			
			// End of the game
			end = 1;
			
			glutPostRedisplay();
		}
	}
	
	else if(position == -2)
	{
		// Setting speed of the ball first time i am in this function
		if(set_v == 1)
		{
			v_x = -0.3;
			v_y = 0.2;
		}
		
		y_curr += v_y;
		x_curr -= v_x;	
		
		// When ball reaches this height then ball starts to decline
		if(y_curr >= 3)
		{
			v_y = -0.1;
			v_x = -0.2;
		}
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			set_v = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = pi - pi / 13;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = -0.5;
			v_y = 0.4;
			v_z = 0;
			
			// End of the game
			end = 1;
			
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
		// Setting speed of the ball first time i am in this function
		if(set_v == 1)
		{
			v_x = 0.2;
			v_y = 0.2;
			v_z = -0.15;
		}
		
		x_curr -= v_x;
		y_curr += v_y;
		z_curr += v_z;	
		
		// When ball reaches this height then ball starts to decline
		if(y_curr >= 3)
		{
			v_y = -0.1;
			v_z = -0.15;
		}
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			set_v = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = pi / 4 + pi / 60;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = 0.4;
			v_y = 0.4;
			v_z = -0.3;
			
			// End of the game
			end = 1;
			
			glutPostRedisplay();
		}
	}
	
	else if(position == 2)
	{
		// Setting speed of the ball first time i am in this function
		if(set_v == 1)
		{
			v_x = -0.3;
			v_y = 0.2;
		}
		
		y_curr += v_y;
		x_curr += v_x;	
		
		// When ball reaches this height then ball starts to decline
		if(y_curr >= 3)
		{
			v_y = -0.1;
			v_x = -0.2;
		}
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			set_v = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = pi / 13;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = -0.5;
			v_y = 0.4;
			v_z = 0;
			
			// End of the game
			end = 1;
			
			glutPostRedisplay();
		}
	}
	
	glutPostRedisplay();
	
	if(shoot)
		glutTimerFunc(TIMER_INTERVAL, weak, WEAK_ID);
}

// Timer function for the strength of the shot
static void strength(int value)
{
	if(value != STRENGTH_ID)
		return;

	shot_strength_parameter += v_shot_strength_parameter;

	glutPostRedisplay();
	
	if(pressed)
		glutTimerFunc(TIMER_STRENGTH, strength, STRENGTH_ID);
}

static void on_special_key_press(int key, int x, int y)
{
	if(!begin)
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
				
				// Coordinates of the ball
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
							
				// Coordinates of the ball
				x_curr = 11 * cos(phi_ball);
				y_curr = 1;
				z_curr = 11 * sin(phi_ball);
					
				glutPostRedisplay();
				break;
		}
	}
}

// Timer function for the good shot
static void on_timer(int value)
{
	if(value != TIMER_ID)
		return;

	// Different v_x, v_y and v_z for every position
	if(position == 0)
	{
		y_curr += v_y;
		z_curr += v_z;	
		
		// When ball reaches this height then ball starts to decline
		if(y_curr >= 5)
		{
			v_y = -0.1;
			v_z = -0.5;
		}
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = pi / 2;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = 0;
			v_y = 0.4;
			v_z = -0.5;
			
			glutPostRedisplay();
		}
		
		// Ball moves down the net
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
		
		// When ball reaches this height then ball starts to decline
		if(y_curr >= 5)
		{
			v_y = -0.1;
			v_z = -0.3;
		}
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = 3 * pi / 4 - pi / 60;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = -0.4;
			v_y = 0.4;
			v_z = -0.3;
			
			glutPostRedisplay();
		}
		
		// Ball moves down the net
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
		
		// When ball reaches this height then ball starts to decline
		if(y_curr >= 5)
		{
			v_y = -0.1;
			v_x = -0.5;
		}
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = pi - pi / 13;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = -0.5;
			v_y = 0.4;
			v_z = 0;
			
			glutPostRedisplay();
		}
		
		// Ball moves down the net
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
		
		// When ball reaches this height then ball starts to decline
		if(y_curr >= 5)
		{
			v_y = -0.1;
			v_z = -0.3;
		}
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = pi / 4 + pi / 60;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = 0.4;
			v_y = 0.4;
			v_z = -0.3;
			
			glutPostRedisplay();
		}
		
		// Ball moves down the net
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
		
		// When ball reaches this height then ball starts to decline
		if(y_curr >= 5)
		{
			v_y = -0.1;
			v_x = -0.5;
		}
		
		if(y_curr <= 0.5)
		{
			// Animation is ending
			y_curr = 0;
			shoot = 0;
			
			
			// Delay of returning ball to its position
			sleep(1);
			shot_strength_parameter = 0;
						
			// Returning to the original coordinates for this position
			phi_ball = pi / 13;
			x_curr = 11 * cos(phi_ball);
			y_curr = 1;
			z_curr = 11 * sin(phi_ball);
			
			// Returning to the initial v_x, v_y and v_z
			v_x = -0.5;
			v_y = 0.4;
			v_z = 0;
			
			glutPostRedisplay();
		}
		
		// Ball moves down the net
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

	// At the begging you are standing in front of the basket
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	if(begin)
	{
		gluLookAt(0, 3, 12, 0, 3, 0, 0, 1, 0);
		startScreen();
	}
	
	else if(end)
	{
		gluLookAt(0, 3, 12, 0, 3, 0, 0, 1, 0);
		endScreen();
	}
	
	else
	{
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

	   	glBindTexture(GL_TEXTURE_2D, names[0]);
			draw_floor();
		glBindTexture(GL_TEXTURE_2D, 0);
		 
		glBindTexture(GL_TEXTURE_2D, names[1]);
			draw_left_wall();
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glBindTexture(GL_TEXTURE_2D, names[1]);
			draw_right_wall();
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glBindTexture(GL_TEXTURE_2D, names[2]);
			draw_front_wall();
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glBindTexture(GL_TEXTURE_2D, names[1]);
			draw_back_wall();  
		glBindTexture(GL_TEXTURE_2D, 0);
		
		glBindTexture(GL_TEXTURE_2D, names[3]);
			draw_sky();
		glBindTexture(GL_TEXTURE_2D, 0);
		
		draw_basket();

		// Ball and movement of the ball
		
		glPushMatrix();
		
			glTranslatef(x_curr, y_curr, z_curr);
			draw_ball();

		glPopMatrix();
		
		// Position of the shoot bar
		glPushMatrix();

			if(position == 0)
			{
				glTranslatef(-4, 1.5, 10);
				glRotatef(6, 1, 0, 0);
			}
			
			else if(position == -1)
			{
				glTranslatef(-9, 1.45, 4.5);
				glRotatef(-50, 0, 1, 0);
				glRotatef(6, 1, 0, 0);		
			}
			
			else if(position == -2)
			{
				glTranslatef(-12, 1.6, 0.6);
				glRotatef(-90, 0, 1, 0);
				glRotatef(6, 1, 0, 0);
				glScalef(0.6, 0.6, 1);
			}
			
			else if(position == 1)
			{
				glTranslatef(4, 1.5, 10.9);
				glRotatef(55, 0, 1, 0);
				glRotatef(6, 1, 0, 0);
			}
			
			else if(position == 2)
			{
				glTranslatef(9.8, 1.45, 6.7);
				glRotatef(90, 0, 1, 0);
				glRotatef(6, 1, 0, 0);
			}
			
			// Frame for the strength
			frame();
			
			// Clip plane which moves when the shooting button is being held down
			double clip_plane[] = {0, -1, 0, shot_strength_parameter};
			glEnable(GL_CLIP_PLANE0);
			glClipPlane(GL_CLIP_PLANE0, clip_plane);
			
			shot_strength();
			
			glDisable(GL_CLIP_PLANE0);

		glPopMatrix();
	}
		
	glutSwapBuffers();
}
