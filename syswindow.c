#include "types.h"
#include "param.h"
#include "window.h"
#include "syscall.h"
#include "defs.h"
#include "proc.h"

int sys_createWindow()
{
	int left_x, right_x, left_y, right_y;
	if (argint(0, &left_x) < 0 || argint(1, &left_y) || argint(2, &right_x) || argint(3, &right_y))
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

}