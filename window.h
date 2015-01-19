typedef struct Rect
{
	int left_x;
	int left_y;
	int right_x;
	int right_y;
	//int rect_index;
}Rect;

typedef struct Window
{
	int window_id;
	int activated;//是否激活，1表示激活，0表示未激活，某一时刻只能有一个窗口被激活
	char *window_title;//窗口标题栏字符串
	Rect window_position;
	//int z_order;
	Window *prior_window;
	Window *next_window;
}Window;

typedef Window* WindowLink;//用队列中的先后顺序表示窗口堆叠的次序

extern WindowLink window_queue;