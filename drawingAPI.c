#include "drawingAPI.h"
#include "CHARACTER.h"

/**
*draw_point : if the point is outside the window
*             then do nothing!
*/

void 
draw_point(Context c, unsigned int x, unsigned int y, unsigned short color)
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
fill_rect(Context c, unsigned int bx, unsigned int by, unsigned int width, unsigned int height, unsigned short color)
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

int
draw_character(Context c, unsigned int x, unsigned int y, char ch, unsigned short color)
{
  if(x + CHARACTER_WIDTH > SCREEN_WIDTH)
    return -1;
  if(y + CHARACTER_HEIGHT > SCREEN_HEIGHT)
    return -1;
  int id = ch - 0x20;
  if(id > 94 || id < 0)
    return -1;

  int i, j;
  int tmp = 0;
  int draw_flag = 1;
  i = 0;
  while(draw_flag == 1 && i < CHARACTER_WIDTH)
  {
    //if row >= 4, if there is nothing in the column, then stop drawing.
    if(i >= 4)
    {
      tmp = 0;
      for(j = 0; j < CHARACTER_HEIGHT; j++)
      {
        if(character[id][j][i] != 0)
        {
          tmp = 1;
          break;
        }
      }
      if(tmp == 1)
        draw_flag = 1;
      else
        draw_flag = 0;
    }

    if(draw_flag == 1)
    {
      for(j = 0; j < CHARACTER_HEIGHT; j++)
      {
        if(character[id][j][i] == 0)
          continue;
        else
        {
          draw_point(c, x + i, y + j, color);
        }
      }
    }
    i++;
  }
  return (i);
}

void
draw_string(unsigned int x, unsigned int y, char *str, unsigned short color)
{
  int i;
  int offset_x = 0, offset_y = 0;
  for(i = 0; i < strlen(str); ++i)
  {
    if(x + offset_x + CHARACTER_WIDTH > SCREEN_WIDTH)
    {
      offset_x = 0;
      offset_y += CHARACTER_HEIGHT;
    }
    if(y + offset_y + CHARACTER_HEIGHT > SCREEN_HEIGHT)
      return;
    offset_x += draw_character(x + offset_x, y + offset_y, str[i], color);
  }
}