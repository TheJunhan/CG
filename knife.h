#pragma once
#include <gl/glut.h>
#include <iostream>

using namespace std;

class Knife
{
private:
	float x;
	float y;
	float triedge = 0.5;
	const float longedge = 1.5;
	const float shortedge = 0.3;
	const int WindowWidth = 800;
	const int WindowHeight = 600;
	unsigned int texture;
public:
	Knife()
	{
		x = -3.5;
		y = 2.5;
	}
	void setTexture(unsigned int t)
	{
		texture = t;
	}
	void draw()
	{
		glBindTexture(GL_TEXTURE_2D, texture);
		//glBegin(GL_TRIANGLE_STRIP);
		glBegin(GL_QUADS);
		glNormal3f(0, 0, 1);
		glTexCoord2f(0.286, 1 - 0.063);
		glVertex3f(x, y, 0);
		glTexCoord2f(0.532, 1 - 0.063);
		glVertex3f(x + shortedge, y, 0);
		glTexCoord2f(0.532, 1 - 0.868);
		glVertex3f(x + shortedge, y - longedge, 0);
		glTexCoord2f(0.286, 1 - 0.868);
		glVertex3f(x, y - longedge, 0);
		glEnd();
		glBegin(GL_TRIANGLES);
		glTexCoord2f(0.28, 1 - 0.868);
		glVertex3f(x, y - longedge, 0);
		glTexCoord2f(0.54, 1 - 0.868);
		glVertex3f(x + shortedge, y - longedge, 0);
		glTexCoord2f(0.66, 1 - 0.957);
		glVertex3f(x + shortedge, y - longedge - triedge, 0);
		glEnd();
	}
	bool judge(int ix, int iy)
	{
		float jx = ix;
		float jy = -iy;
		jx -= WindowWidth / 2;
		jy += WindowHeight / 2;
		jx /= 100;
		jy /= 100;
		float wid = 0.5;
		//cout << "?:" << jx << " " << jy << endl;
		if ((jx <= x + shortedge + wid && jx >= x + shortedge - wid) && (jy <= y - longedge - triedge + wid && jy >= y - longedge - triedge - wid))
			return true;
		return false;
	}
	void move(int ix, int iy)
	{
		float mx = ix;
		float my = -iy;
		mx -= WindowWidth / 2;
		my += WindowHeight / 2;
		mx /= 100;
		my /= 100;
		if (my < 0) return;
		x = mx - shortedge;
		y = my + longedge + triedge;
	}
};