#ifndef STRUCTURES_H
#define STRUCTURES_H
/**
* @brief The basic structure to store
* x and y co-ordinates of the end points
* of lines along with their increment
* values.
* @var x1 stores the value of x co-ordinate of starting point
* @var y1 stores the value of y co-ordinate of starting point
* @var x2 stores the value of x co-ordinate of ending point
* @var y2 stores the value of y co-ordinate of ending point
* @var x1_increment stores the value by which the x1 point should be incremented on each iteration
* @var y1_increment stores the value by which the y1 point should be incremented on each iteration
* @var x2_increment stores the value by which the x2 point should be incremented on each iteration
* @var y2_increment stores the value by which the y2 point should be incremented on each iteration
*/
typedef struct line_attr
{
	int x1;
	int y1;
	int x2;
	int y2;
	int x1_increment;
	int y1_increment;
	int x2_increment;
	int y2_increment;
} Attributes;

/**
* @brief Single variable represents
* a line forming the body of the
* Snake.
* @var attr To store co-ordinates and increment value of the line
* @var next Points to the next line in the Linked List
* @var prev Points to the previous line in the Linked List
*/

typedef struct node {
	Attributes *attr;
	struct node *next;
	struct node *prev;
} RenderingLines;

/**
 * @var x1 Stores X coordinate of the food
 * @var y1 Stores Y coordinate of the food
 */
typedef struct foodAttr {
	int x1;
	int y1;
} Food;

#endif