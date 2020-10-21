#include <stdlib.h>  
#include <GL/glut.h>  
#include<iostream>
#include <Windows.h>
#pragma comment(lib, "glut32.lib")  
using namespace std;
GLfloat roate = 0.0;// set rote of roate ying yu bu hao  bu zhuang le 设置旋转速率
GLfloat rote = 0.0;//shezhi旋转角度
bool flag = 1;
GLfloat headx = 0;
GLfloat heady = 0;
GLfloat tmpX;
GLfloat tmpY;


void init(void)
{
	glClearColor(0.5, 0.5, 0.8, 1.0); //背景黑色  
}
void makeBackGround(GLfloat r, GLfloat g, GLfloat b)
{
	glClearColor(r / 255, g / 255, b / 255, 1.0);
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	//if(rote)
	//橙红255，69，0到麒麟黄 250，250，210
	if (flag) {
		if (rote <= 180) makeBackGround(255 - rote * 5 / 180, 69 + rote * 181 / 180, rote * 210 / 180);
		else makeBackGround(250 + (rote - 180) * 5 / 180, 250 - (rote - 180) * 181 / 180, 210 - (rote - 180) * 210 / 180);
	}
	else
	//天空蓝0,191,255到深蓝0,0,139
	{
		if (rote <= 180) makeBackGround(0, 191 - rote * 191 / 180, 255 - rote * 116 / 180);
		else makeBackGround(0, (rote - 180) * 191 / 180, 139 + (rote - 180) * 116 / 180);
	}
	
	//画笔绿玉127，255，170至紫红色255，0，255
	if (flag)
	{
		if (rote < 180) glColor3ub(127 + rote * 128 / 180, 255 - rote * 255 / 180, 170 + rote * 85 / 180);
		else if (rote == 180) glColor3ub(255, 0, 255);
		else glColor3ub(255 - (rote - 180) * 128 / 180, (rote - 180) * 255 / 180, 255 - (rote - 180) * 85 / 180);
	}
	else
	{//纯黄255，255，0到番木瓜255，239，213
		if (rote <= 180) glColor3ub(255, 255 - rote * 16 / 180, rote * 213 / 180);
		else glColor3ub(255, 239 + (rote - 180) * 16 / 180, 213 - (rote - 180) * 213 / 180);
	}
	glLoadIdentity();  //加载单位矩阵  
	gluLookAt(headx/100, heady / 100, 5.0, headx / 100, heady / 100, 0.0, 0.0, 1.0, 0);
	
	glRotatef(rote, 0, 1.0, 0);
	glutWireTeapot(2);
	rote += roate;
	//glRotatef(angle, 0.0, 1.0, 0.0);
	//angle += 1.0f;
	glutSwapBuffers();
	rote += 1.0;
	if (rote == 360) rote = 0;
	Sleep(30);
}
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void handleKeyboard(unsigned char key, int x, int y)
{
	if (key == 'c') flag = !flag;
	else if (key == 'r') headx = heady = 0;
}

void handleMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		//cout << x << " " << y << endl;
		tmpX = x;
		tmpY = y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		headx = tmpX - x;
		heady = y - tmpY;
		cout << headx << " " << heady << endl;
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	glutKeyboardFunc(handleKeyboard);
	glutMouseFunc(handleMouseClick);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}