//Classe qui definit une intersection 
#ifndef _INTERSECTION_H
#define _INTERSECTION_H

#include "Vec3.h"


class Intersection {

public:
    Vec3f ptIntersection;
    Vec3f normal;
    Vec3f diffuse ;
    float shininess;
    Vec3f specular ;
    Vec3f ambient;
    float ior; //indice of refraction
    bool dansOmbre; //point dans l'ombre ou non 
    bool intersect; //point est une intersection de la scene ou non 

    Intersection (Vec3f ptIntersection, Vec3f normal, Vec3f diffuse, float shininess,Vec3f specular, Vec3f ambient, float ior, bool dansOmbre = false, bool intersect = false)
      : ptIntersection(ptIntersection), normal(normal), diffuse(diffuse), shininess(shininess), specular(specular), ambient(ambient), ior(ior), dansOmbre(dansOmbre), intersect(intersect){}

};

#endif
