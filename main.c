#include <math.h>
#include <stdio.h>
#include "SDL2/SDL.h"
#include "./includes/Snake.h"
#include "./includes/export.h"
#ifdef AIMODE
#include "SnakeAI.h"
#endif
RenderingLines *head = NULL, *last = NULL;
int lastKey = 0;
Food *f = NULL;
#ifdef AIMODE
Command currentCommand = NOOP;
#endif
int main(int argc, char *argv[])
{
	SDL_Rect food;
	SDL_Window *window = NULL;
	SDL_Event event;
	SDL_Renderer *renderer = NULL;
	int condition = 1;
	if (INIT_SNAKE_Y1 - INIT_SNAKE_Y2 > 0) {
		lastKey = SDLK_DOWN;
	} else if (INIT_SNAKE_Y1 - INIT_SNAKE_Y2 < 0) {
		lastKey = SDLK_UP;
	} else if (INIT_SNAKE_X1 - INIT_SNAKE_X2 > 0) {
		lastKey = SDLK_RIGHT;
	} else if (INIT_SNAKE_X1 - INIT_SNAKE_X2 < 0) {
		lastKey = SDLK_LEFT;
	}

#ifdef AIMODE
	float length=calculate_distance(INIT_SNAKE_X1,INIT_SNAKE_Y1,INIT_SNAKE_X2,INIT_SNAKE_Y2);
	init();
	#ifdef PROB_HEURISTIC
	Command_List *command_head=NULL;
	#endif
#endif
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL_Error: %s\n",
		       SDL_GetError());
		return -1;
	}
	if (SDL_CreateWindowAndRenderer(RES_COL, RES_ROW, SDL_WINDOW_SHOWN,
					&window, &renderer)
	    < 0) {
		printf("Window could not be created! SDL_Error: %s\n",
		       SDL_GetError());
		return -1;
	}

	init_snake(&head, &last, INIT_SNAKE_X1, INIT_SNAKE_Y1, INIT_SNAKE_X2,
		   INIT_SNAKE_Y2);
	while (condition) {
		if (f == NULL) {
			if (create_food(&f) == -1) {
				break;
			}
			food.x = f->x1 - 2;
			food.y = f->y1 - 2;
			food.w = 4;
			food.h = 4;
		}
#ifndef AIMODE
		if (SDL_PollEvent(&event) > 0) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					if (lastKey == SDLK_LEFT || lastKey == SDLK_RIGHT) {
						break;
					}
					head = insert_new_line(
						head, head->attr->x1,
						head->attr->y1, -1, 0);
					break;
				case SDLK_RIGHT:
					if (lastKey == SDLK_LEFT || lastKey == SDLK_RIGHT) {
						break;
					}
					head = insert_new_line(
						head, head->attr->x1,
						head->attr->y1, 1, 0);
					break;
				case SDLK_UP:
					if (lastKey == SDLK_DOWN || lastKey == SDLK_UP) {
						break;
					}
					head = insert_new_line(
						head, head->attr->x1,
						head->attr->y1, 0, -1);
					break;
				case SDLK_DOWN:
					if (lastKey == SDLK_UP) {
						break;
					}
					head = insert_new_line(
						head, head->attr->x1,
						head->attr->y1, 0, 1);
					break;
				case SDLK_ESCAPE:
					condition = 0;
					break;
				}
				lastKey = event.key.keysym.sym;
				break;
			}
		}
#else
		if (SDL_PollEvent(&event) > 0) {
				if(event.type==SDL_KEYDOWN && event.key.keysym.sym==SDLK_ESCAPE)
				condition = 0;
		}
		#ifndef PROB_HEURISTIC
		currentCommand = next_command();
		#else
		if(command_head==NULL){
			next_command(&command_head);
		}
		currentCommand=pop_command(&command_head);
		#endif

		switch (currentCommand) {
		case RIGHT:
			if (lastKey == SDLK_LEFT || lastKey == SDLK_RIGHT) {
				break;
			}
			head = insert_new_line(head, head->attr->x1,
					       head->attr->y1, 1, 0);
			lastKey=SDLK_RIGHT;
			break;
		case LEFT:
			if (lastKey == SDLK_RIGHT || lastKey==SDLK_LEFT) {
				break;
			}
			head = insert_new_line(head, head->attr->x1,
					       head->attr->y1, -1, 0);
			lastKey=SDLK_LEFT;
			break;
		case UP:
			if (lastKey == SDLK_DOWN || lastKey == SDLK_UP) {
				break;
			}
			head = insert_new_line(head, head->attr->x1,
					       head->attr->y1, 0, -1);
			lastKey=SDLK_UP;
			break;
		case DOWN:
			if (lastKey == SDLK_UP || lastKey == SDLK_DOWN ) {
				break;
			}
			head = insert_new_line(head, head->attr->x1,
					       head->attr->y1, 0, 1);
			lastKey=SDLK_DOWN;
			break;
		}
#endif
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
		if (f != NULL) {
			SDL_RenderFillRect(renderer, &food);
			if (check_food_collision(head->attr->x1, head->attr->y1,
						 food)) {
				free(f);
				f = NULL;
				#ifdef AIMODE
				length++;
				#endif
				head->attr->x1 +=
					(head->attr->x1_increment << 1);
				head->attr->y1 +=
					(head->attr->y1_increment << 1);
			}
		}
		render_snake(&renderer, head);
		SDL_RenderPresent(renderer);
		if (detect_collision(head) == -1) {
			break;
		}
		#ifndef AIMODE
		SDL_Delay(3);
		#else
		//SDL_Delay(3);
		#endif
	}
	food.x=head->attr->x1;
	food.y=head->attr->y1;
	SDL_SetRenderDrawColor(renderer,255,0,0,1);
	SDL_RenderFillRect(renderer, &food);
	SDL_Surface *sshot = SDL_CreateRGBSurface(0, RES_COL, RES_ROW, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
	SDL_RenderReadPixels(renderer, NULL, SDL_PIXELFORMAT_ARGB8888, sshot->pixels, sshot->pitch);
	SDL_SaveBMP(sshot, "screenshot.bmp");
	SDL_FreeSurface(sshot);
	printf("%d %d SCORE=%lf\n\n",head->attr->x1,head->attr->y1,calculate_score(head));
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}
