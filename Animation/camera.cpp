#ifdef WIN32									
#include <windows.h>								
#endif										
#include <iostream>

#include <GL/gl.h>	
#include <GL/glu.h>	

#include "operateur.hpp"
#include "movable.hpp"
#include "camera.hpp"

#include <cmath>
#ifdef WIN32
#define M_PI PI
#endif

camera::camera()
{
	init();
}

camera::~camera()
{
}

void camera::init()
{
	radius = 15.0f;
}

// Definition of the look of the camera using location
void camera::look()
{
	
	gluLookAt( 
				//eye
				location.matrix[12],
				location.matrix[13],
				location.matrix[14], 

				// center
				location.matrix[12]+location.matrix[8]+0.3,
				location.matrix[13]+location.matrix[9]+0.2,
				location.matrix[14]+location.matrix[10], 

				// up
				location.matrix[4],
				location.matrix[5],
				location.matrix[6]);

	return;	
}
