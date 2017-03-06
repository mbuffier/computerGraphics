#ifndef BODY_HPP
#define BODY_HPP

#include <stack>

#include "movable.hpp"
#include "bvh.hpp"
#include "light.hpp"

class body : public movable
{
	//Recursive function to draw the body part by part
	void recursDraw(bvhPart *part);
	
	stack<matrix16f> orient;

public:
	body(string bvhFile);
	~body();

	// Position in the animation
	unsigned counter;

	bvh *theBvh;
	
	void draw();
	void update();
	
	vector<light*> lights;
};

#endif //BODY_HPP
