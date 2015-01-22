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
void drawItem(Context context, char *name, short type, int n);
void drawFinderWnd(Context context);
void drawFinderContent(Context context);
Rect getPos(Context context, int n);//根据文件序号，计算文件所在位置。
int style = 1; //绘制风格
int itemCounter = 0; // 第几个文件

// 事件处理函数
void addItemEvent(ClickableManager *cm, struct fileItem item);
struct fileItem * getFileItem(Point p); //跟据点击位置，获取文件信息

// Handlers
void enterDir(Point p);
void newFile(Point p);
void newFolder(Point p);
void deleteFile(Point p);
void chooseFile(Point p);


// 文件项列表相关操作
void addFileItem(struct stat st, char *name, Rect pos){
    struct fileItem *temp = (struct fileItem *)malloc(sizeof(struct fileItem));
    temp->name = name;
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
}


// 文件信息相关操作
char* fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
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
      addFileItem(st, fmtname(buf), getPos(context, itemCounter));
      //drawItem(context, fmtname(buf), st.type, itemCounter);
      itemCounter ++;
    }
    break;
  }
  close(fd);
}


// 绘图函数相关操作
void drawItem(Context context, char *name, short type, int n)
{
    PICNODE icon;
    Rect rect = getPos(context, n);
    //cprintf("draw finder Item: type=%d counter=%d\n", type, n);
    if (style == ICON_STYLE)
    {
        switch (type)
        {
            case T_FILE:
                loadBitmap(&icon, "file_icon_big.bmp");
                draw_picture(context, icon, rect.start.x, rect.start.y); 
                break;
            case T_DIR:
                loadBitmap(&icon, "folder_icon_big.bmp");
                draw_picture(context, icon, rect.start.x, rect.start.y);
                break;
        }
        puts_str(context, name, 0x0, rect.start.x + 3, rect.start.y + ICON_HEIGHT_BIG + 2);
    }
    else 
    {
        switch (type)
        {
            case T_FILE:
                loadBitmap(&icon, "file_icon_small.bmp");
                draw_picture(context, icon, rect.start.x, rect.start.y);
                break;
            case T_DIR:
                loadBitmap(&icon, "folder_icon_small.bmp");
                draw_picture(context, icon, rect.start.x, rect.start.y);
                break;
        }
        puts_str(context, name, 0x0, rect.start.x + ICON_WIDTH_SMALL + 2, rect.start.y + 2);
    }
    freepic(&icon);
}
    
struct Icon iconlist[] = {
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
    draw_iconlist(context, iconlist, sizeof(iconlist) / sizeof(ICON));
}

void drawFinderContent(Context context)
{
    struct fileItem *pointer;
    pointer = fileItemList;
    int counter = 0;
    while (pointer) 
    {
        drawItem(context, pointer->name, pointer->st.type, counter);
        pointer = pointer->next;
        counter++;
    }
}

Rect getPos(Context context, int n)
{
    if (style == ICON_STYLE)
    {
        int m = context.width / (ICON_ITEM_WIDTH + ICON_ITEM_GAP_X);
        int r = n / m;
        int c = n % m;
        int y_top = r * (ICON_ITEM_HEIGHT + ICON_ITEM_GAP_Y);
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
        createClickable(cm, item.pos, MSG_LPRESS, chooseFile);
        break;
    case T_DIR:
        createClickable(cm, item.pos, MSG_LPRESS, chooseFile);
        createClickable(cm, item.pos, MSG_DOUBLECLICK, enterDir);
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

struct fileItem * getFileItem(Point p)
{
    struct fileItem *temp = (struct fileItem *)malloc(sizeof(struct fileItem));
    return temp;
}


// Handlers
void enterDir(Point p)
{
    struct fileItem *temp = getFileItem(p);
    if(chdir(temp->name) < 0)
      printf(2, "cannot cd %s\n", temp->name);
}

void newFile(Point p)
{
    int fd = open("newfile.txt", 0);
    close(fd);
}

void newFolder(Point p)
{
    struct fileItem *temp = getFileItem(p);
    if(mkdir(temp->name) < 0){
      printf(2, "mkdir: %s failed to create\n", temp->name);
    }
}

void deleteFile(Point p)
{

}

void chooseFile(Point p)
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
    load_iconlist(iconlist, sizeof(iconlist) / sizeof(ICON));
    drawItem(context, "1", T_DIR, 0);
    drawItem(context, "2", T_DIR, 1);
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
            updateWindow(winid, context.addr);
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






