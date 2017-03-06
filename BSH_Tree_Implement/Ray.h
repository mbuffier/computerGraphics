#include <cmath>
#include "Vec3.h"

class Ray {
    
public:
    Vec3f origin;
    Vec3f direction;
};

// test the intersection between a ray and a triangle
bool intersect(Vec3f orig , Vec3f direc, Vec3f p0 , Vec3f p1,  Vec3f p2) {
    
    Vec3f e0 = p1 - p0 ;
    direc.normalize() ;
    
    Vec3f e1 = p2 - p0 ;
    Vec3f normal = cross(e0,e1) ;
    normal.normalize() ;
    
    Vec3f q = cross( direc, e1) ;
    float a = dot(e0,q) ;
    
    if ( (dot(normal,direc)>=0) ) {
        return false ;
    }
    
    Vec3f s = (orig - p0)/a ;
    Vec3f r = cross(s,e0) ;
    float b0 = dot(s,q) ;
    float b1 = dot(r,direc) ;
    float b2 = 1.0-b0-b1 ;
    
    if ( (b0 < 0.0) | (b1<0.0) | (b2 < 0.0)) {
        return false ;
    }
    
    float t = dot(e1,r) ;
    if ( t>=0 )
        return true ;
    
    return false ;
}