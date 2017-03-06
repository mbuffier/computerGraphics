//Classe qui prend en attribut une liste de points
#ifndef _POINTLIST_H
#define _POINTLIST_H

#include <cmath>
#include <vector>
#include "Vec3.h"
#include "tiny_obj_loader.h"
#include "KdNode.h"
#include "BoundingBox.h"

using namespace std;

class PointList {

public :
vector<float> pointList;

PointList() {} 
~PointList() {}

PointList(vector<float> liste) : pointList(liste) {}
PointList(int n) {pointList.resize(n);}
vector<float> TriSelection(unsigned int n);
vector<float> triListe ();
float findMedianSample(BoundingBox boundingBox);
vector<float> upperPartition( char axeMax, float val);
vector<float> lowerPartition( char axeMax, float val);
KdNode buildKdTree();
BoundingBox computeBoundingBox();
//vector<float> getListOfAllPoints();


} ;


//algorithme de TriSelection
vector<float> PointList::TriSelection(unsigned int n) {
	for(unsigned int i=0 ; i < n ; i++) {				//on parcourt le tableau
		unsigned int p = i ;
		for( unsigned int j = i ; j < n ; j++) {			//on cherche la position p du plus petit élément
			
			if((*this).pointList[j] < (*this).pointList[p] ) {
				p=j;
			}
			//resultat.nbComparaisons++;	//comptage des opérations
		}
		float c = (*this).pointList[i] ;						//on échange
		(*this).pointList[i] = (*this).pointList[p] ;					//les deux
		(*this).pointList[p] = c ;						//nombres
		//resultat.nbAffectations+=2;			//comptage des opérations
	}
	PointList listeRetour = PointList(n);
	
	for(unsigned int k = 0 ; k < n ; k++) {
		listeRetour.pointList[k]=(*this).pointList[k];
	}
	return listeRetour.pointList ;
}


// Recalcul une liste de tous les points dans l'ordre des triangles pour pouvoir utiliser le KdTree avec le triangle
PointList triListe (){
PointList pointListRetour=PointList(taille);
	for (unsigned int s = 0; s < shapes.size(); s++) {
		//on tourne sur les triangles
		for (unsigned int t = 0; t < shapes[s].mesh.indices.size() / 3; t++) {
			//pour chaque triangle
			//on obtient les float x,y,z des vertices du triangle via index, index+1, index+2
			unsigned int indexV1 = 3*shapes[s].mesh.indices[3*t];
			unsigned int indexV2 = 3*shapes[s].mesh.indices[3*t+1];
			unsigned int indexV3 = 3*shapes[s].mesh.indices[3*t+2];

			for (unsigned int i=0; i<3;i++){
				pointListRetour.pointList.push_back(shapes[s].mesh.positions[indexV1+i]);			

			}
			for (unsigned int i=0; i<3;i++){
				pointListRetour.pointList.push_back(shapes[s].mesh.positions[indexV2+i]);			

			}
			for (unsigned int i=0; i<3;i++){
				pointListRetour.pointList.push_back(shapes[s].mesh.positions[indexV3+i]);			

			}

		}
	}

return pointListRetour ;
}

//retourne la coordonnee du point median selon le grand axe de la boite
float PointList::findMedianSample(BoundingBox boundingBox) {
	char axeMax = boundingBox.maxAxis() ;
	int sizeList = floor(this->pointList.size()/3 );
	PointList listeATrier = PointList(sizeList) ;
	PointList listeTrier = PointList(sizeList);

	unsigned int i = 0 ;

	if (axeMax == 'x') {
		for(unsigned j = 0 ; j< this->pointList.size(); j+=3) {
			listeATrier.pointList[i] = (*this).pointList[j] ;
			i++ ;
		}
	}
	if (axeMax == 'y') {
		for(unsigned j = 1 ; j< this->pointList.size(); j+=3) {
			listeATrier.pointList[i] = (*this).pointList[j] ;
			i++ ;
		}
	}
	if (axeMax == 'z') {
		for(unsigned j = 2 ; j< this->pointList.size(); j+=3) {
			listeATrier.pointList[i] = (*this).pointList[j] ;
			i++ ;
		}
	}

	listeTrier.pointList = listeATrier.TriSelection( sizeList) ;

	int indiceMedian = floor(listeTrier.pointList.size()/2) +1 ;

	return listeTrier.pointList[indiceMedian] ;

}

//partie superieur selon le point median
vector<float> PointList::upperPartition( char axeMax, float val) {
int compteur =-1;
	vector<float> upperPartition ;
	upperPartition.resize((*this).pointList.size()) ;
	unsigned int indice = 0;
	if (axeMax == 'x') {
		for(unsigned j = 0 ; j<(*this).pointList.size(); j+=3) {
			compteur++;
			if((*this).pointList[j] > val ) {
//on teste la position du point
if(compteur%3==0){
				upperPartition[indice]=(*this).pointList[j];
				upperPartition[indice+1]=(*this).pointList[j+1];
				upperPartition[indice+2]=(*this).pointList[j+2];

				upperPartition[indice+3]=(*this).pointList[j+3];
				upperPartition[indice+4]=(*this).pointList[j+4];
				upperPartition[indice+5]=(*this).pointList[j+5];

				upperPartition[indice+6]=(*this).pointList[j+6];
				upperPartition[indice+7]=(*this).pointList[j+7];
				upperPartition[indice+8]=(*this).pointList[j+8];

				indice+=9;
				j+=6;
			}
if(compteur%3==1) {
				upperPartition[indice+3]=(*this).pointList[j];
				upperPartition[indice+4]=(*this).pointList[j+1];
				upperPartition[indice+5]=(*this).pointList[j+2];

				upperPartition[indice]=(*this).pointList[j-3];
				upperPartition[indice+1]=(*this).pointList[j-2];
				upperPartition[indice+2]=(*this).pointList[j-1];

				upperPartition[indice+6]=(*this).pointList[j+3];
				upperPartition[indice+7]=(*this).pointList[j+4];
				upperPartition[indice+8]=(*this).pointList[j+5];

				indice+=9;
				j+=3;	 }

if(compteur%3==2) {
				upperPartition[indice+6]=(*this).pointList[j];
				upperPartition[indice+7]=(*this).pointList[j+1];
				upperPartition[indice+8]=(*this).pointList[j+2];

				upperPartition[indice+3]=(*this).pointList[j-3];
				upperPartition[indice+4]=(*this).pointList[j-2];
				upperPartition[indice+5]=(*this).pointList[j-1];

				upperPartition[indice]=(*this).pointList[j-6];
				upperPartition[indice+1]=(*this).pointList[j-5];
				upperPartition[indice+2]=(*this).pointList[j-4];

				indice+=9; }

			}
		}
	}
	if (axeMax == 'y') {
		for(unsigned j = 1 ; j<(*this).pointList.size(); j+=3) {
			compteur++;
			if((*this).pointList[j] > val ) {
//on teste la position du point
if(compteur%3==0) {
				upperPartition[indice]=(*this).pointList[j-1];
				upperPartition[indice+1]=(*this).pointList[j];
				upperPartition[indice+2]=(*this).pointList[j+1];

				upperPartition[indice+3]=(*this).pointList[j+2];
				upperPartition[indice+4]=(*this).pointList[j+3];
				upperPartition[indice+5]=(*this).pointList[j+4];

				upperPartition[indice+6]=(*this).pointList[j+5];
				upperPartition[indice+7]=(*this).pointList[j+6];
				upperPartition[indice+8]=(*this).pointList[j+7];

				indice+=9;
				j+=6; }
if(compteur%3==1) {
				upperPartition[indice+3]=(*this).pointList[j-1];
				upperPartition[indice+4]=(*this).pointList[j];
				upperPartition[indice+5]=(*this).pointList[j+1];

				upperPartition[indice]=(*this).pointList[j-4];
				upperPartition[indice+1]=(*this).pointList[j-3];
				upperPartition[indice+2]=(*this).pointList[j-2];

				upperPartition[indice+6]=(*this).pointList[j+2];
				upperPartition[indice+7]=(*this).pointList[j+3];
				upperPartition[indice+8]=(*this).pointList[j+4];

				indice+=9;
				j+=3; }

if(compteur%3==2) {
				upperPartition[indice+6]=(*this).pointList[j-1];
				upperPartition[indice+7]=(*this).pointList[j];
				upperPartition[indice+8]=(*this).pointList[j+1];

				upperPartition[indice+3]=(*this).pointList[j-4];
				upperPartition[indice+4]=(*this).pointList[j-3];
				upperPartition[indice+5]=(*this).pointList[j-2];

				upperPartition[indice]=(*this).pointList[j-7];
				upperPartition[indice+1]=(*this).pointList[j-6];
				upperPartition[indice+2]=(*this).pointList[j-5];

				indice+=9; }

			}
		}
	}
if (axeMax == 'z') {
		for(unsigned j = 2 ; j<(*this).pointList.size(); j+=3) {
			compteur++;
			if((*this).pointList[j] > val ) {
//on teste la position du point
if(compteur%3==0) {
				upperPartition[indice]=(*this).pointList[j-2];
				upperPartition[indice+1]=(*this).pointList[j-1];
				upperPartition[indice+2]=(*this).pointList[j];

				upperPartition[indice+3]=(*this).pointList[j+1];
				upperPartition[indice+4]=(*this).pointList[j+2];
				upperPartition[indice+5]=(*this).pointList[j+3];

				upperPartition[indice+6]=(*this).pointList[j+4];
				upperPartition[indice+7]=(*this).pointList[j+5];
				upperPartition[indice+8]=(*this).pointList[j+6];

				indice+=9;
				j+=6; }
if(compteur%3==1) {
				upperPartition[indice+3]=(*this).pointList[j-2];
				upperPartition[indice+4]=(*this).pointList[j-1];
				upperPartition[indice+5]=(*this).pointList[j];

				upperPartition[indice]=(*this).pointList[j-5];
				upperPartition[indice+1]=(*this).pointList[j-4];
				upperPartition[indice+2]=(*this).pointList[j-3];

				upperPartition[indice+6]=(*this).pointList[j+1];
				upperPartition[indice+7]=(*this).pointList[j+2];
				upperPartition[indice+8]=(*this).pointList[j+3];

				indice+=9;
				j+=3; }

if(compteur%3==2) {
				upperPartition[indice+6]=(*this).pointList[j-2];
				upperPartition[indice+7]=(*this).pointList[j-1];
				upperPartition[indice+8]=(*this).pointList[j];

				upperPartition[indice+3]=(*this).pointList[j-3];
				upperPartition[indice+4]=(*this).pointList[j-2];
				upperPartition[indice+5]=(*this).pointList[j-1];

				upperPartition[indice]=(*this).pointList[j-6];
				upperPartition[indice+1]=(*this).pointList[j-5];
				upperPartition[indice+2]=(*this).pointList[j-4];

				indice+=9; }

			}
		}
	}

	return upperPartition ;
}

//partie  inferieure
vector<float> PointList::lowerPartition( char axeMax, float val) {
int compteur =-1;
	vector<float> lowerPartition ;
	lowerPartition.resize((*this).pointList.size()) ;
	unsigned int indice =0;
	if (axeMax == 'x') {
		for(unsigned j = 0 ; j<(*this).pointList.size(); j+=3) {
			compteur++;
			if((*this).pointList[j] < val ) {
//on teste la position du point
if(compteur%3==0){
				lowerPartition[indice]=(*this).pointList[j];
				lowerPartition[indice+1]=(*this).pointList[j+1];
				lowerPartition[indice+2]=(*this).pointList[j+2];

				lowerPartition[indice+3]=(*this).pointList[j+3];
				lowerPartition[indice+4]=(*this).pointList[j+4];
				lowerPartition[indice+5]=(*this).pointList[j+5];

				lowerPartition[indice+6]=(*this).pointList[j+6];
				lowerPartition[indice+7]=(*this).pointList[j+7];
				lowerPartition[indice+8]=(*this).pointList[j+8];

				indice+=9;
				j+=6; }
if(compteur%3==1) {
				lowerPartition[indice+3]=(*this).pointList[j];
				lowerPartition[indice+4]=(*this).pointList[j+1];
				lowerPartition[indice+5]=(*this).pointList[j+2];

				lowerPartition[indice]=(*this).pointList[j-3];
				lowerPartition[indice+1]=(*this).pointList[j-2];
				lowerPartition[indice+2]=(*this).pointList[j-1];

				lowerPartition[indice+6]=(*this).pointList[j+3];
				lowerPartition[indice+7]=(*this).pointList[j+4];
				lowerPartition[indice+8]=(*this).pointList[j+5];

				indice+=9;
				j+=3;	 }

if(compteur%3==2) {
				lowerPartition[indice+6]=(*this).pointList[j];
				lowerPartition[indice+7]=(*this).pointList[j+1];
				lowerPartition[indice+8]=(*this).pointList[j+2];

				lowerPartition[indice+3]=(*this).pointList[j-3];
				lowerPartition[indice+4]=(*this).pointList[j-2];
				lowerPartition[indice+5]=(*this).pointList[j-1];

				lowerPartition[indice]=(*this).pointList[j-6];
				lowerPartition[indice+1]=(*this).pointList[j-5];
				lowerPartition[indice+2]=(*this).pointList[j-4];

				indice+=9; }

			}
		}
	}
	if (axeMax == 'y') {
		for(unsigned j = 1 ; j<(*this).pointList.size(); j+=3) {
			compteur++;
			if((*this).pointList[j] < val ) {
//on teste la position du point
if(compteur%3==0) {
				lowerPartition[indice]=(*this).pointList[j-1];
				lowerPartition[indice+1]=(*this).pointList[j];
				lowerPartition[indice+2]=(*this).pointList[j+1];

				lowerPartition[indice+3]=(*this).pointList[j+2];
				lowerPartition[indice+4]=(*this).pointList[j+3];
				lowerPartition[indice+5]=(*this).pointList[j+4];

				lowerPartition[indice+6]=(*this).pointList[j+5];
				lowerPartition[indice+7]=(*this).pointList[j+6];
				lowerPartition[indice+8]=(*this).pointList[j+7];

				indice+=9;
				j+=6; }
if(compteur%3==1) {
				lowerPartition[indice+3]=(*this).pointList[j-1];
				lowerPartition[indice+4]=(*this).pointList[j];
				lowerPartition[indice+5]=(*this).pointList[j+1];

				lowerPartition[indice]=(*this).pointList[j-4];
				lowerPartition[indice+1]=(*this).pointList[j-3];
				lowerPartition[indice+2]=(*this).pointList[j-2];

				lowerPartition[indice+6]=(*this).pointList[j+2];
				lowerPartition[indice+7]=(*this).pointList[j+3];
				lowerPartition[indice+8]=(*this).pointList[j+4];

				indice+=9;
				j+=3; }

if(compteur%3==2) {
				lowerPartition[indice+6]=(*this).pointList[j-1];
				lowerPartition[indice+7]=(*this).pointList[j];
				lowerPartition[indice+8]=(*this).pointList[j+1];

				lowerPartition[indice+3]=(*this).pointList[j-4];
				lowerPartition[indice+4]=(*this).pointList[j-3];
				lowerPartition[indice+5]=(*this).pointList[j-2];

				lowerPartition[indice]=(*this).pointList[j-7];
				lowerPartition[indice+1]=(*this).pointList[j-6];
				lowerPartition[indice+2]=(*this).pointList[j-5];

				indice+=9; }

			}
		}
	}
if (axeMax == 'z') {
		for(unsigned j = 2 ; j<(*this).pointList.size(); j+=3) {
			compteur++;
			if((*this).pointList[j] < val ) {
//on teste la position du point
if(compteur%3==0) {
				lowerPartition[indice]=(*this).pointList[j-2];
				lowerPartition[indice+1]=(*this).pointList[j-1];
				lowerPartition[indice+2]=(*this).pointList[j];

				lowerPartition[indice+3]=(*this).pointList[j+1];
				lowerPartition[indice+4]=(*this).pointList[j+2];
				lowerPartition[indice+5]=(*this).pointList[j+3];

				lowerPartition[indice+6]=(*this).pointList[j+4];
				lowerPartition[indice+7]=(*this).pointList[j+5];
				lowerPartition[indice+8]=(*this).pointList[j+6];

				indice+=9;
				j+=6; }
if(compteur%3==1) {
				lowerPartition[indice+3]=(*this).pointList[j-2];
				lowerPartition[indice+4]=(*this).pointList[j-1];
				lowerPartition[indice+5]=(*this).pointList[j];

				lowerPartition[indice]=(*this).pointList[j-5];
				lowerPartition[indice+1]=(*this).pointList[j-4];
				lowerPartition[indice+2]=(*this).pointList[j-3];

				lowerPartition[indice+6]=(*this).pointList[j+1];
				lowerPartition[indice+7]=(*this).pointList[j+2];
				lowerPartition[indice+8]=(*this).pointList[j+3];

				indice+=9;
				j+=3; }

if(compteur%3==2) {
				lowerPartition[indice+6]=(*this).pointList[j-2];
				lowerPartition[indice+7]=(*this).pointList[j-1];
				lowerPartition[indice+8]=(*this).pointList[j];

				lowerPartition[indice+3]=(*this).pointList[j-3];
				lowerPartition[indice+4]=(*this).pointList[j-2];
				lowerPartition[indice+5]=(*this).pointList[j-1];

				lowerPartition[indice]=(*this).pointList[j-6];
				lowerPartition[indice+1]=(*this).pointList[j-5];
				lowerPartition[indice+2]=(*this).pointList[j-4];

				indice+=9; }

			}
		}
	}
	return lowerPartition ;
}

//methode qui cree un Kdtree a partir de la liste de points
KdNode PointList::buildKdTree() {

this->pointList.resize(taille);
KdNode node ;
    //la dernière boundingBox est composée de 30 points (10 triangles)
    if((*this).pointList.size() > 90) {
	
      BoundingBox boundingBox = (*this).computeBoundingBox() ;
      char maxAxe = boundingBox.maxAxis() ;
      float point = (*this).findMedianSample(boundingBox) ;
	PointList listUp = PointList((*this).pointList.size());
	PointList listLow = PointList((*this).pointList.size());
      listUp.pointList  = (*this).upperPartition( maxAxe, point) ;
      listLow.pointList  = (*this).lowerPartition( maxAxe, point) ;

      *(node.leftChild) = listUp.buildKdTree();
      *(node.rightChild) = listLow.buildKdTree();
    }
    else{

      node.feuille = (*this).pointList ;
      node.boolFeuille=true;
   	}
    return node ; 
}

//calcul de la boite englobatnte miniaml d'une liste de point
BoundingBox PointList::computeBoundingBox() {
	float xMin = 10000.f;
	float xMax = -10000.f;
	float yMin = 10000.f;
	float yMax = -10000.f;
	float zMin = 10000.f;
	float zMax = -10000.f;

	for(unsigned i =0; i<(*this).pointList.size();i+=3 ){
		if((*this).pointList[i]<xMin){ xMin=(*this).pointList[i];}
		if((*this).pointList[i]>xMax){ xMax=(*this).pointList[i];}

	}
	for(unsigned i =1; i<(*this).pointList.size();i+=3 ){
		if((*this).pointList[i]<yMin){ yMin=(*this).pointList[i];}
		if((*this).pointList[i]>yMax){ yMax=(*this).pointList[i];}

	}
	for(unsigned i =2; i<(*this).pointList.size();i+=3 ){
		if((*this).pointList[i]<zMin){ zMin=(*this).pointList[i];}
		if((*this).pointList[i]>zMax){ zMax=(*this).pointList[i];}

	}
	BoundingBox boundingBox = BoundingBox(xMin, xMax,yMin,yMax,zMin,zMax);
	return boundingBox;
}

/*
//retourne la liste de tous les points de la scene
vector<float> getListOfAllPoints(){
	unsigned int taille=calculTailleTotalDePoints();
	PointList pointList=PointList(taille);
	
	
	unsigned int i=0;
	for (unsigned int s = 0; s < shapes.size (); s++){
		for (unsigned int p = 0; p < shapes[s].mesh.positions.size () / 3; p++) {

			pointList.pointList[i]=shapes[s].mesh.positions[3*p] ;
			pointList.pointList[i+1]=shapes[s].mesh.positions[3*p+1] ;
			pointList.pointList[i+2]=shapes[s].mesh.positions[3*p+2] ;
			i=i+3;
		}
	}
	return pointList.pointList ;
}
*/
#endif

