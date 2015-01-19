#define MSG_MOVE 1
#define MSG_LPRESS 2
#define MSG_RPRESS 3
#define MSG_KEYDOWN 4

//鼠标消息
typedef struct MsgMouse
{
	int x;
	int y;
};

//键盘消息
typedef struct MsgKey
{
	char key;
};

typedef struct Msg
{
	int msg_type;//具体见宏定义
	union
	{
		struct MsgMouse msg_mouse;
		struct MsgKey msg_key;
	} concrete_msg;
};

//消息队列结点
typedef struct MsgQueueNode
{
	struct Msg node;
	struct Msg* next;
};

//消息队列
typedef MsgQueueNode* MsgQueue;

//进程消息队列表项
typedef struct MsgTableEntry
{
	int pid;
	struct MsgQueue msg_queue;
};

//系统维护的各进程消息队列列表
typedef MsgTableEntry* MsgTable;

extern void createMsg(int msg_type, int pos_x, int pos_y, char key);

