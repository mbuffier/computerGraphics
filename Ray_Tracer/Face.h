//methode qui cree une face carree a partir de 4 points
#ifndef _FACE_H
#define _FACE_H

#include <cmath>
#include <vector>
#include "Vec3.h"
#include "tiny_obj_loader.h"

using namespace std;

class Face {

public :
Vec3f sommet0;
Vec3f sommet1;
Vec3f sommet2;
Vec3f sommet3;

Face() {} 
~Face() {}

Face(Vec3f sommet0,Vec3f sommet1,Vec3f sommet2,Vec3f sommet3) :sommet0(sommet0), sommet1(sommet1), sommet2(sommet2),sommet3(sommet3) {}

} ;

#endif
