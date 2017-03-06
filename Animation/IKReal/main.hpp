#ifndef MAIN_HPP
#define MAIN_HPP

#include <SDL/SDL.h>

class fileNotFound {};
	
int main(int argc, char* argv[]);
void initGL(int width, int height);
void initSDL(int width, int height);

void update();
void draw();
void handleInput(SDL_Event *e);
int mainLoop();
void terminateApp();
#endif //MAIN_HPP

