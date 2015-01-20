#ifndef DRAWINGAPI_H
#define DRAWINGAPI_H

struct Context;
void draw_point(struct Context c, unsigned int x, unsigned int y, unsigned short color);
void fill_rect(struct Context c, unsigned int bx, unsigned int by, unsigned int width, unsigned int height, unsigned short color);

#endif