#ifndef SNAKE_NETWORK_H
#define SNAKE_NETWORK_H


#include "Structures.h"
#include "SDL2/SDL_net.h"


Command next_command(TCPsocket,Food*,RenderingLines*);
void connect_controller(TCPsocket*);

#endif