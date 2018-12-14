/**
 * @file RenderSnake.h
 * @brief Defines Data structures to store
 * Rendering infromation for Snake.
 * Contains various functions to manipulate and render
 * lines forming the snake
 * @author Mahir Patel
 * @bug No known bugs.
 */

#ifndef RENDER_SNAKE_H
#define RENDER_SNAKE_H

#include "SDL2/SDL.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Structures.h"
#include "./config.h"


/**
 * @brief This function initializes input Attribute variable with given parameter values
 * @param attr The Attribute variable which needs to be initialized
 * @param x1 x1 stores the value of x co-ordinate of starting point
 * @param y1 stores the value of y co-ordinate of starting point
 * @param x2 stores the value of x co-ordinate of ending point
 * @param y2 stores the value of y co-ordinate of ending point
 * @param x1_incr stores the value by which the x1 point should be incremented on each iteration
 * @param y1_incr stores the value by which the y1 point should be incremented on each iteration
 * @param x2_incr stores the value by which the x2 point should be incremented on each iteration
 * @param y2_incr stores the value by which the y2 point should be incremented on each iteration
 */
void init_attributes(Attributes **attr, int x1, int y1, int x2, int y2, int x1_incr, int y1_incr, int x2_incr, int y2_incr);

/**
 * @brief This function updates the co-ordinates of the line to create animation on every main loop iteration
 * @param head Contains pointer to the head of the body
 * @param attr Target Attribute variable which needs to be updated
 * @param condition This variable instructs function whether to consider returning condition valid or not
 * @return return -1 if the values of x and y co-ordinates of starting and ending point of the line are same
 */
int update_attributes(RenderingLines **head,Attributes **attr, int condition);

/**
 * @brief This function creates initial body of the snake
 * @param head Contains pointer to the first line of the snake
 * @param last Contains pointer to the last line of the snake
 * @param x1 x1 stores the value of x co-ordinate of starting point
 * @param y1 stores the value of y co-ordinate of starting point
 * @param x2 stores the value of x co-ordinate of end point
 * @param y2 stores the value of y co-ordinate of end point
 * @return returns -1 if head is not allocated
 */
int init_snake(RenderingLines **head, RenderingLines **last, int x1, int y1, int x2, int y2);

/**
 * @brief This fuctio Adds new line to the body of the snake
 * This function is called when user presses a valid key and
 * snake is required to turn in respective direction.
 * @param head points to the first line of the body of snake
 * @param x1 x co-ordinate of the point from which the snake changes direction
 * @param y1 y co-ordinate of the point from which the snake changes direction
 * @param xincr contains new value for x2_increment
 * @param yincr contains new value for y2_increment
 * @return updated head value
 *
 * xincr 	yincr  		Direction
 *  -1		0		Left
 *   1 		0		Right
 *   0	       -1		Up
 *   0		1		Down
 */
RenderingLines *insert_new_line(RenderingLines *head,int x1,int y1, int xincr, int yincr);

/**
 * @brief This function is used to remove last line when length of the line is zero.
 * This function calculates dx(x2-x1) and dy(y2-y1) to calculate new x1-increment and y1-increment value.
 * 	|dx|		|dy|		x1-increment 			y1-increment
 * 	 =0		 >0			0				1
 * 	 =0		 <0			0			       -1
 * 	 >0		 =0			1				0
 * 	 <0		 =0		       -1				0
 * @param last Contains pointer to the last line of the snake's body which is to be removed
 */
void remove_last_line(RenderingLines **last);
/**
 * @brief This function is used to update head and last line.
 * @param head Contains pointer to the head line of the Snake
 * @param last Contains pointer to the last line of the Snake
 */
void update_lines(RenderingLines **head, RenderingLines **last);
/**
 * @brief This function calculated distance between two points via following formula
 * 	Distance= SQRT((x2-x1)^2+(y2-y1)^2)
 * @param x1 x co-ordinate of first point
 * @param y1 y co-ordinate of first point
 * @param x2 x co-ordinate of second point
 * @param y2 y co-ordinate of second point
 * @return Distance between two points
 */
float calculate_distance(int x1, int y1, int x2, int y2);

/**
 * @brief This function calculates whether the end point of the head line is intersecting with other lines of the body
 * This is calculated by following method:
 * let the end point of head be A(x0,y0) and the starting and ending point for line to be checked are B(x1,y1) and C(x2,y2)
 * if Distance(B,A)+Distance(A,C)=Distance(B,C) then collision has occured.
 * @param
 * @return
 */
int detect_collision(RenderingLines *head);

/**
 * @brief This function sets renderer's color for repainting the window
 * @param SDL_Renderer Contains pointer to the Renderer of the window
 */
void set_color_for_clear(SDL_Renderer **renderer);

/**
 * @brief This function sets renderer's color for drawing the snake
 * @param SDL_Renderer Contains pointer to the Renderer of the window
 */
void set_color_to_draw(SDL_Renderer **renderer);

/**
 * @brief This function renders the snake onto the screen.
 * @param SDL_Renderer Contains pointer to the Renderer of the window
 * @param RenderingLines Contains pointer to the head of the linked list
 */
void render_snake(SDL_Renderer **renderer, RenderingLines *head);

/*
 * This function checks whether head has collided with food or not
 * @param  x    x co-ordinate of head
 * @param  y    y co-ordinate of head
 * @param  food SDL_Rect containing food
 * @return      returns 1 if condition true or 0 if condition false
 */
int check_food_collision(int x, int y, SDL_Rect food);
/**
 * Creates food on random Points
 * @param  f Pointer structure storing Attributes of Food
 * @return   return -1 if memory not allocated
 */
int create_food(Food **f);


/**
 * Calculates the final score
 * @param  head Pointer to the head line
 * @return      returns length of the snake-default length
 */
double calculate_score(RenderingLines *head);

#endif // RENDER_SNAKE_H
