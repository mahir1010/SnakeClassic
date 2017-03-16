#include "RenderSnake.h"

int get_head_x(RenderingLines *head) {
	return head->attr->x2;
}
int get_head_y(RenderingLines *head) {
	return head->attr->y2;
}

void init_attr(Attributes **attr, int x1, int y1, int x2, int y2, int x1_incr, int y1_incr, int x2_incr, int y2_incr) {
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
		if((*attr)->x2==800){
			*head=insert_new_line(*head,0,((*attr)->y2),1,0);
			return 1;
		}
		if((*attr)->x2==0){
			*head=insert_new_line(*head,800,((*attr)->y2),-1,0);
			return 1;
		}
		if((*attr)->y2==600){
			*head=insert_new_line(*head,(*attr)->x2,0,0,1);
			return 1;
		}
		if((*attr)->y2==0){
			*head=insert_new_line(*head,(*attr)->x2,600,0,-1);
			return 1;
		}
	}
	return 1;
}

int init_snake(RenderingLines **head, RenderingLines **last, int x1, int y1, int x2, int y2, int x1_incr, int y1_incr, int x2_incr, int y2_incr) {
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
		return -1;	//returns -1 if memory not allocated
	}
	init_attr(&(*head)->attr, x1, y1, x2, y2, x1_incr, y1_incr, x2_incr, y2_incr); //initializes head line by given values
	(*head)->next = NULL; // sets next element of the linked list to NULL
	(*head)->prev = NULL; // sets previous element of the linked list to NULL
	*last = *head; // As it is the only element of the linked list
	return 0;
}

RenderingLines *insert_new_line(RenderingLines *head, int x1, int y1, int xincr, int yincr) {
	RenderingLines *newLine = (RenderingLines*)malloc(sizeof(RenderingLines)); //Allocates memory to the new line that is to be inserted
	if (newLine == NULL) {
		return NULL;
	}
	newLine->attr = (Attributes*)malloc(sizeof(Attributes)); //Allocates memory to store Attributes of the new line
	if (newLine->attr == NULL) {
		return NULL;
	}
	init_attr(&newLine->attr, x1, y1, x1, y1, 0, 0, xincr, yincr);
	head->prev=newLine;
	newLine->next=head;
	head->attr->y2_increment = 0;
	head->attr->x2_increment = 0;
	return newLine;
}

void remove_last_line(RenderingLines **last) {
	int newXincr = 0, newYincr = 0, delx, dely;
	RenderingLines *temp = *last;
	(*last) = (*last)->prev;
	(*last)->next = NULL;
	delx = (*last)->attr->x2 - (*last)->attr->x1;
	dely = (*last)->attr->y2 - (*last)->attr->y1;
	if (delx == 0) {
		newXincr = 0;
		if (dely > 0) {
			newYincr = 1;
		} else {
			newYincr = -1;
		}
	} else {
		newYincr = 0;
		if (delx > 0) {
			newXincr = 1;
		}
		else {
			newXincr = -1;
		}
	}
	(*last)->attr->x1_increment = newXincr;
	(*last)->attr->y1_increment = newYincr;
	(*last)->attr->x1 += newXincr;
	(*last)->attr->y1 += newYincr;
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

double calculate_distance(int x1, int y1, int x2, int y2) {

	return sqrt(pow(x2 - x1, 2) + (pow(y2 - y1, 2)));
}

int detect_collision(RenderingLines *head) {
	int x = head->attr->x2 + head->attr->x2_increment;
	int y = head->attr->y2 + head->attr->y2_increment;
	RenderingLines *temp = head->next;
	while (temp != NULL) {
		if (calculate_distance(temp->attr->x1, temp->attr->y1, x, y) + calculate_distance(temp->attr->x2, temp->attr->y2, x, y) == calculate_distance(temp->attr->x1, temp->attr->y1, temp->attr->x2, temp->attr->y2)) {
			return -1;
		}
		temp = temp->next;
	}
	return 0;
}