#ifndef movable_H
#define movable_H

#include <string>

#include "operateur.hpp"

#define ROTATE_SLOWNESS 8
#define MOVE_INC  1.4f

using namespace std;

class movable
{
public:
	
	movable();
	virtual ~movable();

	void init();

	movable& operator= (const movable& other);

	// Function for every movable
	virtual void draw();
	virtual void update();
	void move(int pitch, int turn, int roll, float x, float y, float z);

	// State variables
	matrix16f location, newLocation;
};

#endif // movable_H
