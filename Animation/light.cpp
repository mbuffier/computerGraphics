#ifdef WIN32																
#include <windows.h>													
#endif																		
														
#include <stdlib.h>															
	
#include <stdio.h>	
#include <iostream>

#include <GL/gl.h>												
#include <GL/glu.h>	

#include "operateur.hpp"
#include "movable.hpp"
#include "light.hpp"


light::light()
{
}

light::light(camera *viewer, int GL_LIGHTX)
{
	this->viewer = viewer;

	this->GL_LIGHTX = GL_LIGHTX;

	GLfloat pos[] = {location.matrix[12],location.matrix[13],location.matrix[14],1.0f};
	
	Ka[0]=0.3f; Ka[1]=0.3f; Ka[2]=0.3f; Ka[3]=1.0f; 
	Kd[0]=0.5f; Kd[1]=0.5f; Kd[2]=0.5f; Kd[3]=1.0f; 
	Ks[0]=0.3f; Ks[1]=0.3f; Ks[2]=0.3f; Ks[3]=1.0f; 
	
	glEnable(GL_LIGHTX);
		
	glLightfv(GL_LIGHTX, GL_POSITION,pos);
	glLightfv(GL_LIGHTX, GL_AMBIENT, Ka);	
	glLightfv(GL_LIGHTX, GL_DIFFUSE, Kd);
	glLightfv(GL_LIGHTX, GL_SPECULAR,Ks);
}


light::~light()
{
	glDisable(GL_LIGHTX);
}

void light::draw()
{
	glPushMatrix();

	glEnable(GL_LIGHTING);	
	GLfloat pos[] = {location.matrix[12],location.matrix[13],location.matrix[14],1.0f};

	glEnable(GL_LIGHTX);
		
	glLightfv(GL_LIGHTX, GL_POSITION,pos);
	glLightfv(GL_LIGHTX, GL_AMBIENT, Ka);	
	glLightfv(GL_LIGHTX, GL_DIFFUSE, Kd);
	glLightfv(GL_LIGHTX, GL_SPECULAR,Ks);

	glDisable(GL_LIGHTING); 

	glPopMatrix();
}

