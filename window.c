#include "window.h"
#include "defs.h"
#define MAXWINDOW 20

WindowLink window_list = 0;
WindowLink activated_window = 0;
WindowLink list_head = 0;
WindowLink list_tail = 0;

Window window_array[MAXWINDOW];
int next_window_id = 1;

void initWindowList()
{
	WindowLink p;
	memset(window_array, 0, sizeof(Window) * MAXWINDOW);
}

WindowLink getActivated()
{
	return activated_window;
}

WindowLink allocWindow(int left_x, int left_y, int right_x, int right_y, int pid)
{
	WindowLink p;
	for (p = window_array; p < &window_array[MAXWINDOW]; p++)
	{
		if (p->window_id < 0)
		{
			p->window_id = next_window_id++;
			p->pid = pid;
			(p->window_postion).left_x = left_x;
			(p->window_postion).left_y = left_y;
			(p->window_postion).right_x = right_x;
			(p->window_postion).right_y = right_y;
			if (list_head == 0) list_head = p;
			p->prior_window = list_tail;
			list_tail = p;
			p->next_window = 0;
			return p;
		}
	}
	return 0;
}

int releaseWindow(int window_id)
{
	WindowLink p;
	for (p = list_head; p != NULL; p = p->next_window)
	{
		if (p->window_id == window_id)
		{
			if (p->prior_window != 0) 
				p->prior_window->next_window = p->next_window;
			else
				list_head = p->next_window;
			if (p->next_window != 0) 
				p->next_window->prior_window = p->prior_window;
			else
				list_tail = p->prior_window;
			p->window_id = -1;
			return 0;
		}
	}
	return -1;
}

int inClientRect(WindowLink pWindow, int position_x, int position_y)
{
	return (pWindow->window_postion).left_x <= x &&
		(pWindow->window_postion).right_x >= x &&
		(pWindow->window_postion).left_y <= y &&
		(pWindow->window_postion).right_y >= y ? 1 : 0;
}

