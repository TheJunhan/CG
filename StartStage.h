#ifndef START_STAGE_H
#define START_STAGE_H
#include <GL/glut.h>  
#pragma comment(lib, "glut32.lib")  

class startStage
{
private:
	int WindowWidth;
	int WindowHeight;

	float left;
	float right;
	float low;
	float high;

	float tieLow;
	float tieHigh;
	float tieLeft;
	float tieRight;

	float woodLow;
	float woodHigh;
	float woodLeft;
	float woodRight;
public:
	startStage()
	{
		WindowWidth = 800;
		WindowHeight = 600;

		left = -3;
		right = 3;
		low = 0;
		high = 2;

		tieLow = -2;
		tieHigh = -0.5;
		tieLeft = -2.5;
		tieRight = -0.5;

		woodLow = -2;
		woodHigh = -0.5;
		woodLeft = 0.5;
		woodRight = 2.5;
	}
	void showStartStage(unsigned int head, unsigned int tie, unsigned int wood);
	/*如果是铁则返回-1，木头返回1，啥也没点着返回0*/
	int judgeWhere(int x, int y);
};

void startStage::showStartStage(unsigned int head, unsigned int tie, unsigned int wood)
{
	//标题
	glBindTexture(GL_TEXTURE_2D, head);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_LINES);
	for (float i = low; i < high; i += 0.01)
	{
		float tmp = high - low;
		glTexCoord2f(0, (i - low) / tmp);
		glVertex3f(left, i, 0);
		glTexCoord2f(1, (i - low) / tmp);
		glVertex3f(right, i, 0);
	}
	glEnd();
	//铁质纹理
	glBindTexture(GL_TEXTURE_2D, tie);
	glBegin(GL_LINES);
	for (float i = tieLow; i < tieHigh; i += 0.01)
	{
		float tmp = tieHigh - tieLow;
		glTexCoord2f(0, (i - tieLow) / tmp);
		glVertex3f(tieLeft, i, 0);
		glTexCoord2f(1, (i - tieLow) / tmp);
		glVertex3f(tieRight, i, 0);
	}
	glEnd();
	//木质纹理
	glBindTexture(GL_TEXTURE_2D, wood);
	glBegin(GL_LINES);
	for (float i = woodLow; i < woodHigh; i += 0.01)
	{
		float tmp = woodHigh - woodLow;
		glTexCoord2f(0, (i - woodLow) / tmp);
		glVertex3f(woodLeft, i, 0);
		glTexCoord2f(1, (i - woodLow) / tmp);
		glVertex3f(woodRight, i, 0);
	}
	glEnd();

}

int startStage::judgeWhere(int ix, int iy)
{
	float x = ix;
	float y = -iy;
	x -= WindowWidth / 2;
	y += WindowHeight / 2;
	x /= 100;
	y /= 100;

	if (x >= tieLeft && x <= tieRight && y >= tieLow && y <= tieHigh) return -1;
	else if (x >= woodLeft && x <= woodRight && y >= woodLow && y <= woodRight) return 1;
	else return 0;
}
#endif