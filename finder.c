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
    struct fileItem *next;
};
// 文件项列表，用于保存当前目录下所有文件
struct fileItem *fileItemList;
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

// 事件处理函数
void addEvent(char *name, short type);
struct fileItem getFileItem(Point p); //跟据点击位置，获取文件信息

// Handlers
void enterDir(Point p);
void newFile(Point p);
void newFolder(Point p);
void deleteFile(Point p);
void chooseFile(Point p);


// 文件项列表相关操作
void addFileItem(struct stat type, char *name, Rect pos){

}

void freeFileItemList(){

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

int itemCounter = 0;

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
}
    

void drawFinderWnd(Context context) {
    PICNODE close, folder, vm1, vm2, createfolder, createfile, up;
    fill_rect(context, 0, 0, context.width, context.height, 0xFFFF);

    draw_line(context, 0, 0, context.width - 1, 0, BORDERLINE_COLOR);
    draw_line(context, context.width - 1, 0, context.width - 1, context.height - 1, BORDERLINE_COLOR);
    draw_line(context, context.width - 1, context.height - 1, 0, context.height - 1, BORDERLINE_COLOR);
    draw_line(context, 0, context.height - 1, 0, 0, BORDERLINE_COLOR);
    fill_rect(context, 1, 1, context.width - 2, TOPBAR_HEIGHT + TOOLSBAR_HEIGHT, TOOLSBAR_COLOR);

    loadBitmap(&close, "close.bmp");
    draw_picture(context, close, 3, 3);

    loadBitmap(&folder, "foldericon.bmp");
    draw_picture(context, folder, context.width / 2 - 20, 3);
    puts_str(context, "Finder", 0x0, context.width / 2 + 2, 3);

    //printf(0, "loading viewingmode2.bmp\n");
    loadBitmap(&vm2, "viewingmode2.bmp");
    draw_picture(context, vm2, context.width - (BUTTON_WIDTH + 5), TOPBAR_HEIGHT + TOOLSBAR_HEIGHT - (BUTTON_HEIGHT + 3));

    //printf(0, "loading viewingmode1.bmp\n");
    loadBitmap(&vm1, "viewingmode1.bmp");
    //printf(0, "loading viewingmode1.bmp complete!\n");
    draw_picture(context, vm1, context.width - (2 * BUTTON_WIDTH + 6), TOPBAR_HEIGHT + TOOLSBAR_HEIGHT - (BUTTON_HEIGHT + 3));

    //printf(0, "loading createfolder.bmp\n");
    loadBitmap(&createfolder, "createfolder.bmp");
    draw_picture(context, createfolder, 5, TOPBAR_HEIGHT + TOOLSBAR_HEIGHT - (BUTTON_HEIGHT + 3));

    //printf(0, "loading createfile.bmp\n");
    loadBitmap(&createfile, "createfile.bmp");
    draw_picture(context, createfile, (BUTTON_WIDTH + 6), TOPBAR_HEIGHT + TOOLSBAR_HEIGHT - (BUTTON_HEIGHT + 3));

    freepic(&close);
    freepic(&folder);
    freepic(&vm1);
    freepic(&vm2);
    freepic(&createfolder);
    freepic(&createfile);
    freepic(&up);
}

void drawFinderContent(Context context)
{

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
void addEvent(char *name, short type)
{

}

struct fileItem getFileItem(Point p)
{
    struct fileItem temp;
    return temp;
}


// Handlers
void enterDir(Point p)
{

}

void newFile(Point p)
{

}

void newFolder(Point p)
{

}

void deleteFile(Point p)
{

}

void chooseFile(Point p)
{

}


int main(int argc, char *argv[]) {

    int winid;
    struct Msg msg;
    int isRun = 1;
    Point p;

    ClickableManager cm;
    winid = init_context(&context, 400, 300);
    cm = initClickManager(context);

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






