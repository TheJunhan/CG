#include <stdlib.h>  
#include <GL/glut.h>  
#include <iostream>
#include <cmath>
#include <Windows.h>
#pragma comment(lib, "glut32.lib")  
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STBI_WINDOWS_UTF8

#include "WorkPiece.h"
#include "startStage.h"
#include "Knife.h"

using namespace std;
int offset = 0;

/*
* tool variables
*/
float headx = 0;
float heady = 0;
GLfloat tmpX;
GLfloat tmpY;
bool HasChoosen = false;
unsigned int MainHead = 0;
unsigned int MainTie = 0;
unsigned int MainWood = 0;
unsigned int MainKnife = 0;
bool isStarting = true;
startStage s;
WorkPiece work;
bool speLight;
//ObjLoader o("./image/Lemonade_Can.obj");
/*�������*/
Knife knife;
bool movingKnife = false;
/*���������*/
float BethelX;
float BethelY;
float BethelAche = 0;
const float bethelstep = 0.01;
float bethelxs[4];
float bethelys[4];
int bethelhere;
bool waitingBethel = false;
bool drawingBethel = false;

/*
* fuctions
*/
//��ʼ��
void init(int argc, char** argv);
//��Ⱦ
void display(void);
//����Ⱦ
void reshape(int w, int h);
//���ļ���������
GLuint loadTexture(const char* filepath);
//���ճ�ʼ��
void setLight();
//����̨��
void setLap();
//Bethel
void setBethel(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

void handleKeyboard(unsigned char key, int x, int y);
void handleMouseClick(int button, int state, int x, int y);
void handleMotionClick(int x, int y);


/*
* CG-Project
* ������Ϸ
*/
int main(int argc, char** argv)
{
	init(argc, argv);
	glutKeyboardFunc(handleKeyboard);
	glutMouseFunc(handleMouseClick);
	glutMotionFunc(handleMotionClick);
	//loadTexture("./image/tie.jfif");
	MainHead = loadTexture("./image/��ӭ.png");
	MainTie = loadTexture("./image/tie.png");
	MainWood = loadTexture("./image/wood.jfif");
	MainKnife = loadTexture("./image/knife3.png");
	knife.setTexture(MainKnife);
	//cout << MainHead << " " << MainTie << " " << MainWood << endl;
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutMainLoop();
	return 0;
}

/*
* ����ʵ��
*/
void init(int argc, char** argv)
{
	//������ʼ��
	speLight = false;
	//OpenGL��ʼ��
	glutInit(&argc, argv);
	glClearColor(0, 0, 0, 1.0); //������ɫ  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glEnable(GL_POINT_SMOOTH);           //���õ㷴����
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);  //��Ȳ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GLUT_MULTISAMPLE);  //���ز���
	glShadeModel(GL_SMOOTH);    // ������Ӱƽ��

	setLight();
	//setLap();
	//o = ObjLoader("./image/Lemonade_Can.obj");
}
int f = 0;
void display(void)
{
	time_t t1 = time(0);
	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//o.Draw();
	//glutSwapBuffers();
	//return;
	//��������
	if (isStarting)
	{
		s.showStartStage(MainHead, MainTie, MainWood);
		glutSwapBuffers();
		return;
	}
	//bethel
	if (drawingBethel)
	{
		float cylinderRad = 1;
		//for (int i = 0; i < 4; ++i) cout << bethelys[i] << " ";
		//cout << endl;
		//cout << BethelAche << endl;
		BethelX = bethelxs[0] * pow((1 - BethelAche), 3) + 
			3 * bethelxs[1] * pow((1 - BethelAche), 2) * BethelAche + 
			3 * bethelxs[2] * (1 - BethelAche) * pow(BethelAche, 2) +
			bethelxs[3] * pow(BethelAche, 3);

		BethelY = bethelys[0] * pow((1 - BethelAche), 3) + 
			3 * bethelys[1] * pow((1 - BethelAche), 2) * BethelAche +
			3 * bethelys[2] * (1 - BethelAche) * pow(BethelAche, 2) +
			bethelys[3] * pow(BethelAche, 3);
		BethelAche += bethelstep;
		//cout << "������̨" << BethelX << " " << BethelY << endl;

		//cout << "Behtel�㣺" << BethelX << " " << BethelY << endl;
		//f++;
		//cout << "������" << f << endl;
		if (BethelAche >= 1)
		{
			drawingBethel = false;

		}
		knife.move(BethelX, BethelY);
		if(BethelY < 300 && BethelY > 200)
			work.setPoint(BethelX, BethelY);
	}
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);
	//������
	work.draw(offset);
	knife.draw();

	glLoadIdentity();
	gluLookAt(headx / 100, heady / 100, 5.0, headx / 100, heady / 100, 0.0, 0.0, 1.0, 0);
	offset++;
	time_t t2 = time(0);
	if (offset == 360) offset = 0;
	if (t2 - t1 < 20)
		Sleep(20 - t2 + t1);

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
	gluLookAt(0, 0, 5.0, 0, 0, 0.0, 0.0, 1.0, 0);
}

void handleMouseClick(int button, int state, int x, int y)
{
	//��ʼ�׶�ѡ����ʣ���������
	if (isStarting)
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			int tmp = s.judgeWhere(x, y);
			if (tmp == -1)
			{
				setLight();
				setLap();
				//glBindTexture(GL_TEXTURE_2D, MainTie);
				work.texture = MainTie;
				isStarting = false;
			}
			else if (tmp == 1)
			{
				setLight();
				setLap();
				work.texture = MainWood;
				//glBindTexture(GL_TEXTURE_2D, MainWood);
				isStarting = false;
			}
			return;
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && waitingBethel)
	{
		bethelxs[bethelhere] = x;
		bethelys[bethelhere] = y;
		bethelhere++;
		if (bethelhere == 4)
		{
			waitingBethel = false;
			bethelhere = 0;
			drawingBethel = true;
		}
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		if (knife.judge(x, y)) {
			movingKnife = true;
		}
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		movingKnife = false;
	}
}

void handleKeyboard(unsigned char key, int x, int y)
{
	if (key == 'b') {
		//setBethel(-100, 50, 0, 80, 30, 200, 100, 80);
		cout << "����������ģʽ����ʹ��������ѡȡ�ĸ�����Ϊ����" << endl;
		waitingBethel = true;
	}
	else if (key == 'o')
	{
		if (speLight)
		{
			//glDisable(GL_LIGHTING);
			//glDisable(GL_LIGHT0);
			glDisable(GL_LIGHT1);
			speLight = false;
		}
		else
		{
			//glEnable(GL_LIGHTING);
			//glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);
			speLight = true;
		}
	}
}

GLuint loadTexture(const char* filepath)
{
	GLuint texture = 0;
	glGenTextures(1, &texture);

	int texwidth, texheight, nrChannels;//nrChannels��ʾͨ������R/G/B/A��һ��4��ͨ������ЩͼƬֻ��3����A��Ϊalpha
	stbi_set_flip_vertically_on_load(true);   //���ͼ��ת���⣬����Ҫ��SOIL����Ƭ����ɫ����position����Ϊ-y
	unsigned char* image = stbi_load(filepath, &texwidth, &texheight, &nrChannels, STBI_rgb_alpha);//ע�����ﲻ��0
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
	//GLboolean flag = glIsTexture(texture);

	//cout << "tex:" << texture << endl;
	if (image) {
		//cout << "succeed" << filepath << endl;
		//cout << texture << endl;
		if (nrChannels == 3)//rgb ������jpgͼ��
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);//����һ����RGBA
		else if (nrChannels == 4)//rgba ������pngͼ��
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);//ע�⣬��������RGBA

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texwidth, texheight, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	//else
		//std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// ���ù���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

void setLight()
{
	GLfloat sun_light_position[] = { 0, 0, 3.0f, 1.0f }; //��Դ��λ������������ϵԲ�ģ����������ʽ
	GLfloat sun_light_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f }; //RGBAģʽ�Ļ����⣬Ϊ0
	GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBAģʽ��������⣬ȫ�׹�
	GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //RGBAģʽ�µľ���� ��ȫ�׹�
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

	//�����ƹ�
}

void setLap()
{
	
	GLfloat sun_light_position[] = { 0.0f, 3, 2, 1.0f }; //��Դ��λ������������ϵԲ�ģ����������ʽ
	GLfloat sun_light_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f }; //RGBAģʽ�Ļ����⣬Ϊ0
	GLfloat sun_light_diffuse[] = { 1.0f, 0.6f, 0.07f, 1.0f }; //RGBAģʽ��������⣬ȫ�׹�
	GLfloat sun_light_specular[] = { 1.0f, 0.6f, 0.07f, 1.0f };  //RGBAģʽ�µľ���� ��ȫ�׹�
	glLightfv(GL_LIGHT1, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, sun_light_specular);

	//glEnable(GL_LIGHTING);   //����:ʹ�ù�
	//glEnable(GL_LIGHT1);     //��0#��
}

void handleMotionClick(int x, int y)
{
	if (movingKnife)
	{
		knife.move(x, y);
		work.setPoint(x, y);
	}
}

void setBethel(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	const float step = 0.01;
	for (float i = 0; i < 1; i += step)
	{
		float tmpx = x1 * pow((1 - i), 3) + x2 * pow((1 - i), 2) * i + x3 * (1 - i) * pow(i, 2) + x4 * pow(i, 3);
		float tmpy = y1 * pow((1 - i), 3) + y2 * pow((1 - i), 2) * i + y3 * (1 - i) * pow(i, 2) + y4 * pow(i, 3);
		//cout << tmpx << " " << tmpy << endl;
		knife.move(tmpx, tmpy);
		work.setPoint(tmpx, tmpy);
	}
}