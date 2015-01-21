#include "types.h"
#include "stat.h"
#include "user.h"
#include "context.h"
#include "drawingAPI.h"
#include "bitmap.h"
#include "message.h"
#include "clickable.h"

void executeShell(Point point);

int main(int argc, char *argv[])
{
    PICNODE pic1, pic2, pic3, pic4;
    int windowId;
    int result;
    //int pid;//, wpid;
    int winid;
    struct Msg msg;
    short isRun = 1;
    //short isInit = 1;
    struct Context context;
    ClickableManager manager;

    winid = init_context(&context, 800, 600);
    manager = initClickManager(context);
    fill_rect(context, 0, 0, context.width, context.height, 0xffff);
    puts_str(context, "desktop: welcome", 0x0, 0, 0);
    loadBitmap(&pic1, "music.bmp");
    loadBitmap(&pic2, "gamecenter.bmp");
    loadBitmap(&pic3, "notes.bmp");
    loadBitmap(&pic4, "setting.bmp");
    draw_picture(context, pic1, 175, 400);
    draw_picture(context, pic2, 300, 400);
    draw_picture(context, pic3, 425, 400);
    draw_picture(context, pic4, 550, 400);
    createClickable(&manager, initRect(175, 400, 75, 75), MSG_DOUBLECLICK, executeShell);
    while(isRun)
    {
        getMsg(&msg);
        switch(msg.msg_type)
        {
            case MSG_UPDATE:
                updateWindow(winid, context.addr);
                printf(0, "desktop");
                /*if (isInit)
                {
                    printf(1, "init shell: starting shell\n");
                    pid = fork();
                    if(pid < 0){
                        printf(1, "init shell: fork failed\n");
                        exit();
                    }
                    if(pid == 0){
                        exec("finder", argv);
                        printf(1, "init shell: exec shell failed\n");
                        exit();
                    }
                    isInit = 0;
                }*/
                break;
            case MSG_DOUBLECLICK:
                executeHandler(manager.double_click, initPoint(msg.concrete_msg.msg_mouse.x, msg.concrete_msg.msg_mouse.y));
            default:
                break;
        }
    }

    windowId = createWindow(0, 0, 800, 600);
    printf(0, "windowId: %d\n", windowId);


    result = updateWindow(windowId, context.addr);
    printf(0, "updateResult: %d\n", result);

    //while(1);
    free_context(&context, winid);
    exit();
}

void executeShell(Point point)
{
    char* argv[] = {};
    printf(1, "init shell: starting shell\n");
    int pid = fork();
    if (pid < 0)
    {
        printf(1, "init shell: fork failed\n");
        exit();
    }
    if (pid == 0) 
    {
        exec("finder", argv);
        printf(1, "init shell: exec shell failed\n");
        exit();
    }
}