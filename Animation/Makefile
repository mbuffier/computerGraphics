CC = g++
CPP = g++

CIBLE = bioviewer

SRCS =  operateur.cpp camera.cpp light.cpp body.cpp bvh.cpp main.cpp movable.cpp
OBJS = $(SRCS:.cpp=.o)

SDL_CFLAGS := $(shell sdl-config --cflags)
SDL_LDFLAGS := $(shell sdl-config --libs)

FLAGS = -Wall -O2 

CFLAGS = $(SDL_CFLAGS) $(FLAGS)
CXXFLAGS = $(SDL_CFLAGS) $(FLAGS)

LIBS = -lGL -lGLU -lm $(SDL_LDFLAGS)
INCLUDE = 
#-B ./include/ -I ../GLGooey/include/glgooey/

$(CIBLE): $(OBJS)
	g++ $(LDFLAGS) -o $(CIBLE) $(OBJS) $(LIBS)

clean:
	rm -f  *~  $(CIBLE) $(OBJS)
