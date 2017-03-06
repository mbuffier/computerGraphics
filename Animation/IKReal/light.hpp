#ifndef light_H
#define light_H

#include <GL/gl.h>

#include "camera.hpp"
#include "operateur.hpp"


class light:public movable
{
public:

	light();
	~light();

	light(camera *viewer,int number);

	void draw();

	camera *viewer;  

	GLfloat Ka[4];	// Ambient 
	GLfloat Kd[4];	// Diffuse
	GLfloat Ks[4];    // Specular

	int GL_LIGHTX;
};


#endif

