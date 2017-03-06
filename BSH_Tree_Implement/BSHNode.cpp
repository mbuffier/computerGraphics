#include "BSHNode.h"

void BSHNode::boundingSphere() {
    //find the center and the normal
    Vec3f center = Vec3f(0.f, 0.f, 0.f) ;
    Vec3f norm = Vec3f(0.f, 0.f, 0.f) ;
    unsigned long vSize = vertexLeaf.size() ;
    for (unsigned int i = 0; i < vSize; i++) {
        const Vec3f &norV = vertexLeaf[i].n ;
        const Vec3f &cenV = vertexLeaf[i].p ;
        
        center += cenV ;
        norm += norV ;
    }
    center/= vSize;
    norm/= vSize;
    
    //find radius
    float dmax=0.0;
    float d;
    float rad = 0.0 ;
    
    for (int i=0;i < vSize ; i++) {
        d= dist(vertexLeaf[i].p,center);
        if (d>dmax) {
            dmax=d;
        }
        rad = dmax;
    }
    position = center ;
    normal = norm ;
    radius = rad ;
    color = Vec3f(1.f,1.f,1.f) ;
}

int halfSpace( const Vec3f & centre, const Vec3f &point) {
    Vec3f planeNormal = Vec3f(1.0,0.0,0.0) ;
    float d = centre[0]*planeNormal[0] + centre[1]*planeNormal[1] + centre[2]*planeNormal[2] ;
    
    float test = point[0]*planeNormal[0] + point[1]*planeNormal[1] + point[2]*planeNormal[2] - d ;
    
    if(test >=0) {
        // the point is to the right of the plane
        return 1 ;
    }
    else
        return 0 ; // the point is to the left of the plane
}


std::vector<Vertex> BSHNode::separationLeft() {
    
    unsigned long vextexLCount = 0 ;
    
    unsigned long vSize = vertexLeaf.size() ;
    std::vector<Vertex> leftVertex ;
    leftVertex.reserve(vSize) ;

    for (unsigned int k=0; k < vSize ; k++) {
        const Vec3f &v0 = vertexLeaf[k].p;

        int test0 = halfSpace(position, v0) ;
        
        if(test0 ==1) {
            const Vertex &v = vertexLeaf[k] ;
            leftVertex.push_back(v) ;
            vextexLCount += 1 ;
        }
    }
    leftVertex.resize(vextexLCount);
    return leftVertex ;
}

std::vector<Vertex> BSHNode::separationRight() {
    
    unsigned long vextexRCount = 0 ;
    
    unsigned long vSize = vertexLeaf.size() ;
    std::vector<Vertex> rightVertex ;
    rightVertex.reserve(vSize) ;

    for (unsigned int k=0; k < vSize ; k++) {
        const Vec3f &v0 = vertexLeaf[k].p;

        int test0 = halfSpace(position, v0) ;
        
        if(test0 ==0) {
            const Vertex &v = vertexLeaf[k] ;
            rightVertex.push_back(v) ;
            vextexRCount += 1 ;
        }
    }
    rightVertex.resize(vextexRCount);
    return rightVertex ;
}