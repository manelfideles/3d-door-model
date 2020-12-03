
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <GL/freeglut.h>
#include "RgbImage.h"
#include <iostream>

// SYSTEM COLOR 4F
#define WHITE		1.0, 1.0, 1.0, 1.0

// COLORS 3F
#define BLUE		0.0, 0.0, 1.0
#define RED			1.0, 0.0, 0.0
#define GREEN		0.0, 1.0, 0.0
#define YELLOW		1.0, 1.0, 0.0
#define BLACK		0.0, 0.0, 0.0
#define BROWN		0.31, 0.16, 0

// MACROS
#define PI			3.14159
#define DOOR_TOP_SCALE_X 5.0
#define DOOR_TOP_SCALE_Y 5.0
#define DOOR_TOP_SCALE_Z 0.5
#define DOOR_TOP_SCALE	DOOR_TOP_SCALE_X, DOOR_TOP_SCALE_Y, DOOR_TOP_SCALE_Z
#define DOOR_BOTTOM_SCALE_X 1.25
#define DOOR_BOTTOM_SCALE_Y 2.0
#define DOOR_BOTTOM_SCALE_Z 0.5
#define DOOR_BOTTOM_SCALE DOOR_BOTTOM_SCALE_X, DOOR_BOTTOM_SCALE_Y, DOOR_BOTTOM_SCALE_Z

// VARIABLES/CONSTS
int tipoProjeccao = 0;
GLfloat tam = 0.5;
GLfloat door_angle = 0;
GLfloat dog_door_angle = 0;
GLfloat handle_angle = 0;
GLfloat trinco_comp = 0.65;
GLfloat trinco_larg = 0.125;
GLfloat trinco_angle = 90;
GLfloat trinco_pos = 2 * DOOR_TOP_SCALE_X * tam; // initial position = locked
bool door_locked = true;
int angle_increment = 2;
char instrucoes[100];

// COLORS
static GLfloat COR_PORTA[] = { BROWN };
static GLfloat COR_CAO[] = { YELLOW };
static GLfloat COR_MACANETA[] = { GREEN };
static GLfloat COR_TRINCO[] = { BLUE };
static GLfloat COR_PAREDE[] = { BLACK };

// SYSTEM COORDINATES
GLint wScreen = 800, hScreen = 600;				// janela (pixeis)
GLfloat xC = 10.0, yC = 10.0, zC = 10.0;		// Mundo  (unidades mundo)

// VISUALIZATION AND OBSERVER
GLfloat rVisao = 10, aVisao = 0.5 * PI, incVisao = 0.05;
GLfloat obsP[] = {rVisao * cos(aVisao), 3.0, rVisao * sin(aVisao)};
GLfloat obsT[] = {obsP[0] - rVisao * cos(aVisao), obsP[1], obsP[2] - rVisao * sin(aVisao)};
GLfloat angZoom = 90;
GLfloat incZoom = 3;

//TEXTURES
GLuint   texture[5];
RgbImage imag;

// INIT
void initTextures() {

	// PAREDE
	glGenTextures(1, &texture[0]);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
	imag.LoadBmpFile("pedra.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	// PORTA 
	glGenTextures(1, &texture[1]);
	glBindTexture(GL_TEXTURE_2D, texture[1]);
	imag.LoadBmpFile("madeira.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	// TAPETE
	glGenTextures(1, &texture[2]);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
	imag.LoadBmpFile("colorida.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

	// CANDEEIRO + MAÇANETA + FERROLHO
	glGenTextures(1, &texture[3]);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
	imag.LoadBmpFile("metal.bmp");
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, 3,
		imag.GetNumCols(),
		imag.GetNumRows(), 0, GL_RGB, GL_UNSIGNED_BYTE,
		imag.ImageData());

}
void inicializa(void) {

	glClearColor(WHITE);		// Apagar
	glShadeModel(GL_SMOOTH);	// Interpolacao de cores	
	initTextures();
	glEnable(GL_DEPTH_TEST);	// Profundidade

}
void drawText(char* string, GLfloat x, GLfloat y) {
	glPushMatrix();
	glTranslatef(x, y, 0);
	glRasterPos2f(0, 0);
	while (*string)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *string++);
	glPopMatrix();
}

// DRAWING
void drawEixos() {
	// Eixo X
	glColor4f(1.0, 0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(10, 0, 0);
	glEnd();
	// Eixo Y
	glColor4f(0.0, 1.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 10, 0);
	glEnd();
	// Eixo Z
	glColor4f(0.0, 0.0, 1.0, 1.0);
	glBegin(GL_LINES);
	glVertex3i(0, 0, 0);
	glVertex3i(0, 0, 10);
	glEnd();
}
void drawObservador() {
	glColor4f(GREEN, 1.0);
	glPushMatrix();
	glTranslatef(obsP[0], obsP[1], obsP[2]);
	glutSolidCube(1);
	glPopMatrix();
}
void drawCube(float tam, GLfloat color[3]) {

	//glEnable(GL_TEXTURE_2D);

	GLfloat vcube[] = {
		// Esquerda
			-tam,  -tam,  tam,	// 0
			-tam,   tam,  tam,	// 1 
			-tam,   tam, -tam,	// 2 
			-tam,  -tam, -tam,	// 3 
		// Direita
			 tam,  -tam,  tam,	// 4 
			 tam,   tam,  tam,	// 5 
			 tam,   tam, -tam,	// 6 
			 tam,  -tam, -tam,	// 7
		// Cima
			-tam,  tam,  tam,	// 8 
			-tam,  tam, -tam,	// 9 
			 tam,  tam, -tam,	// 10 
			 tam,  tam,  tam,	// 11 
		// Baixo
			-tam,  -tam,  tam,	// 12
			-tam,  -tam, -tam,	// 13
			 tam,  -tam, -tam,	// 14
			 tam,  -tam,  tam,	// 15
		// Frente
			-tam,  -tam,  tam,	// 16
			-tam,   tam,  tam,	// 17
			 tam,   tam,  tam,	// 18
			 tam,  -tam,  tam,	// 19
		// Trás
			-tam,  -tam, -tam,	// 20
			-tam,   tam, -tam,	// 21
			 tam,   tam, -tam,	// 22
			 tam,  -tam, -tam,	// 23
	};
	GLfloat ncube[] = {
		// Esquerda 
	   -1.0,  0.0,  0.0,
	   -1.0,  0.0,  0.0,
	   -1.0,  0.0,  0.0,
	   -1.0,  0.0,  0.0,
	   // Direita
	   1.0,  0.0,  0.0,
	   1.0,  0.0,  0.0,
	   1.0,  0.0,  0.0,
	   1.0,  0.0,  0.0,
	   // Cima
	   0.0,  1.0,  0.0,
	   0.0,  1.0,  0.0,
	   0.0,  1.0,  0.0,
	   0.0,  1.0,  0.0,
	   // Baixo
	   0.0, -1.0,  0.0,
	   0.0, -1.0,  0.0,
	   0.0, -1.0,  0.0,
	   0.0, -1.0,  0.0,
	   // Frente
	   0.0, 0.0, 1.0,
	   0.0, 0.0, 1.0,
	   0.0, 0.0, 1.0,
	   0.0, 0.0, 1.0,
	   // Trás
	   0.0, 0.0, -1.0,
	   0.0, 0.0, -1.0,
	   0.0, 0.0, -1.0,
	   0.0, 0.0, -1.0,
	};
	GLfloat ccube[24 * 3];
	int k = 0;
	for (int j = 0; j < 24; j++) {
		for (int i = 0; i < 3; i++) {
			ccube[k] = color[i];
			k++;
		}
	}
	GLfloat tcube[] = {
		// Esquerda
			0, 0,
			1, 0,
			1, 1,
			0, 1,
		// Direita
			0, 0,
			1, 0,
			1, 1,
			0, 1,
		// Cima
			0, 0,
			1, 0,
			1, 1,
			0, 1,
		// Baixo
			0, 0,
			1, 0,
			1, 1,
			0, 1,
		// Frente
			0, 0,
			1, 0,
			1, 1,
			0, 1,
		// Trás
			0, 0,
			1, 0,
			1, 1,
			0, 1,
	};

	GLuint esquerda[] = { 0, 1, 2, 3 };
	GLuint direita[] = { 4, 7, 6, 5 };
	GLuint cima[] = { 8, 11, 10,  9 };
	GLuint baixo[] = { 15, 12, 13, 14 };
	GLuint frente[] = { 16, 19, 18, 17 };
	GLuint tras[] = { 20, 23, 22, 21 };

	glVertexPointer(3, GL_FLOAT, 0, vcube);
	glEnableClientState(GL_VERTEX_ARRAY);

	glNormalPointer(GL_FLOAT, 0, ncube);
	glEnableClientState(GL_NORMAL_ARRAY);

	glColorPointer(3, GL_FLOAT, 0, ccube);
	glEnableClientState(GL_COLOR_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, tcube);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, cima);
	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, baixo);
	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, frente);
	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, tras);
	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, esquerda);
	//glBindTexture(GL_TEXTURE_2D, texture[1]);
	glDrawElements(GL_POLYGON, 4, GL_UNSIGNED_INT, direita);

	//glDisable(GL_TEXTURE_2D);

}
void drawMacaneta() {

	glPushMatrix();
	glTranslatef(1.76 * DOOR_TOP_SCALE_X * tam - 0.50 * tam, 1.525 * DOOR_TOP_SCALE_Y * tam, tam);
	glRotatef(handle_angle, 0.0, 0.0, 1.0);

	// EXTENSAO
	glPushMatrix();
	glTranslatef(-0.25, 0, 0.25 * 0.5);
	glScalef(0.7, 0.25, 0.25);
	drawCube(tam, COR_MACANETA);
	glPopMatrix();

	// CABO
	glPushMatrix();
	glScalef(0.25, 0.25, 0.5);
	drawCube(tam, COR_MACANETA);
	glPopMatrix();
	glPopMatrix();
}
void drawPortaDoCao() {

	glPushMatrix();
	glTranslatef(4 * DOOR_BOTTOM_SCALE_X * tam, 2 * DOOR_BOTTOM_SCALE_Y * tam, 0);
	glRotatef(dog_door_angle, 1.0, 0.0, 0.0);
	glTranslatef(0, -DOOR_BOTTOM_SCALE_Y * tam, 0);
	glScalef(DOOR_BOTTOM_SCALE_X * 2, DOOR_BOTTOM_SCALE_Y, DOOR_BOTTOM_SCALE_Z * 0.2);
	drawCube(tam, COR_CAO);
	glPopMatrix();
}
void drawCuboDeBaixo(float pos_x) {
	glPushMatrix();
		glTranslatef(pos_x, DOOR_BOTTOM_SCALE_Y * tam, 0);
		glScalef(DOOR_BOTTOM_SCALE);
		drawCube(tam, COR_PORTA);
	glPopMatrix();
}
void drawParteDeCima() {
	glTranslatef(DOOR_TOP_SCALE_X * tam, 2 * DOOR_BOTTOM_SCALE_Y * tam + DOOR_TOP_SCALE_Y * tam, 0);
	glScalef(DOOR_TOP_SCALE);
	drawCube(tam, COR_PORTA);
}
void drawTapete() {
	glPushMatrix();
		glTranslatef(tam * DOOR_TOP_SCALE_X, 0, DOOR_BOTTOM_SCALE_Z * 3);
		glScalef(tam * DOOR_TOP_SCALE_X, trinco_larg, tam * 0.75 * DOOR_TOP_SCALE_X);
		drawCube(tam, COR_TRINCO);
	glPopMatrix();
}
void drawCandeeiro() {
	glPushMatrix();
		glTranslatef(DOOR_TOP_SCALE_X * tam, 2.95 * DOOR_TOP_SCALE_Y * tam, tam * 1.25);
		glScalef(trinco_larg * 5, trinco_larg * 2, 1.25);
		drawCube(tam, COR_TRINCO);
	glPopMatrix();
}
void drawTrinco(float comp, float larg) {
	glPushMatrix();
		glTranslatef(trinco_pos, 1.35 * DOOR_TOP_SCALE_Y * tam, DOOR_TOP_SCALE_Z * tam + larg * tam);
		glRotatef(trinco_angle, 1.0, 0.0, 0.0);

			// EXTENSÃO
			glPushMatrix();
				glTranslatef(-comp * 0.4125, 0, (comp - larg * 3.25) * tam);
				glScalef(larg, larg, 0.35);
				drawCube(tam, COR_CAO);
			glPopMatrix();

			// CABO
			glPushMatrix();
				glScalef(comp, larg, larg);
				drawCube(tam, COR_TRINCO);
			glPopMatrix();
	glPopMatrix();
}
void drawParede() {

	glPushMatrix();
		glTranslatef((DOOR_TOP_SCALE_X + 0.55) * tam, 2.95 * DOOR_TOP_SCALE_Y * tam, 0);
		glScalef(DOOR_TOP_SCALE_X + DOOR_TOP_SCALE_Z + 0.15, DOOR_TOP_SCALE_Z, DOOR_TOP_SCALE_Z);
		drawCube(tam, COR_PAREDE);
	glPopMatrix();

	glPushMatrix();
		glTranslatef(2 * (DOOR_TOP_SCALE_X + 0.35) * tam, (DOOR_TOP_SCALE_Y + 2.5) * tam, 0);
		glScalef(tam, DOOR_BOTTOM_SCALE_Y + DOOR_TOP_SCALE_Y + 0.5, DOOR_TOP_SCALE_Z);
		drawCube(tam, COR_PAREDE);
	glPopMatrix();
}
void drawDoor() {

	// PAREDE
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[0]);
		drawParede();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[2]);
		drawTapete();
	glDisable(GL_TEXTURE_2D);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture[3]);
		drawCandeeiro();
	glDisable(GL_TEXTURE_2D);

	glPushMatrix();
		glRotatef(door_angle, 0.0, 1.0, 0.0);

		// TRINCO + MAÇANETA
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[3]);
			drawTrinco(trinco_comp, trinco_larg);
			drawMacaneta();
		glDisable(GL_TEXTURE_2D);

		// transparente
		drawPortaDoCao();

		// PORTA
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture[1]);
			drawCuboDeBaixo(2 * DOOR_TOP_SCALE_X * tam - DOOR_BOTTOM_SCALE_X * tam);
			drawCuboDeBaixo(DOOR_BOTTOM_SCALE_X * tam);
			drawParteDeCima();
		glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// ANIMATIONS
void animCloseHandle(int n) {
	if (handle_angle != 0) {
		handle_angle -= angle_increment * 0.5;
		glutPostRedisplay();
		glutTimerFunc(5, animCloseHandle, 0);
	}
}
void animOpenHandle(int n) {
	if (handle_angle < 40) {
		handle_angle += angle_increment * 0.5;
		glutPostRedisplay();
		glutTimerFunc(2, animOpenHandle, 0);
	}
	if (handle_angle == 40) glutTimerFunc(2, animCloseHandle, 0);
}
void animOpenDoor(int n) {
	if (door_angle != 90) {
		door_angle += angle_increment;
		glutPostRedisplay();
		glutTimerFunc(40, animOpenDoor, 0);
	}
}
void animCloseDoor(int n) {
	if (door_angle != 0) {
		door_angle -= angle_increment;
		glutPostRedisplay();
		glutTimerFunc(40, animCloseDoor, 0);
	}
}
void animOpenTrinco(int n) {
	// LOCKED -> UNLOCKED
	if (trinco_angle > 0 || (trinco_angle == 90 && door_locked == true)) {
		trinco_angle -= angle_increment;
		glutPostRedisplay();
		glutTimerFunc(1, animOpenTrinco, 0);
	}
}
void animCloseTrinco(int n) {
	// UNLOCKED -> LOCKED
	if (trinco_angle < 90 || (trinco_angle == 0 && door_locked == false)) {
		trinco_angle += angle_increment;
		glutPostRedisplay();
		glutTimerFunc(2, animCloseTrinco, 0);
	}
}
void animUnlockDoor(int n) {
	float travel_dist = trinco_comp * 0.52;

	if (trinco_pos > 2 * DOOR_TOP_SCALE_X * tam - travel_dist) {
		trinco_pos -= travel_dist * 0.1;
		glutPostRedisplay();
		glutTimerFunc(5, animUnlockDoor, 0);
	}
}
void animLockDoor(int n) {
	float travel_dist = trinco_comp * 0.52;

	if (trinco_pos < 2 * DOOR_TOP_SCALE_X * tam) {
		trinco_pos += travel_dist * 0.1;
		glutPostRedisplay();
		glutTimerFunc(5, animLockDoor, 0);
	}
}
void display(void) {
	// Apaga ecra e lida com profundidade (3D)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Main View
	glViewport(0, 0, wScreen, hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angZoom, (float)wScreen / hScreen, 0.1, 3 * zC);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(obsP[0], obsP[1], obsP[2], 0, 0, 0, 0, 1, 0);

	// Objetos
	drawEixos();
	drawDoor();

	// ---------------------------------

	// Mini-map
	glViewport(-0.075 * wScreen, 0, 0.40 * wScreen, 0.40 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20, 20, -20, 20, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 30, 0, 0, 0, 0, 0, 0, -1);

	// Objectos
	drawEixos();
	drawObservador();
	drawDoor();

	// ---------------------------------

	// Front-view only
	glViewport(0.15 * wScreen, -0.05 * hScreen, 0.4 * wScreen, 0.4 * hScreen);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20, 20, -20, 20, -100, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(3, 3, 6, 3, 3, 3, 0, 1, 0);

	drawEixos();
	drawDoor();


	// Actualizacao
	glutSwapBuffers();
}

// EVENTS
void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'e':
	case 'E':
		if (dog_door_angle > -90) dog_door_angle -= angle_increment * 4; // ABRE DOG DOOR
		glutPostRedisplay();
		break;
	case 'd':
	case 'D':
		if (dog_door_angle < 90) dog_door_angle += angle_increment * 4; // FECHA DOG DOOR
		glutPostRedisplay();
		break;
	case 'o':
	case 'O':
		if (!door_locked) {
			glutTimerFunc(50, animOpenHandle, 0);
			glutTimerFunc(200, animOpenDoor, 0);
		}
		break;
	case 'c':
	case 'C':
		if (!door_locked) glutTimerFunc(200, animCloseDoor, 0);
		break;
	case 'l':
	case 'L':
		if (!door_locked) {
			glutTimerFunc(1, animLockDoor, 0);
			if (trinco_angle == 0) glutTimerFunc(200, animCloseTrinco, 0);
		}
		door_locked = true;
		break;
	case 'u':
	case 'U':
		if (door_locked) {
			if (trinco_angle == 90) glutTimerFunc(1, animOpenTrinco, 0);
			glutTimerFunc(200, animUnlockDoor, 0);
		}
		door_locked = false;
		break;
	case 'p':
	case 'P':
		tipoProjeccao = (tipoProjeccao + 1) % 2;
		glutPostRedisplay();
		break;
		// Escape
	case 27:
		exit(0);
		break;
	}
}
void teclasNotAscii(int key, int x, int y) {

	// observador pode andar para cima e para baixo (setas cima / baixo )
	if (key == GLUT_KEY_UP)   obsP[1] = (obsP[1] + 0.1);
	if (key == GLUT_KEY_DOWN) obsP[1] = (obsP[1] - 0.1);

	// observador pode andar à volta da cena  (setas esquerda / direita)
	if (key == GLUT_KEY_LEFT)  aVisao = (aVisao + 0.1);
	if (key == GLUT_KEY_RIGHT) aVisao = (aVisao - 0.1);

	// limita altura
	if (obsP[1] > yC)   obsP[1] = yC;
	if (obsP[1] < -yC)  obsP[1] = -yC;

	// atualiza posicao (X,Z)
	obsP[0] = rVisao * cos(aVisao);
	obsP[2] = rVisao * sin(aVisao);

	glutPostRedisplay();
}

// MAIN
int main(int argc, char** argv) {

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(wScreen, hScreen);
	glutInitWindowPosition(300, 100);
	glutCreateWindow("MANUEL FIDELES 2018282990");

	inicializa();

	glutSpecialFunc(teclasNotAscii);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}
