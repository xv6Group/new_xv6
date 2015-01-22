#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "context.h"
#include "drawingAPI.h"
#include "message.h"
#include "bitmap.h"
#include "finder.h"
#include "windowStyle.h"
#include "clickable.h"
#include "fcntl.h"

#define BUTTON_WIDTH 32
#define BUTTON_HEIGHT 32

#define ICON_STYLE 1
#define LIST_STYLE 2

#define ICON_ITEM_WIDTH 50
#define ICON_ITEM_HEIGHT 70
#define ICON_ITEM_GAP_X 15
#define ICON_ITEM_GAP_Y 15

#define LIST_ITEM_HEIGHT 20

#define ICON_WIDTH_BIG 50
#define ICON_HEIGHT_BIG 50

#define ICON_WIDTH_SMALL 20
#define ICON_HEIGHT_SMALL 20

struct Context context;

// 文件项
struct fileItem{
    struct stat st;
    char *name;
    Rect pos;
    int chosen;
    struct fileItem *next;
};
// 文件项列表，用于保存当前目录下所有文件
struct fileItem *fileItemList = 0;
void addFileItem(struct stat type, char *name, Rect pos);
void freeFileItemList();

// 根据文件目录获取当前目录下所有文件项信息的函数
char* fmtname(char *path);
void list(char *path);

// 绘图函数
void drawItem(Context context, char *name, struct stat st, Rect rect);
void drawFinderWnd(Context context);
void drawFinderContent(Context context);
Rect getPos(Context context, int n);//根据文件序号，计算文件所在位置。
int style = 1; //绘制风格
int itemCounter = 0; // 第几个文件

// 事件处理函数
void addItemEvent(ClickableManager *cm, struct fileItem item);
void addListEvent(ClickableManager *cm);
struct fileItem * getFileItem(Point p); //跟据点击位置，获取文件信息

// Handlers
void h_enterDir(Point p);
void h_newFile(Point p);
void h_newFolder(Point p);
void h_deleteFile(Point p);
void h_chooseFile(Point p);

//测试相关函数
void printItemList();
void testHandlers();

// 文件项列表相关操作
void addFileItem(struct stat st, char *name, Rect pos){
    //int i;
	struct fileItem *temp = (struct fileItem *)malloc(sizeof(struct fileItem));
    temp->name = (char *)malloc(32 * sizeof(char));
    strcpy(temp->name, name);
//    for (i = 0; name[i] != 0; i++)
//    	{
//    		printf(0, "%d : %c\n", i, name[i]);
//    	}
    //printf(0, "copying name\n");
    temp->st = st;
    temp->pos = getPos(context, itemCounter);
    temp->next = fileItemList;
    fileItemList = temp;
}

void freeFileItemList(){
    struct fileItem *p, *temp;
    p = fileItemList;
    while (p != 0)
    {
        temp = p;
        p = p->next;
        free(temp->name);
        free(temp);
    }
    fileItemList = 0;
}


// 文件信息相关操作
char* fmtname(char *path)
{
  //static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}


int containPoint(char *name)
{
    char *p = name;
    while(*p != 0)
    {
        if (*p == '.') return 1;
        p++;
    }
    return 0;
}
void list(char *path)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  itemCounter = 0;
  if((fd = open(path, 0)) < 0){
    printf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    printf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    printf(1, "%s %d %d %d\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf(1, "ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if(stat(buf, &st) < 0){
        printf(1, "ls: cannot stat %s\n", buf);
        continue;
      }
      if (st.type == T_DIR || containPoint(fmtname(buf)))
      {
          addFileItem(st, fmtname(buf), getPos(context, itemCounter));
          itemCounter ++;
      }
    }
    break;
  }
  close(fd);
}


// 绘图函数相关操作
struct Icon contentRes[] = {
        {"file_icon_big.bmp", 0, 0},
        {"file_icon_small.bmp", 0, 0},
        {"folder_icon_big.bmp", 0, 0},
        {"folder_icon_small.bmp", 0, 0},
};
#define FILE_ICON_BIG 0
#define FILE_ICON_SMALL 1
#define FOLDER_ICON_BIG 2
#define FOLDER_ICON_SMALL 3

void drawItem(Context context, char *name, struct stat st, Rect rect)
{
    //cprintf("draw finder Item: type=%d counter=%d\n", type, n);
    if (style == ICON_STYLE)
    {
        switch (st.type)
        {
            case T_FILE:
                draw_picture(context, contentRes[FILE_ICON_BIG].pic, rect.start.x, rect.start.y);
                break;
            case T_DIR:
                draw_picture(context, contentRes[FOLDER_ICON_BIG].pic, rect.start.x, rect.start.y);
                break;
        }
        puts_str(context, name, 0x0, rect.start.x + 3, rect.start.y + ICON_HEIGHT_BIG + 2);
    }
    else 
    {
        switch (st.type)
        {
            case T_FILE:
                draw_picture(context, contentRes[FILE_ICON_SMALL].pic, rect.start.x, rect.start.y);
                break;
            case T_DIR:
                draw_picture(context, contentRes[FOLDER_ICON_SMALL].pic, rect.start.x, rect.start.y);
                break;
        }
        puts_str(context, name, 0x0, rect.start.x + ICON_WIDTH_SMALL + 2, rect.start.y + 2);
    }
}
    
struct Icon wndRes[] = {
    {"close.bmp", 3, 3},
    {"foldericon.bmp", 180, 3},
    {"viewingmode2.bmp", 400 - (BUTTON_WIDTH + 5), TOPBAR_HEIGHT + TOOLSBAR_HEIGHT - (BUTTON_HEIGHT + 3)},
    {"viewingmode1.bmp", 400 - (2 * BUTTON_WIDTH + 6), TOPBAR_HEIGHT + TOOLSBAR_HEIGHT - (BUTTON_HEIGHT + 3)},
    {"createfolder.bmp", 5, TOPBAR_HEIGHT + TOOLSBAR_HEIGHT - (BUTTON_HEIGHT + 3)},
    {"createfile.bmp", (BUTTON_WIDTH + 6), TOPBAR_HEIGHT + TOOLSBAR_HEIGHT - (BUTTON_HEIGHT + 3)}
};

void drawFinderWnd(Context context) {
    fill_rect(context, 0, 0, context.width, context.height, 0xFFFF);

    draw_line(context, 0, 0, context.width - 1, 0, BORDERLINE_COLOR);
    draw_line(context, context.width - 1, 0, context.width - 1, context.height - 1, BORDERLINE_COLOR);
    draw_line(context, context.width - 1, context.height - 1, 0, context.height - 1, BORDERLINE_COLOR);
    draw_line(context, 0, context.height - 1, 0, 0, BORDERLINE_COLOR);
    fill_rect(context, 1, 1, context.width - 2, TOPBAR_HEIGHT + TOOLSBAR_HEIGHT, TOOLSBAR_COLOR);
    puts_str(context, "finder", 0, 200, 3);
    //printf(0, "drawing window\n");
    draw_iconlist(context, wndRes, sizeof(wndRes) / sizeof(ICON));
}


void drawFinderContent(Context context)
{
	struct fileItem *p;
	//printf(0, "listing contents\n");
	fill_rect(context, 0, TOPBAR_HEIGHT + TOOLSBAR_HEIGHT, context.width, context.height - (TOPBAR_HEIGHT + TOOLSBAR_HEIGHT), 0xFFFF);
	freeFileItemList();
	list(".");
	//printf(0, "listing complete!\n");
	//printItemList();
	p = fileItemList;
	itemCounter = 0;
	while (p != 0)
	{
		//printf(0, "draw item\n");
		drawItem(context, p->name, p->st, p->pos);
		p = p->next;
	}
}

void printItemList()
{
    struct fileItem *p;
    p = fileItemList;
    while (p != 0)
    {
        printf(0, "%s\n", p->name);
        p = p->next;
    }
}

Rect getPos(Context context, int n)
{
    if (style == ICON_STYLE)
    {
        int m = context.width / (ICON_ITEM_WIDTH + ICON_ITEM_GAP_X);
        int r = n / m;
        int c = n % m;
        int y_top = r * (ICON_ITEM_HEIGHT + ICON_ITEM_GAP_Y) + TOPBAR_HEIGHT + TOOLSBAR_HEIGHT;
        int x_left = c * (ICON_ITEM_WIDTH + ICON_ITEM_GAP_X);
        return initRect(x_left, y_top, x_left + ICON_ITEM_WIDTH, y_top + ICON_ITEM_HEIGHT);    
    }
    else
    {
        return initRect(0, n * LIST_ITEM_HEIGHT, context.width, (n + 1) * LIST_ITEM_HEIGHT);
    }
}


// 事件处理相关操作
void addItemEvent(ClickableManager *cm, struct fileItem item)
{
	switch(item.st.type)
	{
	case T_FILE:
		break;
	case T_DIR:
		createClickable(cm, item.pos, MSG_LPRESS, h_enterDir);
		break;
	default:
		printf(0, "unknown file type!");
	}
}

void addListEvent(ClickableManager *cm)
{
    struct fileItem *p, *temp;
    p = fileItemList;
    while (p != 0)
    {
        temp = p;
        p = p->next;
        addItemEvent(cm, *temp);
    }
}

struct fileItem * getFileItem(Point point)
{
    struct fileItem *p = fileItemList;
    while (p != 0)
    {
    	if (isIn(point, p->pos))
    	{
    		return p;
    	}
    	p = p->next;
    }
    return 0;
}


// Handlers
void enterDir(char *name)
{
	printf(0, "entering : %s\n", name);
	if(chdir(name) < 0)
		printf(2, "cannot cd %s\n", name);
}

void h_enterDir(Point p)
{
	struct fileItem *temp = getFileItem(p);
	enterDir(temp->name);
}

void cat(int fd)
{
  int n;
  char buf[512];
  while((n = read(fd, buf, sizeof(buf))) > 0)
    write(1, buf, n);
  if(n < 0){
    printf(1, "cat: read error\n");
    exit();
  }
}

void newFile(char *name)
{
	int fd;
	if((fd = open(name, O_CREATE)) < 0){
	      printf(1, "cat: cannot open %s\n", name);
	      exit();
	}
	//cat(fd);
	close(fd);
}

void h_newFile(Point p)
{
	newFile("newfile.txt");
}



void newFolder(char *newfolder)
{
	if(mkdir(newfolder) < 0){
		 printf(0, "mkdir: %s failed to create\n", newfolder);
	}
}

void h_newFolder(Point p)
{
	newFolder("newFolder");
}

void h_deleteFile(Point p)
{

}

void h_chooseFile(Point p)
{
    struct fileItem *temp = getFileItem(p);
    temp->chosen = 1;
}


int main(int argc, char *argv[]) {

    int winid;
    struct Msg msg;
    int isRun = 1;
    Point p;

    ClickableManager cm;
    winid = init_context(&context, 400, 300);
    cm = initClickManager(context);
    load_iconlist(wndRes, sizeof(wndRes) / sizeof(ICON));
    load_iconlist(contentRes, sizeof(contentRes) / sizeof(ICON));
    testHandlers();
    while (isRun) {
        getMsg(&msg);
        switch (msg.msg_type) {
        case MSG_DOUBLECLICK:
            p = initPoint(msg.concrete_msg.msg_mouse.x, msg.concrete_msg.msg_mouse.y);
            executeHandler(cm.double_click, p);
            drawFinderContent(context);
            updateWindow(winid, context.addr);
            break;
        case MSG_UPDATE:
            drawFinderWnd(context);
            drawFinderContent(context);
            deleteClickable(&cm.left_click, initRect(0, 0, 400, 400));
            addListEvent(&cm);
            updateWindow(winid, context.addr);
            break;
        case MSG_PARTIAL_UPDATE:
            updatePartialWindow(winid, context.addr, msg.concrete_msg.msg_partial_update.x1, msg.concrete_msg.msg_partial_update.y1, msg.concrete_msg.msg_partial_update.x2, msg.concrete_msg.msg_partial_update.y2);
            break;
        case MSG_LPRESS:
            p = initPoint(msg.concrete_msg.msg_mouse.x, msg.concrete_msg.msg_mouse.y);
            executeHandler(cm.left_click, p);
            drawFinderContent(context);
            updateWindow(winid, context.addr);
            break;
        case MSG_RPRESS:
            p = initPoint(msg.concrete_msg.msg_mouse.x, msg.concrete_msg.msg_mouse.y);
            executeHandler(cm.right_click, p);
            drawFinderContent(context);
            updateWindow(winid, context.addr);
            break;
        default:
            break;
        }
    }
    free_context(&context, winid);
    exit();
}

void testHandlers()
{
	freeFileItemList();
	list(".");
	printf(0, "original list:\n");
	printItemList();
	printf(0, "\n");
	printf(0, "new a folder:\n");
	newFolder("newfolder");
	freeFileItemList();
	list(".");
	printItemList();
	printf(0, "\n");
	printf(0, "enter new folder:\n");
	enterDir("newfolder");
	freeFileItemList();
	list(".");
	printItemList();
	printf(0, "\n");
	printf(0, "new a file:\n");
	newFile("newfile.txt");
	freeFileItemList();
	list(".");
	printItemList();
	printf(0, "\n");
	printf(0, "enter last folder:\n");
	enterDir("..");
	freeFileItemList();
	list(".");
	printItemList();
}






