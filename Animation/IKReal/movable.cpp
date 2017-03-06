#ifdef WIN32													
#include <windows.h>											
#endif

#include <stdio.h>
#include <string>

#include <GL/gl.h>												

#include "operateur.hpp"
#include "movable.hpp"

using namespace std;

movable::movable()
{
	init();
}

movable::~movable()
{
}

void movable::init()
{
	location.LoadIdentity();
	location.matrix[10] = -1.0f;  
}

movable& movable::operator= (const movable& other)
{
	location = other.location;
	return *this;
}

void movable::draw()
{
}

void movable::update()
{
}

void movable::move(int pitch, int turn, int roll, float x, float y, float z)
{
	if (pitch != 0)
		location.RotateX(-pitch/ROTATE_SLOWNESS);

	if (turn != 0)
		location.RotateY(-turn/ROTATE_SLOWNESS);

	if (roll != 0)
		location.RotateZ(roll/ROTATE_SLOWNESS);

	location.Translate(x,y,z);

	newLocation = location;
}

