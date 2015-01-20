#include "types.h"
#include "stat.h"
#include "user.h"
#include "context.h"
#include "drawingAPI.h"

struct Context context;

int main(int argc, char *argv[])
{
    //int pid, wpid;

    init_context(&context, 800, 600); 
    fill_rect(context, 0, 0, context.width, context.height, 0xdd);
    int windowId;
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
    updateWindow(windowId, context.addr);
    free_context(&context);
    exit();
}
