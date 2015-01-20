#include "context.h"
#include "drawingAPI.h"
#include "defs.h"
#include "user.h"

/**
*draw_point : if the point is outside the window
*             then do nothing!
*/

void 
draw_point(struct Context c, unsigned int x, unsigned int y, unsigned short color)
{
  if(x >= c.width) 
    return;
  if(y >= c.height)
    return;
  c.addr[y*c.width+x] = color;
}

/*
*fill_rect: set a rect area with a certain color
*/
void
fill_rect(struct Context c, unsigned int bx, unsigned int by, unsigned int width, unsigned int height, unsigned short color)
{
	int x, y;
	int mx = c.width < bx + width ? c.width : bx + width;
	int my = c.height < by + height ? c.height : by + height; 
	for (y = by; y < my; y++)
	{
		for (x = bx; x < mx; x++)
		{
			draw_point(c, x, y, color);
		}
	}
}

char buf[512];
//hankaku是一个数组，将hankaku.txt文件中的每一行转化成一个8位整数（unsigned short）
//每16个整数可以代表一个字符
unsigned char *hankaku;
void initializeHankaku()
{

	int fd, n, i, counter;
	int nowChar, x, y;
	cprintf("initialzing Hankaku");
	//打开hankaku.txt文件
	if((fd = open(HANKAKU, 0)) < 0){
	  cprintf("cannot open %s\n", HANKAKU);
	  return;
	}
	//申请hankaku数组
	hankaku = malloc(ASCII_NUM*sizeof(unsigned char));
	for (i = 0; i < ASCII_NUM; i++)
	{
		hankaku[i] = 0;
	}

	//不断读取文件，如果读到的字符是“*/."，就按顺序记录到hankaku数组中
	//y表示当前记录到第几行（对应于hankaku数组里的第几个数），x表示当前记录到第几列
	//如果当前字符是"*",则对应第y个数第x位置为1
	//每当x == ASCII_WIDTH，x重新置为0, y++
	x = 0;
	y = 0;
	while((n = read(fd, buf, sizeof(buf))) > 0)
	{
		for (i = 0; i < n; i++){
			if (buf[i] == '*' || buf[i] == '.')
			{
				if (buf[i] == '*')
				{
					hankaku[y] |= (0x80 >> x);
				}
				x ++;
				if (counter >= ASCII_WIDTH)
				{
					x = 0;
					y ++;
				}
			}
		}
	}
	cprintf("initialzing Hankaku complete!");
}

struct File_Node fontFile;
void initializeFontFile(){
	int fd;
	cprintf("initialzing FontFile");
	if((fd = open(HANKAKU, 0)) < 0){
		cprintf("cannot open %s\n", HZK16);
		return;
	}
	fontFile.buf = malloc(26624*sizeof(unsigned char));
	fontFile.size = read(fd, fontFile.buf, 26624);
	cprintf("initialzing FontFile complete!");
}

void put_ascii(struct Context c, unsigned char ascii, unsigned short colorNum, int x, int y)
{
	int tmpX, tmpY;

	for(tmpY = y; tmpY < y + 16; tmpY++) {
		for(tmpX = 0; tmpX < 8; tmpX++) {
			if((((hankaku + (ascii * 16))[tmpY - y] << tmpX) & 0x80) == 0x80) {
				draw_point(c, x + tmpX, tmpY, colorNum);
				//sheet->buf[tmpY * sheet->width + x + tmpX] = colorNum;
			}
		}
	}
}

void put_gbk(struct Context c, unsigned char gbk, unsigned short colorNum, int x, int y)
{
	int tmpX, tmpY;
	unsigned int offset;
	unsigned int *hzk16Base;

	if((gbk & 0x00FF) >= 0xA1 && ((gbk >> 8) & 0x00FF) >= 0xA1) {
		hzk16Base = fontFile.buf;
		offset = (((gbk & 0x00FF) - 0xa1) * 94 + (((gbk >> 8) & 0x00FF) - 0xa1)) * 32;

		for(tmpY = y; tmpY < 16 + y; tmpY++) {
			for(tmpX = 0; tmpX < 8; tmpX++) {
				if(((hzk16Base[offset] << tmpX) & 0x80) == 0x80) {
					draw_point(c, x + tmpX, tmpY, colorNum);
					//sheet->buf[tmpY * sheet->width + x + tmpX] = colorNum;
				}
			}
			offset++;
			for(tmpX = 0; tmpX < 8; tmpX++) {
				if(((hzk16Base[offset] << tmpX) & 0x80) == 0x80) {
					draw_point(c, x + tmpX, tmpY, colorNum);
					//sheet->buf[tmpY * sheet->width + x + tmpX + 8] = colorNum;
				}
			}
			offset++;
		}
	}
	else {
		put_ascii(c, (gbk & 0x00FF), colorNum, x, y);
		put_ascii(c, ((gbk >> 8) & 0x00FF), colorNum, x + 8, y);
	}
}

void puts_str(struct Context c, unsigned char *str, unsigned short colorNum, int x, int y)
{
	int i = 0, rowLetterCnt, xTmp;
	short wStr;

	rowLetterCnt = strlen(str);
	for(i = 0, xTmp = x; i < rowLetterCnt;) {
		if(str[i] < 0xA1) {
			put_ascii(c, str[i], colorNum, xTmp, y);
			xTmp += 8;
			i++;
		}
		else {
			wStr = (str[i] & 0x00FF) | ((((short)str[i + 1]) << 8) & 0xFF00);
			put_gbk(c, wStr, colorNum, xTmp, y);
			xTmp += 16;
			i += 2;
		}
	}
}

