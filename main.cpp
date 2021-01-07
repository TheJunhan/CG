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
/*刀具相关*/
Knife knife;
bool movingKnife = false;
/*贝塞尔相关*/
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
//初始化
void init(int argc, char** argv);
//渲染
void display(void);
//重渲染
void reshape(int w, int h);
//从文件加载纹理
GLuint loadTexture(const char* filepath);
//光照初始化
void setLight();
//工作台灯
void setLap();
//Bethel
void setBethel(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

void handleKeyboard(unsigned char key, int x, int y);
void handleMouseClick(int button, int state, int x, int y);
void handleMotionClick(int x, int y);


/*
* CG-Project
* 车床游戏
*/
int main(int argc, char** argv)
{
	init(argc, argv);
	glutKeyboardFunc(handleKeyboard);
	glutMouseFunc(handleMouseClick);
	glutMotionFunc(handleMotionClick);
	//loadTexture("./image/tie.jfif");
	MainHead = loadTexture("./image/欢迎.png");
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
* 函数实现
*/
void init(int argc, char** argv)
{
	//变量初始化
	speLight = false;
	//OpenGL初始化
	glutInit(&argc, argv);
	glClearColor(0, 0, 0, 1.0); //背景黑色  
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	glEnable(GL_POINT_SMOOTH);           //启用点反走样
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);  //深度测试
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GLUT_MULTISAMPLE);  //多重采用
	glShadeModel(GL_SMOOTH);    // 启用阴影平滑

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
	//开场动画
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
		//cout << "呼叫塔台" << BethelX << " " << BethelY << endl;

		//cout << "Behtel点：" << BethelX << " " << BethelY << endl;
		//f++;
		//cout << "看过来" << f << endl;
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
	//画工件
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
	//起始阶段选择材质，并绑定纹理
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
		cout << "贝塞尔曲线模式：请使用鼠标左键选取四个点作为参数" << endl;
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

	int texwidth, texheight, nrChannels;//nrChannels表示通道数，R/G/B/A，一共4个通道，有些图片只有3个，A即为alpha
	stbi_set_flip_vertically_on_load(true);   //解决图像翻转问题，不需要像SOIL库中片段着色器的position设置为-y
	unsigned char* image = stbi_load(filepath, &texwidth, &texheight, &nrChannels, STBI_rgb_alpha);//注意这里不是0
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glBindTexture(GL_TEXTURE_2D, texture);
	//GLboolean flag = glIsTexture(texture);

	//cout << "tex:" << texture << endl;
	if (image) {
		//cout << "succeed" << filepath << endl;
		//cout << texture << endl;
		if (nrChannels == 3)//rgb 适用于jpg图像
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);//后面一个是RGBA
		else if (nrChannels == 4)//rgba 适用于png图像
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texwidth, texheight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);//注意，两个都是RGBA

		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, texwidth, texheight, GL_RGBA, GL_UNSIGNED_BYTE, image);
	}
	//else
		//std::cout << "Failed to load texture" << std::endl;
	stbi_image_free(image);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// 设置过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	return texture;
}

void setLight()
{
	GLfloat sun_light_position[] = { 0, 0, 3.0f, 1.0f }; //光源的位置在世界坐标系圆心，齐次坐标形式
	GLfloat sun_light_ambient[] = { 0.8f, 0.8f, 0.8f, 1.0f }; //RGBA模式的环境光，为0
	GLfloat sun_light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f }; //RGBA模式的漫反射光，全白光
	GLfloat sun_light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };  //RGBA模式下的镜面光 ，全白光
	glLightfv(GL_LIGHT0, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, sun_light_specular);

	//开启灯光
}

void setLap()
{
	
	GLfloat sun_light_position[] = { 0.0f, 3, 2, 1.0f }; //光源的位置在世界坐标系圆心，齐次坐标形式
	GLfloat sun_light_ambient[] = { 0.4f, 0.4f, 0.4f, 1.0f }; //RGBA模式的环境光，为0
	GLfloat sun_light_diffuse[] = { 1.0f, 0.6f, 0.07f, 1.0f }; //RGBA模式的漫反射光，全白光
	GLfloat sun_light_specular[] = { 1.0f, 0.6f, 0.07f, 1.0f };  //RGBA模式下的镜面光 ，全白光
	glLightfv(GL_LIGHT1, GL_POSITION, sun_light_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, sun_light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, sun_light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, sun_light_specular);

	//glEnable(GL_LIGHTING);   //开关:使用光
	//glEnable(GL_LIGHT1);     //打开0#灯
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