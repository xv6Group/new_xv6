#include "defs.h"
#include "messages.h"
#include "window.h"

void initMsgQueue()
{
	for(int i = 0; i < MAX_QUEUE_LENGTH; i++)
	{
		MsgQueue[i].msg_type = MSG_UNUSED;
	}
}

void initMsgTable()
{
	for(int i = 0; i < MAX_PROCESS_NUMBER; i++)
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
	for(int i = 0; i < MAX_PROCESS_NUMBER; i++)
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
	for (int i = 0; i < MAX_PROCESS_NUMBER; i++)
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
	}
	else//鼠标事件
	{
		//按照窗口从上到下的顺序检查鼠标事件应该加入哪个进程的消息队列
	}

	dispatch(pid, msg_index);
}

//获得一个进程当前需要处理的msg，并将其从MsgTable和MsgQueue中删除
void getMsg(int pid, struct Msg* ptr)
{
	for(int i = 0; i < MAX_PROCESS_NUMBER; i++)
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
}

//调试
int main()
{
	/*initMsgQueue();
	initMsgTable();
	createMsg(MSG_KEYDOWN,0,1,'a',0);
	createMsg(MSG_LPRESS,0,1,'a',0);
	createMsg(MSG_RPRESS,0,1,'a',0);
	createMsg(MSG_KEYDOWN,0,1,'B',1);
	createMsg(MSG_KEYDOWN,0,1,'C',2);
	createMsg(MSG_KEYDOWN,0,1,'B',1);

	struct Msg temp_ptr;
	getMsg(0, &temp_ptr);
	getMsg(0, &temp_ptr);
	getMsg(0, &temp_ptr);
	getMsg(0, &temp_ptr);*/
	return 0;
}