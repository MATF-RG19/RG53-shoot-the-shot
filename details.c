#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#include "details.h"

void frame()
{
	glDisable(GL_LIGHTING);
	
	//Outer line for shot strength
	glColor3f(0, 0, 0);
	
	glBegin(GL_POLYGON);
	
		glVertex3f(-0.55, -0.05, 0);
		glVertex3f(0.05, -0.05, 0);
		glVertex3f(0.05, 0, 0);
		glVertex3f(-0.55, 0, 0);
	glEnd();
	
	glBegin(GL_POLYGON);
	
		glVertex3f(0, 0, 0);
		glVertex3f(0.05, 0, 0);
		glVertex3f(0.05, 1.05, 0);
		glVertex3f(0, 1.05, 0);
	glEnd();
	
	glBegin(GL_POLYGON);
	
		glVertex3f(-0.55, 1, 0);
		glVertex3f(0, 1, 0);
		glVertex3f(0, 1.05, 0);
		glVertex3f(-0.55, 1.05, 0);
	glEnd();
	
	glBegin(GL_POLYGON);
	
		glVertex3f(-0.55, 0, 0);
		glVertex3f(-0.5, 0, 0);
		glVertex3f(-0.5, 1, 0);
		glVertex3f(-0.55, 1, 0);
	glEnd();
	
	glEnable(GL_LIGHTING);
}

// Shot strength with gradient of colors
void shot_strength()
{
	glDisable(GL_LIGHTING);
	
	glBegin(GL_POLYGON);
	
		glColor3f(1, 1, 1);
		glVertex3f(-0.5, 0, 0);
		glVertex3f(0, 0, 0);
		
		glColor3f(0, 1, 0);
		glVertex3f(0, 0.5, 0);
		glVertex3f(-0.5, 0.5, 0);
	glEnd();
	
	glBegin(GL_POLYGON);
	
		glColor3f(0, 1, 0);
		glVertex3f(-0.5, 0.5, 0);
		glVertex3f(0, 0.5, 0);
		
		glColor3f(1, 0, 0);
		glVertex3f(0, 1, 0);
		glVertex3f(-0.5, 1, 0);
	glEnd();
	
	glEnable(GL_LIGHTING);
}
