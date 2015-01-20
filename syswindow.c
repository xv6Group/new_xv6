#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "window.h"

int sys_createWindow()
{
	int left_x, right_x, left_y, right_y;
	if (argint(0, &left_x) < 0 || argint(1, &left_y) < 0 || argint(2, &right_x) < 0 || argint(3, &right_y) < 0)
		return -1;
	return allocWindow(left_x, left_y, right_x, right_y, proc->pid)->window_id;
}

int sys_destroyWindow()
{
	int window_id;
	if (argint(0, &window_id) < 0)
		return -1;
	return releaseWindow(window_id);
}

int sys_updateWindow()
{
	int window_id;
	color16* context;
	WindowLink pWindow;
	cprintf("updateWindow called");
	if (argint(0, &window_id) < 0)
		return -1;
	pWindow = getWindowById(window_id);
	if (pWindow == 0) return -1;
	int size = ((pWindow->window_position).right_x - (pWindow->window_position).left_x) *
		((pWindow->window_position).right_y - (pWindow->window_position).left_y);
	cprintf("window size: %d\n", size);
	if (size < 0) return -1;
	if (argptr(1, (void*)&context, sizeof(color16) * size) < 0) return -1;
	drawWindow(pWindow, context);
	return 0;
}