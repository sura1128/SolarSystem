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

#define numPlanets 4

float angles[numPlanets] = { 0, 0, 0, 0 }; //planet angles
float M = 0; //moon angles
bool ISCLOCK = false;
float TRACK = 9;
bool DEATHSTAR = false;
bool FLASH = false;


// classes: sun, deathstar (extra effect), planet, star
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
		color[1] = 0.8;
		color[2] = 0;
		opacity = 1;
	}

	void drawCircle(float s, float t, float transperancy) {

		glScalef(s, s, s);
		glTranslatef(0, t, 0);
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
		if (ISCLOCK == true) {
			opacity = 0.5;
		}
		drawCircle(size, distFromRef, opacity);
	}
};

//extra effect: the empire strikes back!
class deathstar
{
public:

	float size;
	float x;
	float y;
	float color[3];

	deathstar()
	{
		size = 1.2;
		x = 0;
		y = 0;
		color[0] = 0.30;
		color[1] = 0.30;
		color[2] = 0.30;
	}

	void drawDeathStar()
	{

		glPushMatrix();
		glScalef(size, size, size);
		glTranslatef(x, y, 0);
		glColor3f(color[0], color[1], color[2]);

		const float PI = 3.14159 / 180;

		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float rad = i*PI;
			glVertex2f(cos(rad) * 5, sin(rad) * 5);
		}
		glEnd();

		glPushMatrix();
		glColor3f(0.2, 0.2, 0.2);
		glTranslatef(0, 2, 0);
		glScalef(0.3, 0.3, 0.3);
		glBegin(GL_POLYGON);
		for (int i = 0; i < 360; i++) {
			float rad = i*PI;
			glVertex2f(cos(rad) * 5, sin(rad) * 5);
		}
		glEnd();

		if (FLASH == true) {
			for (int i = 1; i <= 3; i++) {
				glPushMatrix();
				glColor3f(0, 1, 0.49);
				glRotatef(i * (-5), 0, 0, 1);
				glBegin(GL_LINE_STRIP);
				glVertex2f(-100, 0);
				glVertex2f(0, 0);
				glEnd();
				glPopMatrix();
			}
		}	
		

		glPopMatrix();

		glColor3f(0, 0, 0);
		glBegin(GL_LINE_STRIP);
		glVertex2f(-5, 0);
		glVertex2f(5, 0);
		glEnd();

		glPopMatrix();


	}

};


class planet
{
public:

	float planetNo;
	float distFromRef;
	float angularSpeed;
	GLfloat color1[3];
	GLfloat color2[3];	
	float size;
	float angle;
	float moonOpacity;
	float noOfMoons;
	float moonSpeed;
	float planetOpcacity;
	float noOfRings;
	bool isMoon;

	planet()
	{
		planetNo = 0;
		distFromRef = 0;
		angularSpeed = 0;
		size = 0;
		angle = 0;
		color1[0] = color1[1] = color1[2] = 0;
		color2[0] = color2[1] = color2[2] = 0;
		planetOpcacity = 1;
		
		noOfRings = 0;

		// Moon related attributes
		moonSpeed = 0;
		moonOpacity = 1;
		noOfMoons = 0;
		isMoon = false;
	}

	void drawCircle(float s, float t, int degrees, float transperancy) {

		glScalef(s, s, s);
		glTranslatef(0, t, t);
		const float PI = 3.14159 / 180;

		glColor4f(color1[0], color1[1], color1[2], transperancy); //first half gradient
		if (isMoon == true) {
			glColor4f(1, 1, 1, transperancy);
		}

		glBegin(GL_POLYGON);
		for (int i = 0; i < degrees; i++) {
			float rad = i*PI;
			glVertex2f(cos(rad) * 5, sin(rad) * 5);
		}
		glEnd();

		glColor4f(color2[0], color2[1], color2[2], transperancy); //second half gradient
		if (isMoon == true) {
			glColor4f(1, 1, 1, transperancy);
		}

		glBegin(GL_POLYGON);
		for (int i = 180; i < 360; i++) {
			float rad = i*PI;
			glVertex2f(cos(rad) * 5, sin(rad) * 5);
		}
		glEnd();

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

	// Only for Venus!
	void drawEllipticalOrbit(double angle) {
		glPushMatrix();
		const float PI = 3.14159 / 180;

		double xForEarth = cos(angle*PI)*20.0f;//to change the x co-ordinate
		double yForEarth = sin(angle*PI)*12.0f;//to change the y co-ordinate
		glPushMatrix();
		glTranslated(xForEarth, yForEarth, 0.0f);
		drawPlanet();
		glPopMatrix();		
		glPopMatrix();
	}


	void drawMoon()
	{
		GLfloat color[3] = { 1, 1, 1 };
		if (ISCLOCK == false) {
			M = M + moonSpeed / 100;
			if (M > 360) { M = 0; }
			glRotatef(M, 0, 0, 1);
		}
		isMoon = true;
		drawCircle(0.3, 25, 360, moonOpacity);
		isMoon = false;
	}


	void drawPlanet() {
		glPushMatrix();
		drawCircle(size, distFromRef, 360, planetOpcacity);
		
		if (planetNo == 2) {
			if (DEATHSTAR == true) { //click 'x' to activate the Death Star!
				if (TRACK == 15) {
					TRACK = 9;
				}
				else {
					TRACK = 15;
				}
				glTranslatef(0, TRACK, 0);
				deathstar DeathStar = deathstar();
				DeathStar.drawDeathStar();
			}
			
		}	

		if (noOfMoons > 0) {
			drawMoon();
		}
		if (noOfRings > 0) {
			GLfloat color[3] = { 1, 1, 1 };
			drawRing(1.5, 0, color, 5, 6);
		}
		glPopMatrix();
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
float T = 0;


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

// One star
void createStar(double xPos, double yPos, double millis) {
	glPushMatrix();
	const float PI = 3.14159 / 180;
	star s1 = star();
	s1.xPos = xPos;
	s1.yPos = yPos;
	if (ISCLOCK == true) {
		s1.alpha = 0.5;
	}
	else {
		s1.alpha = (rand() % 10 + millis / (3600 * 1000)) / 10; //randomly change opacity of star
	}	
	s1.drawStar();
	glPopMatrix();

}

// Seven stars = one constellation
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

// Several constellations = one star system
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
	createCluster(-10, -30, 40);
	createCluster(0, -40, 0);
	createCluster(-40, -35, -60);
	createCluster(-20, 10, 90);
	createCluster(-10, 20, 40);
	createCluster(15, 25, -30);

}

// Creates Mercury, Venus, Earth ..... and Saturn
void createPlanets() {

	planet mercury = planet();
	mercury.planetNo = 1;
	mercury.angle = 0;	
	mercury.color1[0] = 0.56;
	mercury.color1[1] = 0.26;
	mercury.color1[2] = 0.56;
	mercury.color2[0] = 0.6;
	mercury.color2[1] = 0.3;
	mercury.color2[2] = 0.6;
	mercury.distFromRef = 45;
	mercury.size = 0.2;
	mercury.angularSpeed = 5;

	planet venus = planet();
	venus.planetNo = 2;
	venus.angle = 0;
	venus.color1[0] = 1;
	venus.color1[1] = 0.25;
	venus.color1[2] = 0;
	venus.color2[0] = 1.6;
	venus.color2[1] = 0.32;
	venus.color2[2] = 0;
	venus.distFromRef = 70;
	venus.size = 0.25;
	venus.angularSpeed = 20;

	planet earth = planet();
	earth.planetNo = 3;
	earth.angle = 0;
	earth.color1[0] = 0.19;
	earth.color1[1] = 0.19;
	earth.color1[2] = 0.8;
	earth.color2[0] = 0.24;
	earth.color2[1] = 0.24;
	earth.color2[2] = 0.85;
	earth.distFromRef = 90;
	earth.size = 0.3;
	earth.angularSpeed = 10;
	earth.noOfMoons = 1;
	earth.moonSpeed = 20;

	planet saturn = planet();
	saturn.planetNo = 4;
	saturn.angle = 0;
	saturn.color1[0] = 0.71;
	saturn.color1[1] = 0.53;
	saturn.color1[2] = 0.53;
	saturn.color2[0] = 0.73;
	saturn.color2[1] = 0.56;
	saturn.color2[2] = 0.56;
	saturn.distFromRef = 100;
	saturn.size = 0.35;
	saturn.angularSpeed = 15;
	saturn.noOfRings = 1;
	if (ISCLOCK == true) {
		saturn.planetOpcacity = 0.5;
	}

	planetList[0] = mercury;
	planetList[1] = venus;
	planetList[2] = earth;
	planetList[3] = saturn;

}

void drawSolarSystem() {
	glPushMatrix();

	for (int i = 0; i < numPlanets; i++) {
		glPushMatrix();
		angles[i] = angles[i] + planetList[i].angularSpeed/100; //incrementing angle based on speed of movement of planet, this changes with frame rate
		if (angles[i] > 360) { angles[i] = 0; } 

		if (i == 1) { //venus is elliptical
			glTranslatef(0, -17, 0);
			planetList[i].drawEllipticalOrbit(angles[i]);
		}
		else {
			glRotated(angles[i], 0, 0, 1);
			planetList[i].drawPlanet();
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
		planetList[i].drawPlanet();
		glPopMatrix();
	}

	planetList[3].drawPlanet();
	glPopMatrix();
}

void createSolarSystem() {
	glPushMatrix();
	sun Sun = sun();
	Sun.drawSun();
	createPlanets();
	createStarSystem();

	//check if clock mode is on
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
	//animation updated in drawSolarSystem function
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

		case 'x':
			DEATHSTAR = !DEATHSTAR;
		break;

		case 'f':
			FLASH = !FLASH;
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
