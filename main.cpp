#define M_PI 3.14159265358979323846
#include"freeglut.h"
#include<cmath>
using namespace std;

GLuint AthomHeart, AthomList, PyramideBodyList, PyramideHeadList;


int windowHeight = 800;
int windowWidth = 1400;
int windowPosX = 0;
int windowPosY = 0;

int currentPrimitive = 1;

float cameraZoom = 5.0f;
float cameraZoom2 = 5.0f;
float cameraAngle = 0.0f;
float angle1 = 0.0f;
float angle2 = 0.0f;
float rotateSpeed = 1.5f;


float jumpDirection  = 0;
bool isJupm = false;
bool up = true;

void drawCircle(float radius, float semnents)
{
	glPushMatrix();
		glBegin(GL_LINE_LOOP);
			glColor3f(1, 1, 1);

			for (int i = 0; i < semnents; ++i)
			{
				float theta = 2.0 * M_PI * static_cast<float>(i) / static_cast<float>(semnents);
				float x = radius * std::cos(theta);
				float y = radius * std::sin(theta);
				glVertex2f(x, y);
			}
		glEnd();
	glPopMatrix();
}

void drawSphere(float angle, float radius, float posX, float posY, float posZ)
{
	glPushMatrix();
	glRotatef(angle, 0, 1, 0);
	glTranslatef(posX, posY, posZ);
	glutWireSphere(radius, 20, 20);
	glPopMatrix();
}


void initGL()
{
	glClearColor(0, 0, 0, 1);
	glEnable(GL_DEPTH_TEST);

	AthomHeart = glGenLists(1);
	glNewList(AthomHeart, GL_COMPILE);
		glPushMatrix();
			glColor3f(1, 0, 0);
			drawSphere(0, 1, 1, 0, 0);
			drawSphere(0, 1, -1, 0, 0);
			drawSphere(0, 1, 0, 0, -1);
			glColor3f(0, 0, 1);
			drawSphere(0, 1, 0, 0, 1);
			drawSphere(0, 1, 1, 1, 0);
			drawSphere(0, 1, -1, 1, 0);
			drawSphere(0, 1, 0, 1, -1);
			glColor3f(1, 0, 0);
			drawSphere(0, 1, 0, 1, 1);
		glPopMatrix();
	glEndList();

	AthomList = glGenLists(1);
	glNewList(AthomList,GL_COMPILE);

		glPushMatrix();
			drawCircle(5, 50);
			glRotatef(90, 1, 0, 0);
			drawCircle(5, 50);
			glRotatef(-45, 1, 0, 0);
			drawCircle(5, 50);
			glPopMatrix();
	glEndList();

	PyramideBodyList = glGenLists(1);
	glNewList(PyramideBodyList, GL_COMPILE);
		glBegin(GL_LINE_LOOP);
			glVertex3f(2, 0, 2);
			glVertex3f(2, 0, -2);
			glVertex3f(-2, 0, -2);
			glVertex3f(-2, 0, 2);
		glEnd();

		glBegin(GL_LINE_LOOP);
			glVertex3f(1, 1, 1);
			glVertex3f(1, 1, -1);
			glVertex3f(-1, 1, -1);
			glVertex3f(-1, 1, 1);
		glEnd();

		glBegin(GL_LINES);
			glVertex3f(2, 0, 2);
			glVertex3f(1, 1, 1);

			glVertex3f(2, 0, -2);
			glVertex3f(1, 1, -1);

			glVertex3f(-2, 0, -2);
			glVertex3f(-1, 1, -1);

			glVertex3f(-2, 0, 2);
			glVertex3f(-1, 1, 1);
		glEnd();
	glEndList();

	PyramideHeadList = glGenLists(1);
	glNewList(PyramideHeadList, GL_COMPILE);
		glBegin(GL_TRIANGLES);
			glVertex3f(1, 1, 1);
			glVertex3f(1, 1, -1);
			glVertex3f(0, 2, 0);
			
			glVertex3f(1, 1, -1);
			glVertex3f(-1, 1, -1);
			glVertex3f(0, 2, 0);

			glVertex3f(-1, 1, -1);
			glVertex3f(-1, 1, 1);
			glVertex3f(0, 2, 0);

			glVertex3f(-1, 1, 1);
			glVertex3f(1, 1, 1);
			glVertex3f(0, 2, 0);
		glEnd();
	glEndList();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glLineWidth(2.0f);
	if (currentPrimitive == 1)
	{
		gluLookAt(cameraZoom, cameraZoom, 5,
			0, 0, 0,
			0, 1, 0
		);
	}
	else if (currentPrimitive == 2)
	{
		gluLookAt(cameraZoom2, cameraZoom2, cameraZoom2,
			cameraAngle, cameraAngle, 0,
			0, 1, 0
		);
	}

	if (currentPrimitive == 1) {

		glPushMatrix();
			glRotatef(angle1 / 2, 1, 1, 0);
			glCallList(AthomHeart);
		glPopMatrix();
		glCallList(AthomList);

		glPushMatrix();
			glColor3f(1, 1, 1);
			drawSphere(angle1 * 5, 0.5, 5, 0, 0);

			glRotatef(angle1 * 5, 0, 0, 1);
			drawSphere(45, 0.5, 0, 5, 0);
		glPopMatrix();

		glPushMatrix();
			glRotatef(135, 1, 0, 0);
			drawSphere(angle1 * 5, 0.5, 5, 0, 0);
		glPopMatrix();
	}
	else if(currentPrimitive == 2)
	{
		glPushMatrix();
			if(isJupm)
				glRotatef(angle2 * 3, 0, 1, 0);
			glCallList(PyramideBodyList);
		glPopMatrix();

		glPushMatrix();
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINES);
			glTranslatef(0, jumpDirection, 0);
			glCallList(PyramideHeadList);
		glPopMatrix();
	}

	glutSwapBuffers();
}

void update(int value)
{
	if (currentPrimitive == 1) {
		cameraZoom2 = 5.0f;
		angle2 = 0.0f;

		angle1 += rotateSpeed;
		cameraZoom += 0.01;
	}
	else if (currentPrimitive == 2)
	{
		cameraZoom = 5.0f;
		angle1 = 0.0f;

		angle2 += rotateSpeed;

		if (isJupm) {
			if (up)
			{
				jumpDirection += 0.03;
				if (jumpDirection >= 1)
					up = false;

			}
			else 
			{
				jumpDirection -= 0.03;
				if (jumpDirection <= 0.0)
				{
					jumpDirection = 0.0;
					up = true;
					isJupm = false;
				}
			}

		}


	}
	glutPostRedisplay();
	glutTimerFunc(15, update, 0);
}

void keyBoard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0);
		break;
	}
}
void specialKeyBoard(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		currentPrimitive = 1;
		break;
	case GLUT_KEY_F2:
		currentPrimitive = 2;
		break;
	case GLUT_KEY_SHIFT_L:
		if (currentPrimitive == 2)
		{
			isJupm = true;
		}
		break;
	case GLUT_KEY_UP:
		if (currentPrimitive == 2)
		{
			cameraZoom2 -= 0.2;
		}
		break;
	case GLUT_KEY_DOWN:
		if (currentPrimitive == 2)
		{
			cameraZoom2 += 0.2;
		}
		break;
	case GLUT_KEY_RIGHT:
		if (currentPrimitive == 2)
			cameraAngle += 0.2;
		break;
	case GLUT_KEY_LEFT:
		if (currentPrimitive == 2)
			cameraAngle -= 0.2;
		break;
	}

}

void resize(int width, int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(45.0f, (float)width / height, 0.1f, 100.f);
	glMatrixMode(GL_MODELVIEW);
}

int main(int args, char** argv)
{
	glutInit(&args, argv);
	const int SCREEN_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
	const int SCREEN_WIDTH = glutGet(GLUT_SCREEN_WIDTH);

	windowPosX = (SCREEN_WIDTH - windowWidth) / 2;
	windowPosY = (SCREEN_HEIGHT - windowHeight) / 2;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(windowPosX, windowPosY);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("CI PLAS PLAS");

	initGL();
	glutReshapeFunc(resize);
	glutKeyboardFunc(keyBoard);
	glutSpecialFunc(specialKeyBoard);
	glutDisplayFunc(display);

	glutTimerFunc(15, update, 0);
	glutMainLoop();

	return 0;
}