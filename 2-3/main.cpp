#include <stdlib.h>  
#include <GL/glut.h>  
#include<iostream>
#include <fstream>
#include <vector>
#pragma comment(lib, "glut32.lib")  
//Í¼²ã
const int WIDTH = 600;
const int HEIGHT = 600;
const int baseX = 30;
const int baseY = 30;
const int dis = 60;

GLfloat image[HEIGHT][WIDTH][3];
using namespace std;
GLfloat vertices[36];

void init(void)
{
	glClearColor(0, 0, 0, 1.0); //±³¾°ºÚÉ«  
}


void setImage(int x, int y, float r, float g, float b)
{
	image[y][x][0] = r;
	image[y][x][1] = g;
	image[y][x][2] = b;
}
int z = 0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 1);
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	//glLoadIdentity();
	glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_FLOAT, image);
	glutSwapBuffers();
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
enum condition{
	lie,
	stand
};

class square
{
public:
	GLfloat color[3];
	condition cond;
	int turn;
	void set(condition con, int t)
	{
		cond = con;
		turn = t;
		if (cond == lie)
		{
			color[0] = (float)68 / (float)255;
			color[1] = (float)114 / (float)255;
			color[2] = (float)196 / (float)255;
		}
		else
		{
			color[0] = (float)112 / (float)255;
			color[1] = (float)48 / (float)255;
			color[2] = (float)160 / (float)255;
		}
	}
	int getMinX()
	{
		if (cond == stand) return (dis + 2 * dis * turn + baseX);
		else return baseX;
	}
	int getMaxX()
	{
		if (cond == stand) return (dis + 2 * dis * turn + baseX + dis);
		else return baseX + dis * 9;
	}
	int getMinY()
	{
		if (cond == stand) return baseY;
		else return (baseY + dis + 2 * dis * turn);
	}
	int getMaxY()
	{
		if (cond == stand) return baseY + dis * 9;
		else return (baseY + dis + 2 * dis * turn + dis);
	}
};
int devide(int x, int y)
{
	int xx = 0;
	int yy = 0;
	if (x - baseX >= dis && x - baseX < 2 * dis) xx = 0;
	else if (x - baseX >= 3 * dis && x - baseX < 4 * dis) xx = 1;
	else if (x - baseX >= 5 * dis && x - baseX < 6 * dis) xx = 2;
	else if (x - baseX >= 7 * dis && x - baseX < 8 * dis) xx = 3;
	else return -1;

	if (y - baseY >= dis && y - baseY < 2 * dis) yy = 0;
	else if (y - baseY >= 3 * dis && y - baseY < 4 * dis) yy = 1;
	else if (y - baseY >= 5 * dis && y - baseY < 6 * dis) yy = 2;
	else if (y - baseY >= 7 * dis && y - baseY < 8 * dis) yy = 3;
	else return -1;

	return xx + yy;
}

bool depth(int x, int y, condition cond)
{
	int tmp;
	if ((tmp = devide(x, y)) == -1) return true;
	if ((cond == lie) && ((tmp % 2) == 0)) return false;
	else if ((cond == stand) && ((tmp % 2) == 1)) return false;
	return true;
}

void draw()
{
	square s[8];
	for (int i = 0; i < 4; ++i)
	{
		s[i].set(stand, i);
		s[i + 4].set(lie, i);
	}
	for (int i = 0; i < 8; ++i)
	{
		//cout << s[i].color[0] << " " << s[i].color[0] << " " << s[i].color[0] << endl;
		//if (s[i].cond == lie) cout << s[i].getMinX() << " " << s[i].getMaxX() << endl;
		for (int x = s[i].getMinX(); x < s[i].getMaxX(); ++x)
		{
			for (int y = s[i].getMinY(); y < s[i].getMaxY(); ++y)
			{
				if (!depth(x, y, s[i].cond)) continue;
				setImage(x, y, s[i].color[0], s[i].color[1], s[i].color[2]);
			}
		}
	}
}

int main(int argc, char** argv)
{

	for (int i = 0; i < WIDTH; ++i)
		for (int j = 0; j < HEIGHT; ++j) setImage(i, j , 1, 1, 1);
	draw();

	//cout << Y_MIN << " " << Y_MAX << endl;

	//tri[0].print();
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutIdleFunc(display);
	glutMainLoop();
	return 0;
}