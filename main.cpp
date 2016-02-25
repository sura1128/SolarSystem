// CS3241Lab2.cpp : Defines the entry point for the console application.
#include <cmath>
#include <iostream>
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#include <cmath>

/* Include header files depending on platform */
#ifdef _WIN32
	#include "glut.h"
	#define M_PI 3.14159
#elif __APPLE__
	#include <OpenGL/gl.h>
	#include <GLUT/GLUT.h>
#endif

using namespace std;

#define numStars 40
#define numPlanets 4

float A[numPlanets] = { 0, 0, 0, 0 }; //planet angles
float M = 0; //moon angles

class planet
{
public:
	float distFromRef;
	float angularSpeed;
	GLfloat color[3];
	float size;
	float angle;
	float planetNo;
	float moonOpacity;
	float noOfMoons;
	float moonSpeed;
	float planetOpcacity;

	planet()
	{
		planetNo = 0;
		distFromRef = 0;
		angularSpeed = 0;
		color[0] = color[1] = color[2] = 0;
		size = 0;
		angle = 0;
		moonOpacity = 1;
		noOfMoons = 0;
		planetOpcacity = 1;
		moonSpeed = 0;
	}

	void drawMoon(bool isClock)
	{
		GLfloat color[3] = { 1, 1, 1 };
		if (isClock == false) {
			M = M + moonSpeed / 100;
			if (M > 360) { M = 0; }
			glRotatef(M, 0, 0, 1);
		}
		drawCircle(0.3, 25, 360, color, moonOpacity);
	}

	void drawCircle(float s, float t, int degrees, GLfloat color[], float transperancy) {

		glScalef(s, s, s);
		glTranslatef(0, t, t);
		glColor4f(color[0], color[1], color[2], transperancy);

		const float PI = 3.14159 / 180;

		glBegin(GL_POLYGON);
		for (int i = 0; i < degrees; i++) {
			float rad = i*PI;
			glVertex2f(cos(rad) * 5, sin(rad) * 5);
		}
		glEnd();

	}

	void drawPlanet(bool isClock) {
		glPushMatrix();
		drawCircle(size, distFromRef, 360, color, 1);
		if (noOfMoons > 0) {
			drawMoon(isClock);
		}
		if (planetNo == 4) {
			GLfloat color[3] = { 1, 1, 1 };
			drawRing(1.5, 0, color, 5, 6);
		}
		glPopMatrix();
	}

	void drawRing(float s, float t, GLfloat color[], float radX, float radY) {
		glScalef(s, s, s);
		glTranslatef(0, t, t);
		glColor3f(color[0], color[1], color[2]);

		const float PI = 3.14159 / 180;
		glBegin(GL_LINE_LOOP);
		for (int i = 0; i < 360; i++) {
			float rad = i*PI;
			glVertex2f(cos(rad)*radX, sin(rad)*radY);
		}
		glEnd();
	}

	void drawEllipticalOrbit(double angle) {
		glPushMatrix();
		const float PI = 3.14159 / 180;

		double xForEarth = cos(angle*PI)*20.0f;//to change the x co-ordinate
		double yForEarth = sin(angle*PI)*12.0f;//to change the y co-ordinate
		glPushMatrix();
		glTranslated(xForEarth, yForEarth, 0.0f);
		drawPlanet(false);
		glPopMatrix();
		glPopMatrix();
	}
};

class sun
{
public:
	float distFromRef;
	float angularSpeed;
	GLfloat color[3];
	float size;
	float angle;
	float opacity;

	sun()
	{
		distFromRef = 0;
		angularSpeed = 0;
		size = 0.25;
		color[0] = 1;
		color[1] = 1;
		color[2] = 0;
		opacity = 1;
	}

	void drawCircle(float s, float t, GLfloat color[], float transperancy) {

		glScalef(s, s, s);
		glTranslatef(0, t, t);
		glColor4f(color[0], color[1], color[2], transperancy);

		const float PI = 3.14159 / 180;

		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float rad = i*PI;
			glVertex2f(cos(rad) * 5, sin(rad) * 5);
		}
		glEnd();

	}

	void drawSun() {
		GLfloat color[3] = { 1, 1, 0 };
		drawCircle(size, distFromRef,color, opacity);
	}
};

class star
{
public:
	float xPos;
	float yPos;
	float size;
	float alpha;
	float color[3];

	star() {
		xPos = 0;
		yPos = 0;
		size = 0.07;
		color[0] = color[1] = color[2] = 1;
		alpha = 1;
	}

	void drawStar() {
		glScalef(size, size, size);
		glTranslatef(xPos, yPos, 0);
		const float PI = 3.14159 / 180;
		glColor4f(1, 1, 1, alpha);

		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float rad = i*PI;
			glVertex2f(cos(rad) * 5, sin(rad) * 5);
		}
		glEnd();
	}
};

float alpha = 0.0, k=1;
float tx = 0.0, ty=0.0;
planet planetList[numPlanets];
star starList[numStars];
bool ISCLOCK = false;
float T = 0;
float BLINK = 0;


void reshape (int w, int h)
{
	glViewport (0, 0, (GLsizei) w, (GLsizei) h);

	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();

	glOrtho(-10, 10, -10, 10, -10, 10);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void init(void)
{
	glClearColor (0.0, 0.0, 0.3, 1.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void createStar(double xPos, double yPos, double millis) {
	glPushMatrix();
	const float PI = 3.14159 / 180;
	star s1 = star();
	s1.xPos = xPos;
	s1.yPos = yPos;
	s1.alpha = (rand() % 10 + millis / (3600 * 1000)) / 10;
	s1.drawStar();
	glPopMatrix();

}

void createConstellation() {
	glPushMatrix();

	SYSTEMTIME time;
	GetSystemTime(&time);
	double millis = (time.wSecond * 1000) + time.wMilliseconds;
	
	createStar(100, 100, millis);
	createStar(100, 250, millis);
	createStar(150, 200, millis);
	createStar(100, 150, millis);
	createStar(200, 150, millis);
	createStar(0, 250, millis);
	createStar(150, 150, millis);
	
	glPopMatrix();
	
}

void createCluster(float tX, float tY, float angle) {
	glPushMatrix();
	glTranslatef(tX, tY, 0);
	glRotatef(angle, 0, 0, 1);
	createConstellation();
	glPopMatrix();
}

void createStarSystem() {
	glPushMatrix();
	createConstellation();
	glPopMatrix();

	createCluster(-10, 0, 20);
	createCluster(-40, 0, -40);
	createCluster(-20, -30, 30);
	createCluster(20, -30, 10);
	createCluster(20, 0, -10);
	createCluster(0, 20, -50);
	createCluster(10, -30, 60);



}


void createPlanets() {
	planet mercury = planet();
	mercury.planetNo = 1;
	mercury.angle = 0;
	mercury.color[0] = 0.6;
	mercury.color[1] = 0.3;
	mercury.color[2] = 0.6;
	mercury.distFromRef = 45;
	mercury.size = 0.2;
	mercury.angularSpeed = 5;

	planet venus = planet();
	venus.planetNo = 2;
	venus.angle = 0;
	venus.color[0] = 1;
	venus.color[1] = 0.25;
	venus.color[2] = 0;
	venus.distFromRef = 70;
	venus.size = 0.25;
	venus.angularSpeed = 20;

	planet earth = planet();
	earth.planetNo = 3;
	earth.angle = 0;
	earth.color[0] = 0.43;
	earth.color[1] = 0.85;
	earth.color[2] = 0.57;
	earth.distFromRef = 90;
	earth.size = 0.3;
	earth.angularSpeed = 10;
	earth.noOfMoons = 1;
	earth.moonSpeed = 20;

	planet saturn = planet();
	saturn.planetNo = 4;
	saturn.angle = 0;
	saturn.color[0] = 0.92;
	saturn.color[1] = 0.78;
	saturn.color[2] = 0.62;
	saturn.distFromRef = 100;
	saturn.size = 0.35;
	saturn.angularSpeed = 15;

	planetList[0] = mercury;
	planetList[1] = venus;
	planetList[2] = earth;
	planetList[3] = saturn;

}

void drawSolarSystem() {
	glPushMatrix();
	
	SYSTEMTIME time;
	GetSystemTime(&time);

	for (int i = 0; i < numPlanets; i++) {
		glPushMatrix();
		A[i] = A[i] + planetList[i].angularSpeed/100;
		if (A[i] > 360) { A[i] = 0; }

		if (i == 1) {
			glTranslatef(0, -17, 0);
			planetList[i].drawEllipticalOrbit(A[i]);
		}
		else {
			glRotated(A[i], 0, 0, 1);
			planetList[i].drawPlanet(false);
		}
		
		glPopMatrix();
	}
	glPopMatrix();
}

void drawClock() {
	glPushMatrix();

	SYSTEMTIME time;
	GetSystemTime(&time);
	double millis = (time.wSecond * 1000) + time.wMilliseconds;

	float sec = millis / 1000;
	float min = time.wMinute;
	float hour = time.wHour;

	double angleSec = 360 - sec / 60 * 360;
	double angleMin = 360 - min / 60 * 360;
	double angleHour = 360 - hour / 60 * 360;

	double angle[3] = { angleHour, angleMin, angleSec };

	for (int i = 0; i < 3; i++) {
		glPushMatrix();
		glRotatef(angle[i], 0, 0, 1);
		planetList[i].drawPlanet(true);
		glPopMatrix();
	}

	planetList[3].drawPlanet(true);
	glPopMatrix();
}

void createSolarSystem() {
	glPushMatrix();
	sun Sun = sun();
	Sun.drawSun();
	createPlanets();
	createStarSystem();

	if (ISCLOCK == true) {
		drawClock();
	}
	else {
		drawSolarSystem();
	}

	glPopMatrix();
	
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glPushMatrix();

	//controls transformation
	glScalef(k, k, k);	
	glTranslatef(tx, ty, 0);	
	glRotatef(alpha, 0, 0, 1);

	//draw stuff here!
	createSolarSystem();
	glPopMatrix();
	glFlush ();
}

void idle()
{
	//update animation here
	glutPostRedisplay();	//after updating, draw the screen again
}

void keyboard (unsigned char key, int x, int y)
{
	//keys to control scaling - k
	//keys to control rotation - alpha
	//keys to control translation - tx, ty
	switch (key) {

		case 'a':
			alpha+=10;
			glutPostRedisplay();
		break;

		case 'd':
			alpha-=10;
			glutPostRedisplay();
		break;

		case 'q':
			k+=0.1;
			glutPostRedisplay();
		break;

		case 'e':
			if(k>0.1)
				k-=0.1;
			glutPostRedisplay();
		break;

		case 'z':
			tx-=0.1;
			glutPostRedisplay();
		break;

		case 'c':
			tx+=0.1;
			glutPostRedisplay();
		break;

		case 's':
			ty-=0.1;
			glutPostRedisplay();
		break;

		case 'w':
			ty+=0.1;
			glutPostRedisplay();
		break;

		case 't':
			ISCLOCK = !ISCLOCK;
		break;
		
		case 27:
			exit(0);
		break;

		default:
		break;
	}
}

int main(int argc, char **argv)
{
	cout<<"CS3241 Lab 2\n\n";
	cout<<"+++++CONTROL BUTTONS+++++++\n\n";
	cout<<"Scale Up/Down: Q/E\n";
	cout<<"Rotate Clockwise/Counter-clockwise: A/D\n";
	cout<<"Move Up/Down: W/S\n";
	cout<<"Move Left/Right: Z/C\n";
	cout <<"ESC: Quit\n";

	glutInit(&argc, argv);
	glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize (600, 600);
	glutInitWindowPosition (50, 50);
	glutCreateWindow (argv[0]);
	init ();
	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutReshapeFunc(reshape);	
	//glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}
