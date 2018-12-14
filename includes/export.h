#ifndef EXPORT_H
#define EXPORT_H
#include "Structures.h"
#include "config.h"

#define GET_X(x,distance)(x+distance>RES_COL-1?x+distance-RES_COL:(x+distance<0?RES_COL+x+distance:x+distance))
#define GET_Y(x,distance)(x+distance>RES_ROW-1?x+distance-RES_ROW:(x+distance<0?RES_ROW+x+distance:x+distance))



extern RenderingLines *head , *last ;
extern Food *f;
extern int lastKey;
extern Command currentCommand;

#endif
