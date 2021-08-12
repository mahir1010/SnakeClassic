#include "./includes/Snake.h"

void init_attributes(Attributes **attr, int x1, int y1, int x2, int y2, int x1_incr, int y1_incr, int x2_incr, int y2_incr) {
	(*attr)->x1 = x1;
	(*attr)->y1 = y1;
	(*attr)->x2 = x2;
	(*attr)->y2 = y2;
	(*attr)->x1_increment = x1_incr;
	(*attr)->y1_increment = y1_incr;
	(*attr)->x2_increment = x2_incr;
	(*attr)->y2_increment = y2_incr;
}

int update_attributes(RenderingLines **head,Attributes **attr, int condition) {
	if (condition && (*attr)->x1 == (*attr)->x2 && (*attr)->y1 == (*attr)->y2) {
		return -1;
	}
	(*attr)->x1 += (*attr)->x1_increment;
	(*attr)->y1 += (*attr)->y1_increment;
	(*attr)->x2 += (*attr)->x2_increment;
	(*attr)->y2 += (*attr)->y2_increment;
	if(!condition){
		if((*attr)->x1>RES_COL-1){
			*head=insert_new_line_crossed(*head,0,((*attr)->y1),1,0);
			return 1;
		}
		if((*attr)->x1<0){
			*head=insert_new_line_crossed(*head,RES_COL-1,((*attr)->y1),-1,0);
			return 1;
		}
		if((*attr)->y1>RES_ROW-1){
			*head=insert_new_line_crossed(*head,(*attr)->x1,0,0,1);
			return 1;
		}
		if((*attr)->y1<0){
			*head=insert_new_line_crossed(*head,(*attr)->x1,RES_ROW-1,0,-1);
			return 1;
		}
	}
	return 1;
}

int init_snake(RenderingLines **head, RenderingLines **last, int x1, int y1, int x2, int y2) {
	int x1_incr=0,y1_incr=0,x2_incr=0,y2_incr=0;
	srand(time(NULL));
	if (*head != NULL) {	//checks whether head is already initialized or not
		return 1;
	}
	(*head) = (RenderingLines*)malloc(sizeof(RenderingLines)); //Allocate memory to head line of the body
	if ((*head) == NULL) {
		return -1;	//returns -1 if memory not allocated
	}
	(*head)->attr = (Attributes*)malloc(sizeof(Attributes)); //Allocate memory to store attributes of head line.
	if ((*head)->attr == NULL) {
		free(*head);
		return -1;	//returns -1 if memory not allocated
	}
	if(x1-x2>0) {
		x1_incr=1;
		x2_incr=1;
	}else if(x1-x2<0){
		x1_incr=-1;
		x2_incr=-1;
	}
	if(y1-y2>0){
		y1_incr=1;
		y2_incr=1;
	}
	else if(y1-y2<0){
		y1_incr=-1;
		y2_incr=-1;
	}
	init_attributes(&(*head)->attr, x1, y1, x2, y2, x1_incr, y1_incr, x2_incr, y2_incr); //initializes head line by given values
	(*head)->next = NULL; // sets next element of the linked list to NULL
	(*head)->prev = NULL; // sets previous element of the linked list to NULL
	*last = *head; // As it is the only element of the linked list
	return 0;
}

RenderingLines *insert_new_line(RenderingLines *head,int x1,int y1, int xincr, int yincr) {
	RenderingLines *newLine = (RenderingLines*)malloc(sizeof(RenderingLines)); //Allocates memory to the new line that is to be inserted

	if (newLine == NULL) {
		return NULL;
	}
	newLine->attr = (Attributes*)malloc(sizeof(Attributes)); //Allocates memory to store Attributes of the new line
	if (newLine->attr == NULL) {
		free(newLine);
		return NULL;
	}
	init_attributes(&newLine->attr,x1, y1, x1+xincr, y1+yincr, xincr, yincr, 0, 0);
	head->prev=newLine;
	newLine->next=head;
	head->attr->x1_increment = 0;
	head->attr->y1_increment = 0;
	return newLine;
}

RenderingLines *insert_new_line_crossed(RenderingLines *head,int x1,int y1, int xincr, int yincr) {
	RenderingLines *newLine = (RenderingLines*)malloc(sizeof(RenderingLines)); //Allocates memory to the new line that is to be inserted

	if (newLine == NULL) {
		return NULL;
	}
	newLine->attr = (Attributes*)malloc(sizeof(Attributes)); //Allocates memory to store Attributes of the new line
	if (newLine->attr == NULL) {
		free(newLine);
		return NULL;
	}
	init_attributes(&newLine->attr,x1, y1, x1, y1, xincr, yincr, 0, 0);
	head->prev=newLine;
	newLine->next=head;
	head->attr->x1_increment = 0;
	head->attr->y1_increment = 0;
	return newLine;
}
void remove_last_line(RenderingLines **last) {
	int newXincr = 0, newYincr = 0, delx, dely;
	RenderingLines *temp = *last;
	(*last) = (*last)->prev;
	(*last)->next = NULL;
	delx = (*last)->attr->x1 - (*last)->attr->x2;
	dely = (*last)->attr->y1 - (*last)->attr->y2;
	if(delx>0){
		newXincr=1;
	}else if(delx<0){
		newXincr=-1;
	}
	if(dely>0){
		newYincr=1;
	}
	if(dely<0){
		newYincr=-1;
	}
	(*last)->attr->x2_increment = newXincr;
	(*last)->attr->y2_increment = newYincr;
	free(temp->attr);
	free(temp);
}
void update_lines(RenderingLines **head, RenderingLines **last) {

	update_attributes(head,&(*head)->attr, 0);
	if ((*head)->next == NULL) {
		return;
	}
	if (update_attributes(head,&(*last)->attr, 1) == -1) {
		remove_last_line(last);
	}

}

float calculate_distance(int x1, int y1, int x2, int y2) {

	return sqrt(pow(x2 - x1, 2) + (pow(y2 - y1, 2)));
}

int detect_collision(RenderingLines *head) {
	int x = head->attr->x1;
	int y = head->attr->y1;
	RenderingLines *temp = head->next;
	while (temp != NULL) {
		if (calculate_distance(temp->attr->x1, temp->attr->y1, x, y) + calculate_distance(temp->attr->x2, temp->attr->y2, x, y) == calculate_distance(temp->attr->x1, temp->attr->y1, temp->attr->x2, temp->attr->y2)) {
			return -1;
		}
		temp = temp->next;
	}
	return 0;
}

void set_color_for_clear(SDL_Renderer **renderer) {
	SDL_SetRenderDrawColor(*renderer, 44, 62, 80 , 1);
}

void set_color_to_draw(SDL_Renderer **renderer) {
	SDL_SetRenderDrawColor(*renderer, 236, 240, 241 , 1);
}

void render_snake(SDL_Renderer **renderer, RenderingLines *head) {
	RenderingLines *temp = head;
	SDL_SetRenderDrawColor(*renderer,255,0,0,1);
	SDL_RenderDrawLine(*renderer, temp->attr->x1, temp->attr->y1, temp->attr->x2, temp->attr->y2);
	temp=temp->next;
	set_color_to_draw(renderer);
	while (temp != NULL) {
		SDL_RenderDrawLine(*renderer, temp->attr->x1, temp->attr->y1, temp->attr->x2, temp->attr->y2);
		temp = temp->next;
	}
}

int check_food_collision(int x, int y, SDL_Rect food) {
	return (x <= food.x + food.w && x >= food.x) && (y <= food.y + food.h && y >= food.y);
}

int create_food(Food **f) {
	*f = (Food*)malloc(sizeof(Food));
	if (*f == NULL) {
		return -1;
	}
	(*f)->x1 = (rand() % (RES_COL-4))+1;
	(*f)->y1 = (rand() % (RES_ROW-4))+1;
	return 1;
}


double calculate_score(RenderingLines *head){
	RenderingLines *temp=head;
	double length=0;
	while(temp!=NULL){
		length += calculate_distance(temp->attr->x1,temp->attr->y1,temp->attr->x2,temp->attr->y2);
		temp=temp->next;
	}	
	return length-calculate_distance(INIT_SNAKE_X1,INIT_SNAKE_Y1,INIT_SNAKE_X2,INIT_SNAKE_Y2);
}