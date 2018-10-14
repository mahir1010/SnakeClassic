#include "SDL2/SDL.h"
#include "./libs/Snake.h"
#include <math.h>
#include <stdio.h>
#include "export.h"

int main(int argc, char *argv[])
{
	SDL_Rect food;
	Food *f = NULL;
	RenderingLines *head = NULL, *last = NULL;
	int lastKey = SDLK_DOWN;
	SDL_Window *window = NULL;
	SDL_Event event;
	SDL_Renderer *renderer = NULL;
	int condition = 1;
	
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}
	if (SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_SHOWN, &window, &renderer) < 0) {
		printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
		return -1;
	}

	init_snake(&head, &last, INIT_SNAKE_X1,INIT_SNAKE_Y1, INIT_SNAKE_X2, INIT_SANKE_Y2);
	while (condition) {
		if (SDL_PollEvent(&event) > 0) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					if (lastKey == SDLK_RIGHT) {
						break;
					}
					head = insert_new_line(head, head->attr->x1, head->attr->y1, -1, 0);
					break;
				case SDLK_RIGHT:
					if (lastKey == SDLK_LEFT) {
						break;
					}
					head = insert_new_line(head, head->attr->x1, head->attr->y1, 1, 0);
					break;
				case SDLK_UP:
					if (lastKey == SDLK_DOWN) {
						break;
					}
					head = insert_new_line(head, head->attr->x1, head->attr->y1, 0, -1);
					break;
				case SDLK_DOWN:
					if (lastKey == SDLK_UP) {
						break;
					}
					head = insert_new_line(head, head->attr->x1, head->attr->y1, 0, 1);
					break;
				case SDLK_ESCAPE:
					condition = 0;
					break;
				}
				lastKey = event.key.keysym.sym;
				break;
			}
		}
		update_lines(&head, &last);
		set_color_for_clear(&renderer);
		SDL_RenderClear(renderer);
		set_color_to_draw(&renderer);
		if (f == NULL) {
			if (create_food(&f) == -1) {
				break;
			}
			food.x = f->x1 - 2;
			food.y = f->y1 - 2;
			food.w = 4;
			food.h = 4;
		}
		if (f != NULL) {
			SDL_RenderFillRect(renderer, &food);
			if (check_food_collision(head->attr->x1, head->attr->y1, food)) {
				free(f);
				f = NULL;
				head->attr->x1 += (head->attr->x1_increment <<3);
				head->attr->y1 += (head->attr->y1_increment <<3);
			}
		}
		render_snake(&renderer, head);
		SDL_RenderPresent(renderer);
		if (detect_collision(head) == -1) {
			break;
		}
		SDL_Delay(5);
	}
	printf("SCORE=%lf\n",calculate_score(head));
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
