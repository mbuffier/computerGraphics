#ifdef WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

#include "body.hpp"
#include "main.hpp"

// Create the body using the BVH constructor
body::body(string bvhFile = "Take 006.bvh")
{
	counter = 1;
	theBvh = new bvh(bvhFile); 

	matrix16f ident;
	ident.LoadIdentity();
	orient.push(ident);	
}

body::~body()
{
}

// Replay the animation every time it's come to the end
void body::update()
{
	counter++; 
	if (counter >= theBvh->root->motion.size()) counter = 1;
}

// Draw the body using recursDraw
void body::draw()
{
	glPushMatrix();
	recursDraw(theBvh->root);
	glPopMatrix();
}

void body::recursDraw(bvhPart* part)
{
	// motion[0] holds the offset for every bvhPart, orient.top is the orientation of the parent
	matrix16f ident = orient.top()*part->motion[0];
	
	// Translate every part according to the counter and the position of the bvhPart
	ident.Translate( part->motion[counter].matrix[12], part->motion[counter].matrix[13], part->motion[counter].matrix[14]);
	ident.Rotate(part->motion[counter]); 	
		
	orient.push(ident);
	
	unsigned i = 0;
	for (i = 0; i < part->child.size(); i++) {
		// Give the offset of the child
		vector3f offset = part->child[i]->offset;
		
		if (part->child[i]->child.size() == 0) glColor3f(0.0f,0.0f,1.0f);
		else glColor3f(0.0f,1.0f,0.0f);

		glPushMatrix();
		glMultMatrixf(ident.matrix);	

		glBegin(GL_LINES);
		// The origin is the parent part, we draw a line from this point to the offset
		glVertex3f( 0,0,0 );
		glVertex3fv(offset.vertex);
		glEnd();
		glPopMatrix();
	
		if (part->child[i]->child.size() != 0) recursDraw(part->child[i]);
	}
	
	orient.pop();	
}
