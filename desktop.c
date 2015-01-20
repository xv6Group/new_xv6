#include "types.h"
#include "stat.h"
#include "user.h"
#include "context.h"
#include "drawingAPI.h"
#include "message.h"

struct Context context;

int main(int argc, char *argv[])
{
    //int pid, wpid;

    init_context(&context, 800, 600); 
    fill_rect(context, 0, 0, context.width, context.height, 2016);
    int windowId;
    int result;
    int running = 1;
    /*
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

    while((wpid=wait()) >= 0 && wpid != pid)
        printf(1, "shell finished!\n");
    */
    windowId = createWindow(0, 0, 800, 600);
    printf(0, "windowId: %d\n", windowId);
    while(running)
    {
        Msg msg;
        msg.msg_type = MSG_NONE;
        getMsg(&msg);
        switch(msg.msg_type)
        {
            case MSG_UPDATE:
                result = updateWindow(windowId, context.addr);
                printf(0, "updateResult: %d\n", result);
                break;
            default:
                break;
        }
    }
    free_context(&context);
    exit();
}
