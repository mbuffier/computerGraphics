// ----------------------------------------------
// Informatique Graphique 3D & Réalité Virtuelle.
// Travaux Pratiques
// Introduction à OpenGL
// Copyright (C) 2015 Tamy Boubekeur
// All rights reserved.
// ----------------------------------------------

#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include <GLUT/GLUT.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

using namespace std;

// App parameters
static const unsigned int DEFAULT_SCREENWIDTH = 1024;
static const unsigned int DEFAULT_SCREENHEIGHT = 768;
static string appTitle ("Informatique Graphique & Realite Virtuelle - Travaux Pratiques - Introduction a OpenGL");
static GLint window;
static unsigned int screenWidth;
static unsigned int screenHeight;

// Sphere parameters
static float radius = 1.f ;

// function prototype
void glSphere (float pos_x, float pos_y, float pos_z, float radius) ;
void glSphereWithMat (float pos_x, float pos_y, float pos_z, float radius, float difR, float difG, float difB,float specR, float specG, float specB, float shininess) ;
void genCheckerboard (unsigned int width, unsigned int height, unsigned char image[][3]) ;

// Light parameters
static bool light0 = true ;
static bool light1 = true ;

// Texture parameters
static unsigned int width = 8;
static unsigned int height = 8 ;

// Animation
static float currentTime ;
static float acceleration = 1.0 ;

// Mouse movements
static float camPosX, camPosY , camPosZ, deltaAngleX= 0.1 , deltaAngleY = 0.1 ;
static float xOrigine ;
static int yOrigine ;

// Camera parameters
static float fovAngle;
static float aspectRatio;
static float nearPlane;
static float farPlane;
static float camPhi; // Expressing the camera position in polar coordinate, in the frame of the target
static float camTheta;
static float camDist2Target;
static float camTargetX;
static float camTargetY;
static float camTargetZ;


void polar2Cartesian (float phi, float theta, float d, float & x, float & y, float & z) {
    x = d*sin (theta) * cos (phi);
    y = d*cos (theta);
    z = d*sin (theta) * sin (phi);
    
}

void printUsage () {
    std::cerr << std::endl // send a line break to the standard error output
    << appTitle << std::endl
    << "Author : Tamy Boubekeur" << std::endl << std::endl
    << "Usage : ./main [<file.off>]" << std::endl
    << "Cammandes clavier :" << std::endl
    << "------------------" << std::endl
    << " ?: Print help" << std::endl
    << " w: Toggle wireframe mode" << std::endl
    << " <drag>+<left button>: rotate model" << std::endl
    << " <drag>+<right button>: move model" << std::endl
    << " <drag>+<middle button>: zoom" << std::endl
    << " q, <esc>: Quit" << std::endl << std::endl;
}

void init () {
    // OpenGL initialization
    glCullFace (GL_BACK);     // Specifies the faces to cull (here the ones pointing away from the camera)
    glEnable (GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc (GL_LESS); // Specify the depth test for the z-buffer
    glEnable (GL_DEPTH_TEST); // Enable the z-buffer in the rasterization
    glLineWidth (2.0); // Set the width of edges in GL_LINE polygon mode
    glClearColor (0.0f, 0.0f, 0.0f, 1.0f); // Background color
    
    // Camera initialization
    fovAngle = 45.f;
    nearPlane = 0.01;
    farPlane = 10.0;
    camPhi = M_PI/2.0;
    camTheta = M_PI/2.0;
    camDist2Target = 5.0;
    camTargetX = 0.0;
    camTargetY = 0.0;
    camTargetZ = 0.0;
    
    glEnable (GL_LIGHTING) ;
    
    // First light
    GLfloat light_position0[4] = {10.0f, 10.0f, 10.0f,1.0f};
    GLfloat color0[4] = {1.f, 1.f, 1.f,1.0f};
    glLightfv (GL_LIGHT0, GL_POSITION, light_position0);
    glLightfv (GL_LIGHT0, GL_DIFFUSE, color0);
    glLightfv (GL_LIGHT0, GL_SPECULAR, color0);
    glEnable (GL_LIGHT0);
    
    // Second light
    GLfloat light_position1[4] = {-10.0f, 0.0f, -1.0f,1.0f};
    GLfloat color1[4] = {1.0f, 1.0f, 1.0f,1.0f};
    glLightfv (GL_LIGHT1, GL_POSITION, light_position1);
    glLightfv (GL_LIGHT1, GL_DIFFUSE, color1);
    glLightfv (GL_LIGHT1, GL_SPECULAR, color1);
    glEnable (GL_LIGHT1);
    
    // initialisation for the texture
    unsigned char image[width*height][3] ;
    
    genCheckerboard(width ,height ,image) ;
    
    // set texture parameters
    GLuint texture;
    glEnable (GL_TEXTURE_2D);
    glGenTextures (1, &texture);
    glBindTexture (GL_TEXTURE_2D, texture);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image) ;
}

void setupCamera () {
    glMatrixMode (GL_PROJECTION); // Set the projection matrix as current. All upcoming matrix manipulations will affect it.
    glLoadIdentity ();
    gluPerspective (fovAngle, aspectRatio, nearPlane, farPlane); // Set the current projection matrix with the camera intrinsics
    glMatrixMode (GL_MODELVIEW); // Set the modelview matrix as current. All upcoming matrix manipulations will affect it.
    glLoadIdentity ();
    
    polar2Cartesian (camPhi, camTheta, camDist2Target, camPosX, camPosY, camPosZ);
    camPosX += camTargetX;
    camPosY += camTargetY;
    camPosZ += camTargetZ;
    gluLookAt (camPosX, camPosY, camPosZ, camTargetX, camTargetY, camTargetZ, 0.0, 1.0, 0.0); // Set up the current modelview matrix with camera transform
}

void reshape (int w, int h) {
    screenWidth = w;
    screenHeight = h;
    aspectRatio = static_cast<float>(w)/static_cast<float>(h);
    glViewport (0, 0, (GLint)w, (GLint)h); // Dimension of the drawing region in the window
    setupCamera ();
}


void glSphere (float pos_x, float pos_y, float pos_z, float radius) {
    
    glMatrixMode (GL_MODELVIEW);
    glPushMatrix() ;
    glTranslatef(pos_x,pos_y,pos_z) ;
    
    glBegin(GL_TRIANGLES) ;
    
    double phi = 0;
    double theta =0 ;
    
    const  double PI = 3.1415926535897;
    
    for ( phi = 0; phi <= 2*PI ; phi += 0.1) {
        for ( theta =0 ; theta <= 2*PI ; theta += 0.1 ) {
            
            // First triangle
            float x=0,y=0,z=0 ;
            
            polar2Cartesian(phi,theta,radius, x,  y,  z) ;
            //glColor3f(x,y,z);
            glNormal3f (x/radius,y/radius,z/radius) ;
            glTexCoord2f(phi,theta) ;
            glVertex3f(x,y,z) ;
            
            polar2Cartesian(phi,theta+0.1,radius, x,  y,  z) ;
            //glColor3f(x,y,z);
            glNormal3f (x/radius,y/radius,z/radius) ;
            glTexCoord2f(phi,theta+0.1) ;
            glVertex3f(x,y,z) ;
            
            polar2Cartesian(phi+0.1,theta+0.1,radius, x,  y,  z) ;
            //glColor3f(x,y,z);
            glNormal3f (x/radius,y/radius,z/radius) ;
            glTexCoord2f(phi+0.1,theta+0.1) ;
            glVertex3f(x,y,z) ;
            
            // Second triangle
            polar2Cartesian(phi,theta,radius, x,  y,  z) ;
            //glColor3f(x,y,z);
            glNormal3f (x/radius,y/radius,z/radius) ;
            glTexCoord2f(phi,theta) ;
            glVertex3f(x,y,z) ;
            
            polar2Cartesian(phi+0.1,theta+0.1,radius, x,  y,  z) ;
            //glColor3f(x,y,z);
            glNormal3f (x/radius,y/radius,z/radius) ;
            glTexCoord2f(phi+0.1,theta+0.1) ;
            glVertex3f(x,y,z) ;
            
            polar2Cartesian(phi+0.1,theta,radius, x,  y,  z) ;
            //glColor3f(x,y,z);
            glNormal3f (x/radius,y/radius,z/radius) ;
            glTexCoord2f(phi+0.1,theta) ;
            glVertex3f(x,y,z) ;
            
        }
    }
    glEnd() ;
    
    glPopMatrix() ;
}

void glSphereWithMat (float pos_x, float pos_y, float pos_z, float radius, float difR, float difG, float difB,float specR, float specG, float specB, float shininess) {
    
    GLfloat material_color[4] = {difR, difG, difB, 1.0f};
    GLfloat material_specular[4] = {specR, specG, specB, 1.0};
    glMaterialfv (GL_FRONT_AND_BACK, GL_SPECULAR, material_specular);
    glMaterialfv (GL_FRONT_AND_BACK, GL_DIFFUSE, material_color);
    glMaterialf (GL_FRONT_AND_BACK, GL_SHININESS, shininess*128);
    
    glSphere(pos_x,pos_y,pos_z, radius) ;
    
}

void genCheckerboard (unsigned int width, unsigned int height,unsigned char image[][3]) {
    
    for (unsigned int j=0 ; j < width*height ; j+=2) {
        // creation of a checkboard with different colors
        int line = j/width ;
        
        if(line%2) {
            image[j][0] = 102 ;
            image[j][1] =  205;
            image[j][2] = 0 ;
            
            image[j+1][0] = 139 ;
            image[j+1][1] = 71 ;
            image[j+1][2] = 137; }
        
        else {
            image[j][0] = 139 ;
            image[j][1] = 71 ;
            image[j][2] = 137 ;
            
            image[j+1][0] = 102 ;
            image[j+1][1] = 205 ;
            image[j+1][2] = 0; }
    }
}

void display () {
    
    setupCamera ();
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Erase the color and z buffers.
    
    // ********* Simple triangle *********
     /*glBegin(GL_TRIANGLES) ;
     glColor3f(1.0,0.0,0.0) ;
     glVertex3f(0.0,0.0,0.0) ;
     glColor3f(0.0,1.0,0.0) ;
     glVertex3f(1.0,0.0,0.0) ;
     glColor3f(0.0,0.0,1.0) ;
     glVertex3f(1.0,1.0,0.0) ;
     glEnd() ; */
    
    
    // ********* Pyramide without diffuse and specular components *********
    // Level 1
     /*glSphere(-1.5,-1,-2,radius) ;
     glSphere(-1.5+2*radius,-1,-2,radius) ;
     glSphere(-1.5+4*radius,-1,-2,radius) ;
     glSphere(-1.5+3*radius,-1,-2-sqrt(3)*radius,radius) ;
     glSphere(-1.5+radius,-1,-2-sqrt(3)*radius,radius) ;
     glSphere(-1.5+2*radius,-1,-2-2*sqrt(3)*radius,radius) ;
     
     // Level 2
     glSphere(-1.5+2*radius,-1+radius*1.632993161,-2-2*sqrt(3)*radius+0.5773502691*radius, radius) ;
     glSphere(-1.5+radius,-1+radius*1.632993161,-sqrt(3)*radius-2+0.5773502691*radius,radius) ;
     glSphere(-1.5+3*radius,-1+radius*1.632993161,-sqrt(3)*radius-2+0.5773502691*radius,radius) ;
     
     // Level 3
     glSphere(-1.5+2*radius,-1+2*radius*1.632993161,-2-sqrt(3)*radius,radius) ;*/
    
    
    
    // ********* Pyramide with diffuse and specular components *********
    /*// Level 1
    glSphereWithMat(-1.5,-1,-2,radius,1.0,0.f,0.f,0.5,0.5,0.5,6) ;
    glSphereWithMat(-1.5+2*radius,-1,-2,radius,0.0,1.f,0.f,0.5,0.5,0.5,6) ;
    glSphereWithMat(-1.5+4*radius,-1,-2,radius,0.0,0.f,1.f,0.5,0.5,0.5,6) ;
    glSphereWithMat(-1.5+3*radius,-1,-2-sqrt(3)*radius,radius,1.0,0.f,0.f,0.5,0.5,0.5,6) ;
    glSphereWithMat(-1.5+radius,-1,-2-sqrt(3)*radius,radius,0.0,1.f,0.f,0.5,0.5,0.5,6) ;
    glSphereWithMat(-1.5+2*radius,-1,-2-2*sqrt(3)*radius,radius,0.0,0.f,1.f,0.5,0.5,0.5,6) ;
    
    // Level 2
    glSphereWithMat(-1.5+2*radius,-1+radius*1.632993161,-2-2*sqrt(3)*radius+0.5773502691*radius, radius,0.8,0.2,0.8,0.1,0.1,0.1,5) ;
    glSphereWithMat(-1.5+radius,-1+radius*1.632993161,-sqrt(3)*radius-2+0.5773502691*radius,radius,0.8,0.8,0.2,0.1,0.1,0.1,5) ;
    glSphereWithMat(-1.5+3*radius,-1+radius*1.632993161,-sqrt(3)*radius-2+0.5773502691*radius,radius,0.2,0.8,0.8,0.1,0.1,0.1,5) ;
    
    // Level 3
    glSphereWithMat(-1.5+2*radius,-1+2*radius*1.632993161,-2-sqrt(3)*radius,radius,0.9,0.2,0.2,0.7,0.7,0.7,0.7) ; */

    
    // ********* Pyramide with colors components and animation *********
    // Level 1
    glSphereWithMat(-1.5+sin((currentTime/150)*acceleration),-1,-2+sin((currentTime/150)*acceleration),radius,1.0,0.f,0.f,0.5,0.5,0.5,6) ;
    glSphereWithMat(-1.5+2*radius,-1,-2,radius,0.0,1.f,0.f,0.5,0.5,0.5,6) ;
    glSphereWithMat(-1.5+4*radius+sin((currentTime/150)*acceleration),-1,-2-sin((currentTime/150)*acceleration),radius,0.0,0.f,1.f,0.5,0.5,0.5,6) ;
    glSphereWithMat(-1.5+3*radius,-1,-2-sqrt(3)*radius,radius,1.0,0.f,0.f,0.5,0.5,0.5,6) ;
    glSphereWithMat(-1.5+radius,-1,-2-sqrt(3)*radius,radius,0.0,1.f,0.f,0.5,0.5,0.5,6) ;
    glSphereWithMat(-1.5+2*radius,-1,-2-2*sqrt(3)*radius,radius,0.0,0.f,1.f,0.5,0.5,0.5,6) ;
    
    // Level 2
    glSphereWithMat(-1.5+2*radius,-1+radius*1.632993161,-2-2*sqrt(3)*radius+0.5773502691*radius, radius,0.8,0.2,0.8,0.1,0.1,0.1,5) ;
    glSphereWithMat(-1.5+radius,-1+radius*1.632993161,-sqrt(3)*radius-2+0.5773502691*radius,radius,0.8,0.8,0.2,0.1,0.1,0.1,5) ;
    glSphereWithMat(-1.5+3*radius,-1+radius*1.632993161,-sqrt(3)*radius-2+0.5773502691*radius,radius,0.2,0.8,0.8,0.1,0.1,0.1,5) ;
    
    // Level 3
    glSphereWithMat(-1.5+2*radius+radius/2*sin((currentTime/300)*acceleration),-1+2*radius*1.632993161,-2-sqrt(3)*radius+radius*cos((currentTime/300)*acceleration),radius,0.9,0.2,0.2,0.7,0.7,0.7,0.7) ;
    
    
    glFlush () ; // Ensures any previous OpenGL call has been executed
    glutSwapBuffers ();  // swap the render buffer and the displayed (screen) one
    
}

void keyboard (unsigned char keyPressed, int x, int y) {
    
    switch (keyPressed) {
        case 'w':
            GLint mode[2];
            glGetIntegerv (GL_POLYGON_MODE, mode);
            glPolygonMode (GL_FRONT_AND_BACK, mode[1] ==  GL_FILL ? GL_LINE : GL_FILL);
            break;
            // LightSource 0 activated with a
        case 'a' :
            if (light0) {
                glDisable(GL_LIGHT0);
                light0 = false ;}
            else {
                glEnable(GL_LIGHT0) ;
                light0 = true ; }
            break ;
            
            // LightSource 1 activated with z
        case 'z' :
            if (light1) {
                glDisable(GL_LIGHT1) ;
                light1 = false ;}
            else {
                glEnable(GL_LIGHT1) ;
                light1 = true ;
            }
            break ;
            // Increased or decreased the acceleration of the animation
        case '-' :
            acceleration -= 0.5 ;
            break ;
            
        case '+' :
            acceleration += 0.5 ;
            break ;
            
        case 27:
            exit (0);
            break;
        default:
            printUsage ();
            break;
    }
    glutPostRedisplay ();
}

void mouse (int button, int state, int x, int y) {
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_UP) {
            yOrigine = -1 ;
            xOrigine = -1;
        }
        else  {
            xOrigine = x;
            yOrigine = y ;
        }
    }
}

void motion (int x, int y) {
    if (xOrigine >= 0  || yOrigine >= 0 ) {
        
        deltaAngleX = (x - xOrigine) ;
        deltaAngleY = (y - yOrigine) ;
        
        // Update camPhi and camTheta
        camPhi += deltaAngleX/1000.0f;
        camTheta+=deltaAngleY / 1000.0f;
        
        xOrigine = x;
        yOrigine = y;
    }
}

// This function is executed in an infinite loop. It updated the window title
// (frame-per-second, model size) and ask for rendering
void idle () {
    glutPostRedisplay () ;
    currentTime = glutGet((GLenum)GLUT_ELAPSED_TIME);
}

int main (int argc, char ** argv) {
    glutInit (&argc, argv); // Initialize a glut app
    glutInitDisplayMode (GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); // Setup a RGBA framebuffer to display, with a depth buffer (z-buffer), in double buffer mode (fill a buffer then update the screen)
    glutInitWindowSize (DEFAULT_SCREENWIDTH, DEFAULT_SCREENHEIGHT); // Set the window app size on screen
    window = glutCreateWindow (appTitle.c_str ()); // create the window
    init (); // Your initialization code (OpenGL states, geometry, material, lights, etc)
    glutReshapeFunc (reshape); // Callback function executed whenever glut need to setup the projection matrix
    glutDisplayFunc (display); // Callback function executed when the window app need to be redrawn
    glutKeyboardFunc (keyboard); // Callback function executed when the keyboard is used
    glutMouseFunc (mouse); // Callback function executed when a mouse button is clicked 
    glutMotionFunc (motion); // Callback function executed when the mouse move
    glutIdleFunc (idle); // Callback function executed continuously when no other event happens (good for background procesing or animation for instance).
    printUsage (); // By default, display the usage help of the program   
    glutMainLoop ();
    return 0;
}