// Classe qui definit un noeud d'un KdTree
#ifndef _KDNODE_H
#define _KDNODE_H

#include <cmath>
#include <vector>
#include "Vec3.h"
#include "BoundingBox.h"
#include "tiny_obj_loader.h"
int calculTailleTotalDePoints();
static int taille = calculTailleTotalDePoints();
static vector<tinyobj::shape_t> shapes;

using namespace std;

class KdNode {

public :

  BoundingBox boundingBox ; 
  KdNode * leftChild = NULL;
  KdNode *rightChild = NULL;
  bool intersection ;
  bool boolFeuille; // true si t'es une feuille
  
  vector<float> feuille ;

  KdNode() {feuille.resize(taille);}
  ~KdNode() {}

  KdNode(BoundingBox boundingBox,  bool intersection = false, bool boolFeuille=false) : boundingBox(boundingBox) {feuille.resize(taille);} 

} ;

//calcul le nombre total de points dans la scene
int calculTailleTotalDePoints(){
	unsigned int taille=0;
	for (unsigned int s = 0; s < shapes.size (); s++){
		for (unsigned int p = 0; p < shapes[s].mesh.positions.size () / 3; p++) {
			taille++;
		}
	}

	return taille;
}

#endif
