#include "types.h"
#include "stat.h"
#include "user.h"
#include "context.h"
#include "drawingAPI.h"
#include "bitmap.h"
#include "message.h"
#include "vesamode.h"

struct Context context;

int main(int argc, char *argv[])
{

    int windowId;
    int result;
    int pid;//, wpid;
    int winid;
    struct Msg msg;
    short isRun = 1;
    short isInit = 1;

    winid = init_context(&context, 800, 600);
    fill_rect(context, 0, 0, context.width, context.height, 0xffff);
    puts_str(context, "desktop: welcome", 0x0, 0, 0);

    while(isRun)
    {
        getMsg(&msg);
        switch(msg.msg_type)
        {
            case MSG_UPDATE:
                updateWindow(winid, context.addr);
                printf(0, "desktop");
                if (isInit)
                {
                    printf(1, "init shell: starting shell\n");
                    pid = fork();
                    if(pid < 0){
                        printf(1, "init shell: fork failed\n");
                        exit();
                    }
                    if(pid == 0){
                        exec("shell", argv);
                        printf(1, "init shell: exec shell failed\n");
                        exit();
                    }
                    isInit = 0;
                }
                break;
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
