#include "mywindow.h"
#include <iostream>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QEventLoop>

#include "main.hpp"
#include "body.hpp"
#include "operateur.hpp"
#include "camera.hpp"
#include "light.hpp"

#include <iostream>
#include <sstream>

using namespace std;

namespace m
{
    float x;
    float y,z;
    int roll,pitch,turn,relMouseX,relMouseY;
    bool rightButton;
    bool leftButton;

    int frameTimeMs;
    int updateTimeMs;

    int frameOldCount;
    int frameNewCount;

    int updateOldCount;
    int updateNewCount;

    camera *theCamera;
    body *some;

    light *light1;
    light *light2;

    unsigned width;
    unsigned height;
}

myWindow::myWindow(QWidget *parent)
    : myGLWidget(60, parent, "Test")
{

    timer.start();

    string bvhFile;
    using namespace m;

    bvhFile ="Take 006.bvh";


    int width = 500;
    int height = 400;

    resize(width, height);

    // Definition of the camera
    theCamera = new camera();
    theCamera->move(0,0,0,10.0f,20.0f,-103.0f) ;

    // Definition of 2 lights
    light1 = new light(theCamera,GL_LIGHT1);
    light1->move(0,0,0,100.0f,15.0f,70.3f);

    light1->Kd[0] = 1.0f;
    light1->Kd[1] = 0.2f;
    light1->Kd[2] = 0.2f;

    light2 = new light(theCamera,GL_LIGHT2);
    light2->move(0,0,0,-150.0f,55.0f,20.3f);

    light2->Kd[0] = 1.0f;
    light2->Kd[1] = 1.0f;
    light2->Kd[2] = 0.3f;

    vector<light*> lights;

    lights.push_back(light1);
    lights.push_back(light2);

    //Creation of the body
    some = new body(bvhFile);

    some->lights = lights;

    m::x = 0;
    m::y = 0;
    m::z = 0;
    initializeGL();
    resizeGL(width,height);

    updateTimeMs = (1000/25);
    frameTimeMs = (int)(some->theBvh->frameTime*1000);

    updateOldCount = 0;
    frameOldCount = 0;

    mainLoop();

}

void myWindow::initializeGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_SMOOTH);

    glDisable(GL_LIGHTING);
    glLineWidth(1.0f);
    glEnable (GL_LINE_SMOOTH);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);


}

void myWindow::resizeGL(int width, int height)
{
    if(height == 0)
        height = 1;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,10000.0f);

    glMatrixMode(GL_MODELVIEW);
}

void myWindow::paintGL()
{
    /*
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(-1.5f, 0.0f, -6.0f);

    glBegin(GL_TRIANGLES);
        glVertex3f(0.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
    glEnd();

    glTranslatef(3.0f, 0.0f, -6.0f);

    glBegin(GL_QUADS);
        glVertex3f(-1.0f, 1.0f, 0.0f);
        glVertex3f(-1.0f, -1.0f, 0.0f);
        glVertex3f(1.0f, -1.0f, 0.0f);
        glVertex3d(1.0f, 1.0f, 0.0f);
    glEnd();*/

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    m::theCamera->look();

    m::light1->draw();
    m::light2->draw();

    m::some->draw();

    glFlush();

    //SDL_GL_SwapBuffers();
    swapBuffers();

    glPopMatrix();
}

// Update the drawing every time mainLoop() is called
void myWindow::update(){
    using namespace m;

    frameNewCount = updateNewCount = timer.elapsed();

    if ( (frameNewCount - frameOldCount) > frameTimeMs ) {
        some->update();
        frameOldCount = frameNewCount;
    }

    if ( (updateNewCount - updateOldCount) > updateTimeMs ) {
        //SDL_GetRelativeMouseState(&relMouseX,&relMouseY); je sais pas comment le remplacer ...
        pitch = 0;
        turn = 0;
        roll = 0;

        if (leftButton)
        {
            pitch = relMouseY;
            turn  = relMouseX;
        }
        else if (rightButton)
        {
            roll = relMouseX;
        }

        theCamera->move(pitch,turn,roll,m::x,m::y,m::z);
        //draw();
        paintGL();
        updateOldCount = updateNewCount;
    }
}

//Draw the body
/*void myWindow::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    m::theCamera->look();

    m::light1->draw();
    m::light2->draw();

    m::some->draw();

    glFlush();

    //SDL_GL_SwapBuffers();
    swapBuffers();

    glPopMatrix();
}*/

void myWindow::mousePressEvent(QMouseEvent *e){
    using namespace m;
    switch( e->button()) {
    case Qt::LeftButton:
        leftButton= true;

            break;

    case Qt::RightButton:
        rightButton= true;

            break;
        default:
            break;
    }
}

void myWindow::mouseReleaseEvent(QMouseEvent *e){
    using namespace m;
    switch( e->button()) {
    case Qt::LeftButton:
        leftButton= false;

            break;

    case Qt::RightButton:
        rightButton= false;

            break;
        default:
            break;
    }
}
/*
void terminateApp(void)
{
        static SDL_Event Q;

        Q.type = SDL_QUIT;

        if(SDL_PushEvent(&Q) == -1)
        {
                cerr << "SDL_QUIT event can't be pushed: " << SDL_GetError() << "\n";
                exit(1);
        }
        return;
}*/

// Call the function to handle the mouse and update()
int myWindow::mainLoop()
{
    /*
    bool isProgramLooping = true;
    QEvent* e ;
    //QEventLoop loop;
    while(isProgramLooping)
    {
        /*if(e->processEvents()) {
            switch(e->type()) {

            case QCloseEvent:
                isProgramLooping = false;
                break;
            }
            else {
                update();
            }*/
        update();

            /*case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
                handleInput(&E) ;
                break ;*/
            /*case QMouseEvent:
                break;*
            }*/
       // }
       /* else {
            update();
        }
    }*/
    return 0;
}
