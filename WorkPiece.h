#include <stdlib.h>  
#include <GL/glut.h>
#include <map>
#include <cmath>
#include <iostream>
#include "particles.h"

using namespace std;

class WorkPiece
{
private:
	const GLfloat cylinderLength = 3;
	const GLfloat cylinderRad = 1;
	const GLfloat step = 2;
	const GLfloat pi = 3.1415926;
	const int WindowWidth = 800;
	const int WindowHeight = 600;
	particles part;

	void inline put(float x1, float y1)
	{
	}
	struct position
	{
		float x;
		float y;
		position() {}
		position(float xx, float yy) { x = xx; y = yy; }
	};
	struct info
	{
		position p;
		bool isBethel;
		position ps[4];
		info* next;
		info(position pp)
		{
			p = pp;
			isBethel = false;
			next = NULL;
		}
		info(position pss[])
		{
			p = pss[0];
			for (int i = 0; i < 4; ++i) ps[i] = pss[i];
			isBethel = true;
			next = NULL;
		}
	};

	info* head;
	info* tail;
	position startPosition, endPosition;

	info* find(float x);

public:
	unsigned int texture;
	WorkPiece()
	{
		startPosition.x = -cylinderLength / 2;
		startPosition.y = cylinderRad;
		endPosition.x = cylinderLength / 2;
		endPosition.y = cylinderRad;

		position p(startPosition.x, startPosition.y);
		head = new info(p);
		position o(endPosition.x, endPosition.y);
		tail = new info(o);
		head->next = tail;
	}

	void setBethel(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);
	void setPoint(float x, float y);
	void draw(int offset);
};
/*绘制Bethel曲线要加入两个点*/
void WorkPiece::setBethel(float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4)
{
	position ps[4];
	ps[0].x = x1;
	ps[0].y = y1;
	ps[1].x = x2;
	ps[1].y = y2;
	ps[2].x = x3;
	ps[2].y = y3;
	ps[3].x = x4;
	ps[3].y = y4;
	info* bethelStart = new info(ps);
}

void WorkPiece::setPoint(float ix, float iy)
{
	float mx = ix;
	float my = -iy;
	mx -= WindowWidth / 2;
	my += WindowHeight / 2;
	mx /= 100;
	my /= 100;
	//cout << "呼叫塔台" << mx << " " << my << endl;
	//check
	if (mx <= -cylinderLength / 2 || mx >= cylinderLength / 2 || my >= cylinderRad || my <= 0) return;

	info* dst = find(mx);
	if (dst->p.x == mx) {
		if(dst->p.y > my)
			dst->p.y = my;
		return;
	}
	else if (dst->next->p.x == mx)
	{
		if(dst->next->p.y > my)
			dst->next->p.y = my;
		return;
	}
	else
	{
		if (dst->next->p.y > my || dst->p.y > my)
		{
			part.set(mx, my);
			//
			/*position p0(mx - 0.2, (dst->next->p.y - dst->p.y) / (dst->next->p.x - dst->p.x) * (mx - dst->p.x) + dst->p.y);
			info* tmp0 = new info(p0);*/
			//一个点
			position p(mx, my);
			info* tmp = new info(p);
			//第二个点
			//position p1(mx + 0.05, (dst->next->p.y - dst->p.y) / (dst->next->p.x - dst->p.x) * (mx - dst->p.x) + dst->p.y);
			//info* tmp1 = new info(p1);
			/*tmp->next = tmp1;
			tmp0->next = tmp;
			tmp1->next = dst->next;
			dst->next = tmp0;*/
			tmp->next = dst->next;
			dst->next = tmp;
		}
		else return;
	}

}

void WorkPiece::draw(int offset)
{

	glBindTexture(GL_TEXTURE_2D, texture);
	glEnable(GL_TEXTURE_2D);
	info* it = head;
	//头
	glLineWidth(3.5);
	//画横截面的
	glBegin(GL_LINES);
	for (float i = 0; i <= 360; i += step)
	{
		float tmpSin = sin((i * pi) / 180);
		float tmpCos = cos((i * pi) / 180);
		//glNormal3f(1, 0, 0);
		glVertex3f(it->next->p.x, cylinderRad * tmpSin, cylinderRad * tmpCos);
		glVertex3f(it->next->p.x, -cylinderRad * tmpSin, cylinderRad * tmpCos);
	}
	for (float rote = offset; rote <= offset + 360; rote += step)
	{
		float tmpSin = sin((rote * pi) / 180);
		float tmpCos = cos((rote * pi) / 180);
		float tmpSinTex = sin((rote - offset) / 180 * pi);
		float texRate1 = (it->p.x + cylinderLength / 2) / cylinderLength;
		float texRate2 = (it->next->p.x + cylinderLength / 2) / cylinderLength;
		//glNormal3f(0, tmpSin, tmpCos);
		glTexCoord2d(texRate1, tmpSinTex);
		glVertex3f(it->p.x, it->p.y * tmpSin, it->p.y * tmpCos);
		glTexCoord2d(texRate2, tmpSinTex);
		glVertex3f(it->next->p.x, it->p.y * tmpSin, it->p.y * tmpCos);

	}
	glEnd();
	for (; it->next != tail; it = it->next)
	{
		//初始判断
		float endX;
		float endY;
		info* tmp = it->next;
		endX = tmp->p.x;
		endY = tmp->p.y;
		//普通绘制
		
		glLineWidth(3.5);
		glBegin(GL_LINES);
		for (float rote = offset; rote <= offset + 360; rote += step)
		{
			float tmpSin = sin((rote * pi) / 180);
			float tmpCos = cos((rote * pi) / 180);
			float tmpSinTex = sin((rote - offset) / 180 * pi);
			float texRate1 = (it->p.x + cylinderLength / 2) / cylinderLength;
			float texRate2 = (endX + cylinderLength / 2) / cylinderLength;
			glNormal3f(0, tmpSin, tmpCos);
			if (it->p.y >= cylinderRad - 0.05)
			{
				glTexCoord2d(texRate1, tmpSinTex);
				glVertex3f(it->p.x, it->p.y * tmpSin, it->p.y * tmpCos);
				glTexCoord2d(texRate2, tmpSinTex);
				glVertex3f(endX, it->p.y * tmpSin, it->p.y * tmpCos);
			}
			else
			{
				glVertex3f(it->p.x, it->p.y * tmpSin, it->p.y * tmpCos);
				glVertex3f(endX, it->p.y * tmpSin, it->p.y * tmpCos);
			}
		}
		glEnd();
	}
	glBegin(GL_LINES);
	//画横截面的
	for (float i = 0; i <= 360; i += step)
	{
		float tmpSin = sin((i * pi) / 180);
		float tmpCos = cos((i * pi) / 180);
		//glNormal3f(-1, 0, 0);
		glVertex3f(it->p.x, cylinderRad * tmpSin, cylinderRad * tmpCos);
		glVertex3f(it->p.x, -cylinderRad * tmpSin, cylinderRad * tmpCos);
	}
	for (float rote = offset; rote <= offset + 360; rote += step)
	{
		float tmpSin = sin((rote * pi) / 180);
		float tmpCos = cos((rote * pi) / 180);
		float tmpSinTex = sin((rote - offset) / 180 * pi);
		float texRate1 = (it->p.x + cylinderLength / 2) / cylinderLength;
		float texRate2 = (it->next->p.x + cylinderLength / 2) / cylinderLength;
		glNormal3f(0, tmpSin, tmpCos);
		glTexCoord2d(texRate1, tmpSinTex);
		glVertex3f(it->p.x, cylinderRad * tmpSin, cylinderRad * tmpCos);
		glTexCoord2d(texRate2, tmpSinTex);
		glVertex3f(it->next->p.x, cylinderRad * tmpSin, cylinderRad * tmpCos);

	}
	glEnd();
	part.draw();
}

WorkPiece::info* WorkPiece::find(float x)
{
	info* tmp = head;
	for (; tmp->next != NULL; tmp = tmp->next)
	{
		if (tmp->p.x <= x && tmp->next->p.x >= x)
			break;
	}
	return tmp;
}