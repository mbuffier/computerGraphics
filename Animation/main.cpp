/*****************************************************************************

Projet IGR204 - Lena Petersen, Shane Nana Yemming, Maud Buffier 

******************************************************************************/

#include <iostream>
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>

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
	float x,y,z;
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

int main(int argc, char* argv[])
{
	// Definition of the file
	string bvhFile;
	using namespace m;
	
	if (argc > 1)
		bvhFile = argv[1];
	else 
		bvhFile ="Take 006.bvh";
	
	width = 400;
	height = 300;
	
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
	
	x = y = z = 0;	
	initSDL(width,height);
	initGL(width,height);

	updateTimeMs = (1000/25); 
	frameTimeMs = (int)(some->theBvh->frameTime*1000);
	
	updateOldCount = 0;
	frameOldCount = 0;
	
	mainLoop();

	return 0;
}

// Definition of the windows GL and SDL
void initGL(int width, int height)	       
{	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDepthFunc(GL_LEQUAL);			    
	glEnable(GL_DEPTH_TEST);	
	glShadeModel(GL_SMOOTH);		

	glDisable(GL_LIGHTING);
	glLineWidth(1.0f);	
	glEnable (GL_LINE_SMOOTH);
	
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
	glViewport(0, 0, width, height);
   	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				

	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,10000.0f);	

	glMatrixMode(GL_MODELVIEW);
}

void initSDL(int width, int height)
{	
	srand(time(NULL));

	if ( SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER ) < 0 ) {
		cerr << "Unable to initialize SDL: " << SDL_GetError() << "\n"; //>
      		exit(1);
	}

	atexit(SDL_Quit);

	  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	unsigned int flags = SDL_OPENGL;

   	if ( SDL_SetVideoMode(width, height, 32, flags) == NULL ) {
		cerr << "Unable to create OpenGL screen: " << SDL_GetError() << "\n"; 
		SDL_Quit();
		exit(2);
	}
	
	SDL_WM_SetCaption("bioviewer", NULL);

}

// Update the drawing every time mainLoop() is called
void update()					
{
	using namespace m;
	
	frameNewCount = updateNewCount = SDL_GetTicks();
	
	if ( (frameNewCount - frameOldCount) > frameTimeMs ) {
		some->update();
		frameOldCount = frameNewCount;
	}
	
	if ( (updateNewCount - updateOldCount) > updateTimeMs ) {
		SDL_GetRelativeMouseState(&relMouseX,&relMouseY);
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
			
		theCamera->move(pitch,turn,roll,m::x,m::y,z);
		draw() ;
		updateOldCount = updateNewCount;
	}	
}

//Draw the body
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	m::theCamera->look();

	m::light1->draw();
	m::light2->draw();

	m::some->draw();
	
	glFlush();

	SDL_GL_SwapBuffers();
	
	glPopMatrix();
}

// Handle the mouse movements
void handleInput(SDL_Event *E)
{
	using namespace m;
	
	rightButton = false;

	switch(E->type) {
		case SDL_MOUSEBUTTONDOWN:

			switch( E->button.button) {
				case SDL_BUTTON_LEFT:
					leftButton= true;

						break;
						
				case SDL_BUTTON_RIGHT:
					rightButton= true;

						break;
					default:
						break;
			}
			break;
					
		case SDL_MOUSEBUTTONUP:
			switch(E->button.button)
			{
				case SDL_BUTTON_LEFT:
					leftButton=false;
		
					break;
				case SDL_BUTTON_RIGHT:
					rightButton=false;
		
					break;
				default:
					break;
			}
			break;
	}
	return;
}

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
}

// Call the function to handle the mouse and update() 
int mainLoop()
{
	bool isProgramLooping = true;			
	SDL_Event E ;

	while(isProgramLooping)
	{
		if(SDL_PollEvent(&E)) {
			switch(E.type) {
				
			case SDL_QUIT:
				isProgramLooping = false;
				break;				
								
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				handleInput(&E) ;
				break ;
			}
		} 
		else {
			update();
		}	
	}
	return 0;		
}
