#include <math.h>
#include <stdio.h>
#include "SDL2/SDL.h"
#include "./includes/Snake.h"
#include "./includes/export.h"
#ifdef AIMODE
#include "SnakeAI.h"
#endif
#ifdef NETWORK_MODE
#include "SnakeNetwork.h"
#endif

RenderingLines *head = NULL, *last = NULL;
int lastKey = 0;
Food *f = NULL;

Command currentCommand = NOOP;

int main(int argc, char *argv[])
{
	int score = 0;
	SDL_Rect food;
	SDL_Window *window = NULL;
	SDL_Event event;
	SDL_Renderer *renderer = NULL;
	int condition = 1;
	if (INIT_SNAKE_Y1 - INIT_SNAKE_Y2 > 0)
	{
		lastKey = SDLK_DOWN;
	}
	else if (INIT_SNAKE_Y1 - INIT_SNAKE_Y2 < 0)
	{
		lastKey = SDLK_UP;
	}
	else if (INIT_SNAKE_X1 - INIT_SNAKE_X2 > 0)
	{
		lastKey = SDLK_RIGHT;
	}
	else if (INIT_SNAKE_X1 - INIT_SNAKE_X2 < 0)
	{
		lastKey = SDLK_LEFT;
	}
#ifdef AIMODE
	float length = calculate_distance(INIT_SNAKE_X1, INIT_SNAKE_Y1, INIT_SNAKE_X2, INIT_SNAKE_Y2);
	init();
#ifdef PROB_HEURISTIC
	Command_List *command_head = NULL;
#endif
#endif
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL_Error: %s\n",
			   SDL_GetError());
		return -1;
	}
#ifdef NETWORK_MODE
	if (SDLNet_Init() == -1)
	{
		printf("SDLNet_Init: %s\n", SDLNet_GetError());
		exit(2);
	}
	TCPsocket client = NULL;
	printf("Waiting for a connection...\n");
	connect_controller(&client);
	printf("Connected...\n");
#endif
	if (SDL_CreateWindowAndRenderer(RES_COL, RES_ROW, SDL_WINDOW_SHOWN,
									&window, &renderer) < 0)
	{
		printf("Window could not be created! SDL_Error: %s\n",
			   SDL_GetError());
		return -1;
	}

	init_snake(&head, &last, INIT_SNAKE_X1, INIT_SNAKE_Y1, INIT_SNAKE_X2,
			   INIT_SNAKE_Y2);
int k=0;
	while (condition)
	{
		while (f == NULL)
		{
			if (create_food(&f) == -1)
			{
				printf("Memory allocation failed\n");
				condition=-1;
			}
			if (detect_body_collision(f->x1,f->y1,head)==-1){
				free(f);
				f=NULL;
				continue;
			}
			food.x = f->x1 - 2;
			food.y = f->y1 - 2;
			food.w = 4;
			food.h = 4;
		}

#if !defined(AIMODE) && !defined(NETWORK_MODE)
		if ((SDL_PollEvent(&event) > 0) && (event.type == SDL_KEYDOWN))
		{
			switch (event.key.keysym.sym)
			{
			case SDLK_LEFT:
				currentCommand = LEFT;
				break;
			case SDLK_RIGHT:
				currentCommand = RIGHT;
				break;
			case SDLK_UP:
				currentCommand = UP;
				break;
			case SDLK_DOWN:
				currentCommand = DOWN;
				break;
			case SDLK_ESCAPE:
				condition = 0;
				break;
			default:
				currentCommand = NOOP;
			}
		}
#else
		if ((SDL_PollEvent(&event) > 0) && (event.type == SDL_KEYDOWN) && (event.key.keysym.sym == SDLK_ESCAPE))
		{
			condition = 0;
		}
#if defined(PROB_HEURISTIC)
		if (command_head == NULL)
		{
			next_command(&command_head);
		}
		currentCommand = pop_command(&command_head);
#elif defined(NETWORK_MODE)
		currentCommand = next_command(client,f,head);
#else
		currentCommand=next_command();
#endif
#endif
		switch (currentCommand)
		{
		case RIGHT:
			if (lastKey == SDLK_LEFT || lastKey == SDLK_RIGHT)
			{
				break;
			}
			head = insert_new_line(head, head->attr->x1,
								   head->attr->y1, 1, 0);
			lastKey = SDLK_RIGHT;
			break;
		case LEFT:
			if (lastKey == SDLK_RIGHT || lastKey == SDLK_LEFT)
			{
				break;
			}
			head = insert_new_line(head, head->attr->x1,
								   head->attr->y1, -1, 0);
			lastKey = SDLK_LEFT;
			break;
		case UP:
			if (lastKey == SDLK_DOWN || lastKey == SDLK_UP)
			{
				break;
			}
			head = insert_new_line(head, head->attr->x1,
								   head->attr->y1, 0, -1);
			lastKey = SDLK_UP;
			break;
		case DOWN:
			if (lastKey == SDLK_UP || lastKey == SDLK_DOWN)
			{
				break;
			}
			head = insert_new_line(head, head->attr->x1,
								   head->attr->y1, 0, 1);
			lastKey = SDLK_DOWN;
			break;
		case STOP:
			condition=0;
		}

		update_lines(&head, &last);
#ifdef AIMODE
#ifdef PROB_HEURISTIC
		update_map(length);
#else
		update_map();
#endif
#endif
		set_color_for_clear(&renderer);
		SDL_RenderClear(renderer);
		set_color_to_draw(&renderer);
		if (f != NULL)
		{
			SDL_RenderFillRect(renderer, &food);
			if (check_food_collision(head->attr->x1, head->attr->y1,
									 food))
			{
				score++;
				free(f);
				f = NULL;
                \\ Break game loop when score reaches the threshold
                if (score>=100){
                    condition=-1;
                }
#ifdef AIMODE
				length++;
#endif
				head->attr->x1 += head->attr->x1_increment;
				head->attr->y1 += head->attr->y1_increment;
			}
		}
        render_snake(&renderer, head);
        SDL_RenderPresent(renderer);
		if (detect_collision(head) == -1)
		{
			break;
		}
#if !defined(AIMODE) && !defined(NETWORK_MODE)
		SDL_Delay(10);
#else
// SDL_Delay(2);
#endif
		// printf("Score %lf\n",calculate_score(head));
	}
	food.x = head->attr->x1;
	food.y = head->attr->y1;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
	SDL_RenderFillRect(renderer, &food);
	SDL_Surface *sshot = SDL_CreateRGBSurface(0, RES_COL, RES_ROW, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
	SDL_SaveBMP(sshot, "screenshot.bmp");
	SDL_FreeSurface(sshot);
	printf("%d %d SCORE=%d\n\n", head->attr->x1, head->attr->y1, score);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
#ifdef NETWORK_MODE
	int status=-1;
	SDLNet_TCP_Send(client,&status,sizeof(int));
	SDLNet_Quit();
#endif
	SDL_Quit();
	return 0;
}
