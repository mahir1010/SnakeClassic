#ifndef EXPORT_H
#define EXPORT_H
#include "Structures.h"
#include "config.h"


#define LEFT 0x0
#define RIGHT 0x1
#define UP 0x2
#define DOWN 0x3
#define NOOP 0x4

extern RenderingLines *head , *last ;
extern Food *f;
extern int lastKey;
typedef char Command;

extern Command currentCommand;
#endif
