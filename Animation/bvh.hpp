#ifndef BVH_HPP
#define BVH_HPP

#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

#include "operateur.hpp"

using namespace std;

// Every bvh is composed with several bvhPart
class bvhPart 
{
	
public: 
	enum channelTypes {Xpos,Ypos,Zpos,Zrot,Xrot,Yrot};
	
	bvhPart();
	string name;
	vector3f offset ;
	// A matrix to describe the movement of the part
	vector<matrix16f> motion ;
	
	bvhPart* parent ;
	vector<channelTypes> channels ;
	//Children from this part
	vector<bvhPart*> child ;
};

class bvh 
{
	enum mode {NONE,OFFSET,CHANNELS,JOINT,ROOT,End,Site,MOTION,Frames,Frame,Time,MOTIONDATA};

public:
	bvhPart *root;
	float frameTime;
	int framesNum;

	bvh(string bvhFile);
	void recurs(bvhPart* some);
	void process(string line);
	void init(string bvhFile);

private:

	bvhPart *current ;
	// Here are the different part of the body
	vector <bvhPart*> bvhPartsLinear;
	
      mode theMode;
	int vertIndex;
	unsigned channelIndex;
	unsigned partIndex;
	int data;
	unsigned channelsNum;	
	
	matrix16f tempMotion;
	matrix16f tempMotionY;
	matrix16f tempMotionX;
	matrix16f tempMotionZ;
 
};

#endif //BVH_HPP
