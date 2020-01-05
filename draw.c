#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#include "draw.h"

void draw_floor()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	// Floor and texture for floor
	glBegin(GL_POLYGON);
		glNormal3f(1, 1, 1);
	
		glTexCoord2f(0, 0);
		glVertex3f(-30, 0, 30);
		
		glTexCoord2f(10, 0);
		glVertex3f(30, 0, 30);
		
		glTexCoord2f(10, 10);
		glVertex3f(30, 0, 0);
		
		glTexCoord2f(0, 10);
		glVertex3f(-30, 0, 0);
	glEnd();
}

void draw_sky()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Sky and texture for sky
	// +/- 31 and not 30 because of textures
	glBegin(GL_POLYGON);
		glNormal3f(1, 1, 1);
	
		glTexCoord2f(0, 0);
		glVertex3f(-31, 15, 30);
		
		glTexCoord2f(2, 0);
		glVertex3f(31, 15, 30);
		
		glTexCoord2f(2, 1);
		glVertex3f(31, 15, 0);
		
		glTexCoord2f(0, 1);
		glVertex3f(-31, 15, 0);
	glEnd();
}

void draw_front_wall()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Front wall and texture for wall
	glBegin(GL_POLYGON);
		
		glTexCoord2f(0, 0);
		glVertex3f(-30, 0, 0);
		
		glTexCoord2f(1, 0);
		glVertex3f(30, 0, 0);
		
		glTexCoord2f(1, 1);
		glVertex3f(30, 15, 0);
		
		glTexCoord2f(0, 1);
		glVertex3f(-30, 15, 0);
	glEnd();
}

void draw_left_wall()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Left wall and texture for wall
	glBegin(GL_POLYGON);
		
		glTexCoord2f(0, 0);
		glVertex3f(-30, 0, 30);
		
		glTexCoord2f(2, 0);
		glVertex3f(-30, 0, 0);
		
		glTexCoord2f(2, 3);
		glVertex3f(-30, 15, 0);
		
		glTexCoord2f(0, 3);
		glVertex3f(-30, 15, 30);
	glEnd();
}	

void draw_right_wall()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Right wall and texture for wall
	glBegin(GL_POLYGON);
		
		glTexCoord2f(0, 0);
		glVertex3f(30, 0, 0);
		
		glTexCoord2f(2, 0);
		glVertex3f(30, 0, 30);
		
		glTexCoord2f(2, 3);
		glVertex3f(30, 15, 30);
		
		glTexCoord2f(0, 3);
		glVertex3f(30, 15, 0);
	glEnd();
}

void draw_back_wall()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Back wall and texture for wall
	glBegin(GL_POLYGON);
		
		glTexCoord2f(0, 0);
		glVertex3f(30, 0, 30);
		
		glTexCoord2f(2, 0);
		glVertex3f(-30, 0, 30);
		
		glTexCoord2f(2, 3);
		glVertex3f(-30, 15, 30);
		
		glTexCoord2f(0, 3);
		glVertex3f(30, 15, 30);
	glEnd();
}

void draw_basket()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	// Enable 
	glEnable(GL_BLEND);
	
	/*
	glBlendFunc(src, dest);
	src - new colors, default 1
	dest - colors that already exist, default 0
	By default blend mode is ADD 
	R = Rsrc * rc + Rdest * dest
	G = Gsrc * src + Gdest * dest
	B = Bsrc * src + Bdest * dest
	A = Asrc * src + Adest * dest
	*/
	
	// Blend because of the transparency of the board
	
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// Line for shooting
	double x = -15;
		
	glBegin(GL_LINE_STRIP);
	
		glColor3f(0.5, 0.5, 0.5);
	
		while(x <= 15)
		{
			glVertex3f(x, 0, sqrt(225 - x * x));
			x += 0.01;
		}
		
		glVertex3f(15, 0, 0);
	glEnd();
	
	// Base of the basket
	glPushMatrix();
	
		glTranslatef(0, 0, 1);
		glRotatef(-90, 1, 0, 0);
		glColor3f(0.2, 0.2, 0.2);
	
		GLUquadric* base = gluNewQuadric();
		//gluQuadricDrawStyle(cyl, GLU_TRUE);
		//gluQuadricTexture(cyl, GLU_FILL);
		gluCylinder(base, 0.5, 0.5, 5, 10, 10);

	glPopMatrix();
	
	// Basketball board
	glPushMatrix();
		
		glTranslatef(0, 5, 1.4);	
		glScalef(2, 0.92, 0.1);
		glColor4f(1, 1, 1, 0.5);
		glutSolidCube(3);
		
	glPopMatrix();

	// Rectangle inside of the board, dimensions of the outer rectangle: 1.98 and 1.5007
	// Inner rectangle is distant by 0.15 from each side of the outer rectangle
	glPushMatrix();

		glTranslatef(0, 4, 1.6);
		glColor3f(0, 0, 0);
		glBegin(GL_POLYGON);
		
			glVertex3f(-0.99, 0, 0);
			glVertex3f(0.99, 0, 0);
			glVertex3f(0.84, 0.15, 0);
			glVertex3f(-0.84, 0.15, 0);
		
		glEnd();
		
		glBegin(GL_POLYGON);
		
			glVertex3f(0.99, 0, 0);
			glVertex3f(0.99, 1.5007, 0);
			glVertex3f(0.84, 1.3507, 0);
			glVertex3f(0.84, 0.15, 0);
		
		glEnd();
		
		glBegin(GL_POLYGON);
		
			glVertex3f(0.99, 1.5007, 0);
			glVertex3f(-0.99, 1.5007, 0);
			glVertex3f(-0.84, 1.3507, 0);
			glVertex3f(0.84, 1.3507, 0);
		
		glEnd();
		
		glBegin(GL_POLYGON);
		
			glVertex3f(-0.99, 1.5007, 0);
			glVertex3f(-0.99, 0, 0);
			glVertex3f(-0.84, 0.15, 0);
			glVertex3f(-0.84, 1.3507, 0);
		
		glEnd();
		
	glPopMatrix();
	
	// Hoop
	glPushMatrix();
		
		glTranslatef(0, 4, 2.6);
		glRotatef(-90, 1, 0, 0);
		glColor3f(1, 0, 0);
	
		GLUquadric* hoop = gluNewQuadric();
		//gluQuadricDrawStyle(cyl, GLU_TRUE);
		//gluQuadricTexture(cyl, GLU_FILL);
		gluCylinder(hoop, 1, 1, 0.2, 30, 30);
	
	glPopMatrix();
	
	// Net
	glPushMatrix();
	
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
		glTranslatef(0, 3, 2.6);
		glRotatef(-90, 1, 0, 0);
		glColor3f(1, 1, 1);
	
		GLUquadric* net = gluNewQuadric();
		//gluQuadricDrawStyle(cyl, GLU_TRUE);
		//gluQuadricTexture(cyl, GLU_FILL);
		gluCylinder(net, 0.6, 0.95, 1, 40, 10);
	
	glPopMatrix();
}

void draw_ball()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		
	glColor3f(0.6, 0, 0);
	glutSolidSphere(0.5, 50, 50);
}
