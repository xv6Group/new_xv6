#include "types.h"
#include "defs.h"
#include "param.h"
#include "mmu.h"
#include "proc.h"
#include "message.h"
#include "window.h"

//全局消息队列
struct Msg MsgQueue[MAX_QUEUE_LENGTH];

//系统维护的各进程消息队列列表
struct MsgTableEntry MsgTable[MAX_PROCESS_NUMBER];

void msgqueueinit()
{
	int i;
	for(i = 0; i < MAX_QUEUE_LENGTH; i++)
	{
		MsgQueue[i].msg_type = MSG_UNUSED;
	}
}

void msgtableinit()
{
	int i;
	for(i = 0; i < MAX_PROCESS_NUMBER; i++)
	{
		MsgTable[i].pid = -1;
	}
}

//在MsgQueue中分配一个msg
int requireMsg(int msg_type, int pos_x, int pos_y, char key)
{
	int i;
	for(i = 0; i < MAX_QUEUE_LENGTH; i++)
	{
		if(MsgQueue[i].msg_type == MSG_UNUSED)
		{
			MsgQueue[i].msg_type = msg_type;
			if(msg_type == MSG_KEYDOWN)
			{
				MsgQueue[i].concrete_msg.msg_key.key = key;
			}
			else if(msg_type == MSG_DRAG)
			{

			}
			else
			{
				MsgQueue[i].concrete_msg.msg_mouse.x = pos_x;
				MsgQueue[i].concrete_msg.msg_mouse.y = pos_y;
			}
			return i;
		}
	}
	return -1;
}

//将msg加入MsgTable中对应的pid项
void dispatch(int pid, int msg_index)
{
	int i;
	for(i = 0; i < MAX_PROCESS_NUMBER; i++)
	{
		if(MsgTable[i].pid == pid)
		{
			int temp_msg_index = MsgTable[i].first_msg;
			while(MsgQueue[temp_msg_index].next_msg != -1)
			{
				temp_msg_index = MsgQueue[temp_msg_index].next_msg;
			}
			MsgQueue[temp_msg_index].next_msg = msg_index;
			MsgQueue[msg_index].next_msg = -1;
			return;
		}
	}
	for (i = 0; i < MAX_PROCESS_NUMBER; i++)
	{
		if(MsgTable[i].pid == -1)
		{
			MsgTable[i].pid = pid;
			MsgTable[i].first_msg = msg_index;
			MsgQueue[msg_index].next_msg = -1;
			break;
		}
	}
}

//创建一则消息，包括将其加入MsgTable和MsgQueue中
void createMsg(int msg_type, int pos_x, int pos_y, char key)
{
	int msg_index = requireMsg(msg_type, pos_x, pos_y, key);
	if (msg_index == -1) return;

	int pid;
	if(msg_type == MSG_KEYDOWN)//键盘事件
	{
		pid = getActivated()->pid;
        cprintf("key pressed!  pid:%d\n", pid);
	}
	else//鼠标事件
	{
        struct Window* win_ptr = getWindowByPoint(pos_x, pos_y);
        cprintf("%d\n", win_ptr);
        pid = win_ptr->pid;
        int relative_x = pos_x - win_ptr->window_position.left_x;
        int relative_y = pos_y - win_ptr->window_position.left_y;
        cprintf("relative x: %d\n", relative_x);
        cprintf("relative y: %d\n", relative_y);
        MsgQueue[msg_index].concrete_msg.msg_mouse.x = relative_x;
        MsgQueue[msg_index].concrete_msg.msg_mouse.y = relative_y;
		drawMouse(pos_x, pos_y);
	}

	dispatch(pid, msg_index);
}

void createUpdateMsg(int pid)
{
	int msg_index = requireMsg(MSG_UPDATE, 0, 0, ' ');
	if (msg_index == -1) return;
	
	dispatch(pid, msg_index);
}

//系统调用，获得一个进程当前需要处理的msg，并将其从MsgTable和MsgQueue中删除
void getMsg(int pid, struct Msg* ptr)
{
	int i;
	for(i = 0; i < MAX_PROCESS_NUMBER; i++)
	{
		if(MsgTable[i].pid == pid)
		{
			int msg_index = MsgTable[i].first_msg;
			ptr->msg_type = MsgQueue[msg_index].msg_type;
			ptr->concrete_msg = MsgQueue[msg_index].concrete_msg;
			if(MsgQueue[msg_index].next_msg == -1)
			{
				MsgTable[i].pid = -1;
			}
			else
			{
				MsgTable[i].first_msg = MsgQueue[msg_index].next_msg;
			}
			MsgQueue[msg_index].msg_type = MSG_UNUSED;
			return;
		}
	}
    ptr->msg_type = MSG_NONE;
}

int sys_getMsg()
{
	struct Msg* ptr;
	if (argptr(0, (void*)&ptr, sizeof(*ptr)) < 0)
		return -1;
	getMsg(proc->pid, ptr);
	return 0;
}
