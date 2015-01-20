#ifndef DRAWINGAPI_H
#define DRAWINGAPI_H

#define HANKAKU "hankaku.txt"
#define ASCII_NUM 256
#define ASCII_WIDTH 8
#define ASCII_HEIGHT 16
#define ACCII_SIZE 128
#define HZK16 "HZK16.fnt"

struct File_Node
{
	unsigned char * buf;
	int size;
};
struct Context;
void draw_point(struct Context c, unsigned int x, unsigned int y, unsigned short color);
void fill_rect(struct Context c, unsigned int bx, unsigned int by, unsigned int width, unsigned int height, unsigned short color);
void puts_str(struct Context c, char *str, unsigned short colorNum, int x, int y);
#endif
