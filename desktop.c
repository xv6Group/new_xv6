#include "types.h"
#include "stat.h"
#include "user.h"
#include "context.h"
#include "drawingAPI.h"
#include "bitmap.h"

struct Context context;

int main(int argc, char *argv[])
{
	//PICNODE pic;
    //int pid, wpid;
	initializeHankaku();
	initializeFontFile();

    init_context(&context, 800, 600); 
    fill_rect(context, 0, 0, context.width, context.height, 2016);
//    char str[5] = "haha";
//    puts_str(context, str, 0x0, 0, 0);
//    loadBitmap(&pic, "9.bmp");
//    draw_picture(context, pic, 0, 0);
    draw_line(context, 0, 0, 50, 50, 0x0);
    int windowId;
    int result;
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


    result = updateWindow(windowId, context.addr);
    printf(0, "updateResult: %d\n", result);

    while(1);
    free_context(&context);
    exit();
}
