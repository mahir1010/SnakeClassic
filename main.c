#include "SDL2/SDL.h"
#include "RenderSnake.h"
#include <math.h>

/**
 * This function checks whether head has collided with food or not
 * @param  x    x co-ordinate of head
 * @param  y    y co-ordinate of head
 * @param  food SDL_Rect containing food
 * @return      returns 1 if condition true or 0 if condition false
 */
int check_food_collision(int x, int y, SDL_Rect food) {
	return (x <= food.x + food.w && x >= food.x) && (y <= food.y + food.h && y >= food.y);
}
/**
 * Creates food on random Points
 * @param  f Pointer structure storing Attributes of Food
 * @return   return -1 if memory not allocated
 */
int create_food(Food **f) {
	*f = (Food*)malloc(sizeof(Food));
	if (*f == NULL) {
		return -1;
	}
	(*f)->x1 = rand() % 796;
	(*f)->y1 = rand() % 596;
	return 1;
}

/**
 * @brief This function sets renderer's color to Black for repainting the window
 * @param SDL_Renderer Contains pointer to the Renderer of the window
 */
void set_color_for_clear(SDL_Renderer **renderer) {
	SDL_SetRenderDrawColor(*renderer, 44, 62, 80 , 1);
}

/**
 * @brief This function sets renderer's color to Black for drawing the snake
 * @param SDL_Renderer Contains pointer to the Renderer of the window
 */
void set_color_to_draw(SDL_Renderer **renderer) {
	SDL_SetRenderDrawColor(*renderer, 236, 240, 241 , 1);
}

void render_snake(SDL_Renderer **renderer, RenderingLines *head) {
	RenderingLines *temp = head;
	while (temp != NULL) {
		SDL_RenderDrawLine(*renderer, temp->attr->x1, temp->attr->y1, temp->attr->x2, temp->attr->y2);
		temp = temp->next;
	}

}
/**
 * Calculates the final score
 * @param  head Pointer to the head line
 * @return      returns length of the snake-default length
 */
int calculate_score(RenderingLines *head){
	RenderingLines *temp=head;
	int length=0;
	while(temp!=NULL){
		length += abs(temp->attr->x2 - temp->attr->x1) + abs(temp->attr->y2 - temp->attr->y1);
		temp=temp->next;
	}
	return length-150;
}
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

	init_snake(&head, &last, 100, 200, 100, 350, 0, 1, 0, 1);
	while (condition) {
		if (SDL_PollEvent(&event) > 0) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_LEFT:
					if (lastKey == SDLK_RIGHT) {
						break;
					}
					head = insert_new_line(head, head->attr->x2, head->attr->y2, -1, 0);
					break;
				case SDLK_RIGHT:
					if (lastKey == SDLK_LEFT) {
						break;
					}
					head = insert_new_line(head, head->attr->x2, head->attr->y2, 1, 0);
					break;
				case SDLK_UP:
					if (lastKey == SDLK_DOWN) {
						break;
					}
					head = insert_new_line(head, head->attr->x2, head->attr->y2, 0, -1);
					break;
				case SDLK_DOWN:
					if (lastKey == SDLK_UP) {
						break;
					}
					head = insert_new_line(head, head->attr->x2, head->attr->y2, 0, 1);
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
			if (check_food_collision(get_head_x(head), get_head_y(head), food)) {
				f = NULL;
				head->attr->x2 += (head->attr->x2_increment << 2);
				head->attr->y2 += (head->attr->y2_increment << 2);
			}
		}
		render_snake(&renderer, head);
		SDL_RenderPresent(renderer);
		if (detect_collision(head) == -1) {
			break;
		}
		SDL_Delay(10);
	}
	printf("SCORE=%d\n",calculate_score(head));
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}
