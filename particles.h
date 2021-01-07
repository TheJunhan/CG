#pragma once
#include <ctime>
#include <random>
#include <gl/glut.h>

using namespace std;
const int max_size = 200;

class particles
{
private:
    struct parties//structrue for particle
    {
        bool active;//active or not
        unsigned long long born;//last time
        float x, y, z;//the position
        float xi, yi, zi;//what direction to move
        //float xg, yg, zg;//gravity
        parties()
        {
            active = false;
        }
    };
    parties p[max_size];
    float g = 0.05;
    const float lifetime = 20;
    const float size = 0.02;
    unsigned long long current_version = 0;
public:
    void set(float x, float y)
    {
        int j = 0;
        srand(time(0));
        for (int i = 0; i < max_size; ++i)
        {
            if (!p[i].active)
            {
                p[i].active = true;
                p[i].born = lifetime;
                p[i].x = x;
                p[i].y = y;
                p[i].z = 0;
                int tmp = rand() % 3;
                if(tmp) p[i].xi = -(float)(rand() % 10) / 100;
                else p[i].xi = (float)(rand() % 10) / 100;
                p[i].yi = (float)(rand() % 10) / 100 + 0.1;
                p[i].zi = (float)(rand() % 10) / 100;
                //cout << p[i].xi << " " << p[i].yi << " " << p[i].zi << endl;
                j++;
            }
            if (j >= 5) break;
        }
    }
    void draw()
    {
        for (int i = 0; i < max_size; ++i)
        {
            if (p[i].active)
            {
                glBegin(GL_TRIANGLE_STRIP);
                //top right
                glTexCoord2d(1, 1);
                glVertex3f(p[i].x + size, p[i].y + size, p[i].z);
                //top left
                glTexCoord2d(0, 1);
                glVertex3f(p[i].x - size, p[i].y + size, p[i].z);
                //bottom right
                glTexCoord2d(1, 0);
                glVertex3f(p[i].x + size, p[i].y - size, p[i].z);
                //bottom left
                glTexCoord2d(0, 0);
                glVertex3f(p[i].x - size, p[i].y - size, p[i].z);
                glEnd();
                if (0 >= p[i].born)
                    p[i].active = false;
                else
                {
                    p[i].x += p[i].xi;
                    p[i].y += p[i].yi;
                    p[i].z += p[i].zi;
                    p[i].yi -= g;
                    p[i].born--;
                }
            }
        }
    }
};