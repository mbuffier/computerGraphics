// ----------------------------------------------
// Informatique Graphique 3D & RÈalitÈ Virtuelle.
// Travaux Pratiques
// Algorithmes de Rendu
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <cmath>
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>


#include "Vec3.h"
#include "Camera.h"
#include "Mesh.h"
#include "Ray.h"
#include "BSHNode.h"


using namespace std;

static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static const string DEFAULT_MESH_FILE ("/Users/Maud/Documents/UCL_CGVI/farAway/repriseTamy/models/rhino.off");

static string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Algorithmes de Rendu");
static GLint window;
static unsigned int FPS = 0;
static bool fullScreen = false;
static float Pi = 3.14159265 ;
static float camPosX , camPosY, camPosZ ;

// Light position
static float posx = -10 ;
static float posy = 10 ;
static float posz = 10 ;
static Vec3f originLight = Vec3f(posx,posy,posz) ;

// Shadow
void computeShading() ;
static vector<bool>  lighting ;

//Anbiant occlusion
void computeAmbiantOcclusion(unsigned int N) ;
Vec3f generateRandomVect(Vec3f normal) ;
static vector<float>  occlusion ;
static vector<Vec3f>  randomVec ;

// Scene definition
static Camera camera;
static Mesh mesh;

//BSHNode
BSHNode* root ;
BSHNode* buildingBSH(const std::vector<Vertex> vertexList) ;
void renderRecursive(const BSHNode *root, int minLeaf) ;
void drawGl(const BSHNode *nood) ;


void printUsage () {
    std::cerr << std::endl
    << appTitle << std::endl
    << "Author: Tamy Boubekeur" << std::endl << std::endl
    << "Usage: ./main [<file.off>]" << std::endl
    << "Commands:" << std::endl
    << "------------------" << std::endl
    << " ?: Print help" << std::endl
    << " w: Toggle wireframe mode" << std::endl
    << " <drag>+<left button>: rotate model" << std::endl
    << " <drag>+<right button>: move model" << std::endl
    << " <drag>+<middle button>: zoom" << std::endl
    << " q, <esc>: Quit" << std::endl << std::endl;
}

void init (const char * modelFilename) {
    glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
    glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
    glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
    glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f);
    
    mesh.loadOFF (modelFilename);
    
    //computeShading() ;
    //computeAmbiantOcclusion(25) ;
    
    root = buildingBSH(mesh.V) ;
    
    camera.resize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
}

void drawScene () {
    glBegin (GL_TRIANGLES);
    for (unsigned int i = 0; i < mesh.T.size (); i++)
        for (unsigned int j = 0; j < 3; j++) {
            const Vertex & v = mesh.V[mesh.T[i].v[j]];
            
            // float puissanceEmission = 2.5 ;
            
            
            //                      ***** LAMBERT BRDF = diffuse term *****
            /*float lightPower = 2.5 ;
             Vec3f diffuse = Vec3f(0.7, 0.8, 0.5)/Pi ;
             
             // Direction from the light
             Vec3f wi = Vec3f(posx - v.p[0] , posy - v.p[1], posz - v.p[2]);
             wi.normalize() ;
             
             Vec3f color = lightPower*diffuse*dot(wi,v.n) ;
             glColor3f (color[0], color[1], color[2]); */
            
            
            //                      ***** BLING-PHONG BRDF *****
             /*float lightPower = 2.5 ;
             Vec3f diffuse = Vec3f(0.0, 0.8, 0.0)/Pi ;
             
             // Direction from the point to the light
             Vec3f wi = Vec3f(posx - v.p[0] , posy - v.p[1], posz - v.p[2]);
             wi.normalize() ;
             
             // Direction from the point to the camera
             camera.getPos(camPosX,camPosY,camPosZ) ;
             Vec3f w0 = Vec3f(camPosX -v.p[0] , camPosY - v.p[1], camPosZ - v.p[2]);
             
             Vec3f wh = w0 + wi ;
             wh.normalize() ;
             
             Vec3f coefSpecular = Vec3f(0.2, 0.6, 0.2) ;
             float shininess = 50 ;
             
             Vec3f specular = coefSpecular*pow(dot(v.n, wh), shininess) ;
             
             Vec3f LightRes = specular+diffuse ;
             Vec3f color = lightPower*LightRes*dot(wi,v.n) ;
             
             glColor3f (color[0], color[1], color[2]); . */
            
            //                      ***** COOK-TORRANCE : microfacet model *****
            
            /*float lightPower = 2.5 ;
             Vec3f diffuse = Vec3f(0.0, 0.8, 0.0)/Pi ;
             
             // Direction from the point to the light
             Vec3f wi = Vec3f(posx - v.p[0] , posy - v.p[1], posz - v.p[2]);
             wi.normalize() ;
             
             // Direction from the point to the camera
             camera.getPos(camPosX,camPosY,camPosZ) ;
             Vec3f w0 = Vec3f(camPosX -v.p[0] , camPosY - v.p[1], camPosZ - v.p[2]);
             
             Vec3f wh = w0 + wi ;
             wh.normalize() ;
             
             // Beckmann distribution
             float alpha = 0.01 ;
             float beckmanDist = (1/(Pi*pow(alpha,2)*pow(dot(v.n,wh),4)))*exp((pow(dot(v.n,wh),2)-1)/(pow(alpha,2)*pow(dot(v.n,wh),2))) ;
             
             // Fresnel term
             float f0 = 0.1 ;
             float fresnel = f0 + (1-f0)*pow(1-max(0.f,dot(wi,wh)),5) ;
             
             // Geometric term for cook-torrance
             float compare = min((dot(v.n,wh)*dot(v.n,wi)*2)/dot(w0,wh),(dot(v.n,wh)*dot(v.n,w0)*2)/dot(w0,wh)) ;
             float geoCook = min(1.f,compare) ;
             
             float specular = (beckmanDist*fresnel*geoCook)/(dot(v.n,wi)*dot(v.n,w0)*4) ;
             
             Vec3f LightRes = Vec3f(specular, specular, specular) + diffuse ;
             Vec3f color = lightPower*LightRes*dot(wi,v.n) ;
             
             glColor3f (color[0], color[1], color[2]); */
            
            
            
            //                      ***** GGX : microfacet model *****
            
            float lightPower = 2.5 ;
            
            /*unsigned int vertexIndex = mesh.T[i].v[j];
            float vertexEclairage = lighting[vertexIndex];
            float vertexOcclusion = occlusion[vertexIndex];*/
            
            //Vec3f diffuse = Vec3f(0.0,vertexOcclusion,0.0)/Pi;
            Vec3f diffuse = Vec3f(0.0,0.8,0.0)/Pi;

            // Direction from the point to the light
            Vec3f wi = Vec3f(posx - v.p[0] , posy - v.p[1], posz - v.p[2]);
            wi.normalize() ;
            
            // Direction from the point to the camera
            camera.getPos(camPosX,camPosY,camPosZ) ;
            Vec3f w0 = Vec3f(camPosX -v.p[0] , camPosY - v.p[1], camPosZ - v.p[2]);
            
            Vec3f wh = w0 + wi ;
            wh.normalize() ;
            
            // GGX distribution
            float alpha = 0.01 ;
            float alphaP = 0.01 ;
            float distributionGGX = pow(alphaP,2)/(Pi*pow(1.f+(pow(alphaP,2)-1.f)*pow(dot(v.n,wh),2),2)) ;
            
            // Fresnel term
            float f0 = 0.01 ;
            float fresnel = f0 + (1-f0)*pow(1-max(0.f,dot(wi,wh)),5) ;
            
            // Geometric term for GGX
            float geoGGX1 = (dot(v.n,wi)*2)/(dot(v.n,wi)+sqrt(pow(alpha,2)+(1-pow(alpha,2))*(pow(dot(v.n,wi),2))))  ;
            float geoGGX2 = (dot(v.n,w0)*2)/(dot(v.n,w0)+sqrt(pow(alpha,2)+(1-pow(alpha,2))*(pow(dot(v.n,w0),2))))  ;
            float geoGGX = geoGGX1*geoGGX2 ;
            
            float specular = (distributionGGX*fresnel*geoGGX)/(dot(v.n,wi)*dot(v.n,w0)*4) ;
            
            Vec3f LightRes = Vec3f(specular, specular, specular) + diffuse ;
            
            Vec3f color = lightPower*LightRes*dot(wi,v.n) ;
            
            //glColor3f (color[0]*vertexEclairage, color[1]*vertexEclairage, color[2]*vertexEclairage);
            glColor3f (color[0], color[1], color[2]);

            glNormal3f (v.n[0], v.n[1], v.n[2]); // Specifies current normal vertex
            glVertex3f (v.p[0], v.p[1], v.p[2]); // Emit a vertex (one triangle is emitted each time 3 vertices are emitted)
        }
    glEnd ();
}

void drawGl(const BSHNode *nood) {
    Vec3f color = nood->color ;
    Vec3f position = nood->position ;
    float radius = nood->radius ;
    
    glColor3f(color[0],color[1],color[2]);
    GLUquadric *quad;
    quad = gluNewQuadric();
    
    glPushMatrix();
    glTranslatef(position[0],position[1],position[2]);
    gluSphere(quad,radius,50,50);
    glPopMatrix();
}

void renderSphere(float x, float y, float z, float radius,int subdivisions,GLUquadricObj *quadric)

{
    glPushMatrix() ;
    glTranslatef( x,y,z ) ;
    gluSphere(quadric, radius, subdivisions,subdivisions);
    glPopMatrix();
    
}
void renderSphere_convenient(float x, float y, float z, float radius,int subdivisions)

{
    //the same quadric can be re-used for drawing many spheres
    GLUquadricObj *quadric=gluNewQuadric();
    gluQuadricNormals(quadric, GLU_SMOOTH);
    renderSphere(x,y,z,radius,subdivisions,quadric);
    gluDeleteQuadric(quadric);
    
}

void reshape(int w, int h) {
    camera.resize (w, h);
}

BSHNode* buildingBSH(const std::vector<Vertex> vertexList) {
    BSHNode * root = new BSHNode() ;
    root->vertexLeaf = vertexList ;
    root->boundingSphere() ;
    
    std::vector<Vertex> leftList = root->separationLeft() ;
    
    std::vector<Vertex> rightList = root->separationRight() ;
    
    if(leftList.size() > 100) {
        root->leftChild = buildingBSH(leftList) ;
    }
    
    if((rightList).size() > 100) {
        root->rightChild = buildingBSH(rightList) ;
    }
    
    return root ;
} ;


void renderRecursive(const BSHNode *root, int minLeaf) {
    drawGl(root) ;

    if(root->leftChild->vertexLeaf.size() >= minLeaf) {
        renderRecursive(root->leftChild,minLeaf) ;
    }
    if(root->rightChild->vertexLeaf.size() >= minLeaf) {
        renderRecursive(root->rightChild,minLeaf) ;
    }
}

void display () {
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    camera.apply ();
    //drawScene ();
    //renderRecursive(root, 500) ;
    renderSphere_convenient(0.0,0.0,0.0, 50,50) ;
    glFlush ();
    glutSwapBuffers ();
}

void key (unsigned char keyPressed, int x, int y) {
    switch (keyPressed) {
        case 'f':
            if (fullScreen) {
                glutReshapeWindow (camera.getScreenWidth (), camera.getScreenHeight ());
                fullScreen = false;
            } else {
                glutFullScreen ();
                fullScreen = true;
            }
            break;
        case 'q':
        case 27:
            exit (0);
            break;
        case 'w':
            GLint mode[2];
            glGetIntegerv (GL_POLYGON_MODE, mode);
            glPolygonMode (GL_FRONT_AND_BACK, mode[1] ==  GL_FILL ? GL_LINE : GL_FILL);
            break;
            break;
        default:
            printUsage ();
            break;
    }
}

void mouse (int button, int state, int x, int y) {
    camera.handleMouseClickEvent (button, state, x, y);
}

void motion (int x, int y) {
    camera.handleMouseMoveEvent (x, y);
}

void computeShading() {
    
    lighting.resize(mesh.V.size());
    int position = 0 ;
    for (unsigned int i = 0; i < mesh.V.size(); i++) {
        
        const Vertex & v = mesh.V[i];
        Vec3f wi = Vec3f(posx - v.p[0] , posy - v.p[1], posz - v.p[2]);
        Vec3f n = Vec3f(v.n[0], v.n[1], v.n[2]);
        lighting[position] = true ;
        
        for (unsigned int k=0; k < mesh.T.size() ; k++) {
            const Vertex & v1 = mesh.V[mesh.T[k].v[0]];
            const Vertex & v0 = mesh.V[mesh.T[k].v[1]];
            const Vertex & v2 = mesh.V[mesh.T[k].v[2]];
            
            if (intersect(v.p + 0.0001f*n, wi, v0.p, v1.p, v2.p)) {
                lighting[position] = false ;
                break ;
            }
        }
        position ++ ;
    }
}

void computeAmbiantOcclusion(unsigned int N) {
    occlusion.resize(mesh.V.size());
    
    for (unsigned int i = 0; i < mesh.V.size(); i++) {
        const Vertex & v = mesh.V[i];
        Vec3f normal = Vec3f(v.n[0], v.n[1], v.n[2]) ;
        
        int result =0 ;
        for(int i=0; i<N ;i++) {
            Vec3f randVec = generateRandomVect(normal) ;

            for (unsigned int k=0; k < mesh.T.size() ; k++) {
                const Vertex & v1 = mesh.V[mesh.T[k].v[0]];
                const Vertex & v0 = mesh.V[mesh.T[k].v[1]];
                const Vertex & v2 = mesh.V[mesh.T[k].v[2]];
                
                if (intersect(v.p + 0.0001f*normal, randVec , v0.p, v1.p, v2.p)) {
                    result += 1 ;
                    break ;
                }
            }
        }
        float occlu = float(result)/float(N) ;
        occlusion[i] = 1.0 - occlu ;
    }
}


Vec3f generateRandomVect(Vec3f normal)
{
    Vec3f v = Vec3f(rand()%2, rand()%2, rand()%2);
    v.normalize() ;
    return v * (dot(v, normal)/abs(dot(v, normal))) ;
}

void idle () {
    static float lastTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    static unsigned int counter = 0;
    counter++;
    float currentTime = glutGet ((GLenum)GLUT_ELAPSED_TIME);
    if (currentTime - lastTime >= 1000.0f) {
        FPS = counter;
        counter = 0;
        static char winTitle [128];
        unsigned long numOfTriangles = mesh.T.size ();
        sprintf (winTitle, "Number Of Triangles: %lu - FPS: %d", numOfTriangles, FPS);
        glutSetWindowTitle (winTitle);
        lastTime = currentTime;
    }
    glutPostRedisplay ();
}

int main (int argc, char ** argv) {
    if (argc > 2) {
        printUsage ();
        exit (1);
    }
    glutInit (&argc, argv);
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT);
    window = glutCreateWindow (appTitle.c_str ());
    init (argc == 2 ? argv[1] : DEFAULT_MESH_FILE.c_str ());
    glutIdleFunc (idle);
    glutReshapeFunc (reshape);
    glutDisplayFunc (display);
    glutKeyboardFunc (key);
    glutMotionFunc (motion);
    glutMouseFunc (mouse);
    printUsage ();
    glutMainLoop ();
    return 0;
}