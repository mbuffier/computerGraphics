#include "Mesh.h"
#include <stdio.h>
#include <stdlib.h>

// Definition of BSHNode
class BSHNode {
public:
    Vec3f position;
    Vec3f normal;
    Vec3f color;
    float radius;
    BSHNode * leftChild ;
    BSHNode * rightChild ;
    std::vector<Vertex> vertexLeaf ;
    
    BSHNode() {} ;

    void boundingSphere() ;
    std::vector<Vertex> separationLeft() ;
    std::vector<Vertex> separationRight() ;
    friend int halfSpace(const Vec3f & centre, const Vec3f &point) ;
};

