#define MSG_NONE 0
#define MSG_UNUSED 0
#define MSG_MOVE 1
#define MSG_LPRESS 2
#define MSG_RPRESS 3
#define MSG_DOUBLECLICK 4
#define MSG_DRAG 5
#define MSG_KEYDOWN 6
#define MSG_UPDATE 7
#define MAX_PROCESS_NUMBER 20
#define MAX_QUEUE_LENGTH 100

//鼠标消息
typedef struct MsgMouse
{
	int x;
	int y;
} MsgMouse;

//键盘消息
typedef struct MsgKey
{
	char key;
} MsgKey;

typedef struct Msg
{
	int msg_type;
	union
	{
		struct MsgMouse msg_mouse;
		struct MsgKey msg_key;
	} concrete_msg;
	int next_msg;//下一个消息在全局消息队列中的索引
} Msg;

//进程消息队列表项
typedef struct MsgTableEntry
{
	int pid;
	int first_msg;//待处理的第一个消息在全局消息队列中的索引
} MsgTableEntry;

extern void createMsg(int msg_type, int pos_x, int pos_y, char key);
extern void msgqueueinit();
extern void msgtableinit();
extern void createUpdateMsg(int pid);
