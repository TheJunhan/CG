#include <stdlib.h>  
#include <GL/glut.h>  
#include<iostream>
#include <fstream>
#include <vector>
#pragma comment(lib, "glut32.lib")  
//图层
const int WIDTH = 600;
const int HEIGHT = 600;
int Y_MAX = 0;
int Y_MIN = HEIGHT;
GLfloat image[HEIGHT][WIDTH][3];
int depth[WIDTH][HEIGHT];
using namespace std;
GLfloat vertices[36];

void init(void)
{
	glClearColor(0, 0, 0, 1.0); //背景黑色  
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
//点
class Point
{
	float pos[3];
	int rgb[3];
public:
	void init(float* v, float* c)
	{
		pos[0] = v[0];
		pos[1] = v[1];
		pos[2] = v[2];
		rgb[0] = c[0];
		rgb[1] = c[1];
		rgb[2] = c[2];
		//cout << rgb[0] << ", " << rgb[1] << ", " << rgb[2] << ", " << endl;
	}
	float getX() { return pos[0]; }
	float getY() { return pos[1]; }
	float getZ() { return pos[2]; }
	float getR() { return rgb[0]; }
	float getG() { return rgb[1]; }
	float getB() { return rgb[2]; }
};
//边表和边
class edge
{
public:
	Point points[2];

	float dx, dr, dg, db, dz;
	float ymax, ymin;
	float xmax, xmin;
	float currentx, currentr, currentg, currentb, currentz;
	int lower;
public:
	edge(float* v1, float *v2)
	{
		
		points[0].init(v1, v1 + 9);
		points[1].init(v2, v2 + 9);
		lower = points[0].getY() < points[1].getY() ? 0 : 1;
		if (points[0].getY() == points[1].getY()) 
		{ 
			lower = -1; 
			return;
		}
		ymax = points[0].getY() > points[1].getY() ? points[0].getY() : points[1].getY();
		ymin = points[0].getY() < points[1].getY() ? points[0].getY() : points[1].getY();
		xmax = points[0].getX() > points[1].getX() ? points[0].getX() : points[1].getX();
		xmin = points[0].getX() < points[1].getX() ? points[0].getX() : points[1].getX();
		if (ymin < Y_MIN) Y_MIN = ymin;
		if (ymax > Y_MAX) Y_MAX = ymax;
		dx = ((float)points[1].getX() - (float)points[0].getX()) / ((float)points[1].getY() - (float)points[0].getY());
		dr = (points[1].getR() - points[0].getR()) / (points[1].getY() - points[0].getY());
		dg = (points[1].getG() - points[0].getG()) / (points[1].getY() - points[0].getY());
		db = (points[1].getB() - points[0].getB()) / (points[1].getY() - points[0].getY());
		dz = (points[1].getZ() - points[0].getZ()) / (points[1].getY() - points[0].getY());
		currentx = points[lower].getX();
		currentr = points[lower].getR();
		currentg = points[lower].getG();
		currentb = points[lower].getB();
		currentz = points[lower].getZ();
		//cout << currentx << " " << currentr << " " << currentg << " " << currentb << endl;
	}
	int getLow()
	{
		if (lower == -1) return -1;
		return points[lower].getY();
	}
};
void setxrgbz(float& x, float& r, float& g, float& b, float& z, edge e)
{
	x = e.currentx;
	r = e.currentr;
	g = e.currentg;
	b = e.currentb;
	z = e.currentz;
}


void draw()
{
	/*
	float** depth;
	depth = new float* [WIDTH];
	for (int i = 0; i < WIDTH; ++i) depth[i] = new float[HEIGHT];
	*/
	//操作depth
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j = 0; j < HEIGHT; ++j)
		{
			depth[i][j] = z;
			setImage(i, j, 0, 0, 0);
		}
	}
	
	for (int all = 0; all < 2; ++all)
	{
		//cout << "\t" << all << endl;
		vector<edge> edges;
		vector<edge> now_e;
		edges.push_back(edge(vertices + all * 18, vertices + 3 + all * 18));
		edges.push_back(edge(vertices + all * 18, vertices + 6 + all * 18));
		edges.push_back(edge(vertices + 3 + all * 18, vertices + 6 + all * 18));
		for (int y = Y_MIN; y <= Y_MAX; ++y)
		{
			//去掉活动边表多余的
			for (vector<edge>::iterator i = now_e.begin(); i != now_e.end();)
			{
				if (y >= i->ymax) i = now_e.erase(i);
				else i++;
			}
			//cout << 1 << endl;
			//从所有边表中选出扫描到的放到活动边表里
			for (vector<edge>::iterator i = edges.begin(); i != edges.end();)
			{
				if (i->getLow() == -1)
				{
					i = edges.erase(i);
				}
				else if (y >= i->getLow())
				{
					edge ttmp = *i;
					now_e.push_back(ttmp);
					i = edges.erase(i);
				}
				else i++;
			}
			//cout << 2 << endl;
			//开始遍历活动边表
			if (now_e.size() < 2) continue;
				vector<edge>::iterator i = now_e.begin();
				vector<edge>::iterator tmp = i + 1;
				vector<edge>::iterator i1, i2;
				//cout << 3 << endl;
				
				//cout << 5 << endl;
				
				if (tmp == now_e.end())
				{

					cout << "wrong tmpp" << endl;
					exit(-1);
				}
				i1 = i->currentx < tmp->currentx ? i : tmp;
				i2 = i->currentx > tmp->currentx ? i : tmp;
				float x1, x2, z1, z2, r1, r2, g1, g2, b1, b2;
				//cout << i->currentx << " " << i->dx << endl;
				//左侧的点
				i1->currentx += i1->dx;
				i1->currentr += i1->dr;
				i1->currentg += i1->dg;
				i1->currentb += i1->db;
				i1->currentz += i1->dz;
				setxrgbz(x1, r1, g1, b1, z1, *i1);

				//cout << i->currentx << " " << i->dx << endl;
				//右侧的点
				i2->currentx += i2->dx;
				i2->currentr += i2->dr;
				i2->currentg += i2->dg;
				i2->currentb += i2->db;
				i2->currentz += i2->dz;
				setxrgbz(x2, r2, g2, b2, z2, *i2);
				//cout << 4 << endl;
				//画三角形
				for (float i = x1; i < x2; i += 1)
				{
					if (((i - x1) * z2) / (x2 - x1) + ((x2 - i) * z1) / (x2 - x1) < depth[(int)i][y]) continue;
					depth[(int)i][y] = ((i - x1) * z2) / (x2 - x1) + ((x2 - i) * z1) / (x2 - x1);
					setImage(
						i + WIDTH / 2,
						y + HEIGHT / 2,
						(((i - x1) * r2) / (x2 - x1) + ((x2 - i) * r1) / (x2 - x1)),
						(((i - x1) * g2) / (x2 - x1) + ((x2 - i) * g1) / (x2 - x1)),
						(((i - x1) * b2) / (x2 - x1) + ((x2 - i) * b1) / (x2 - x1))
					);
				}
			
		}
	}
}

int main(int argc, char** argv)
{
	//handle file
	//ifstream in("./overlapping.tri");
	ifstream in("./intersecting.tri");
	if (!in) cout << "打开失败" << endl;
	for (int i = 0; i < 36; ++i) in >> vertices[i];
	for (int i = 2; i < 9; i += 3)
	{
		if (vertices[i] > 0)
		{
			vertices[i - 2] -= 10;
			vertices[i - 1] += 5;
		}
	}
	for (int i = 20; i < 27; i += 3)
	{
		if (vertices[i] > 0)
		{
			vertices[i - 2] -= 10;
			vertices[i - 1] += 5;
		}
	}
	//for (int i = 0; i < 72; ++i) if (vertices[i] < -1 || vertices[i] > 1) vertices[i] /= 20;
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