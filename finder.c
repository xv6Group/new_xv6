#include "types.h"
#include "stat.h"
#include "user.h"
#include "context.h"
#include "drawingAPI.h"
#include "message.h"
#include "bitmap.h"
#include "finder.h"
#include "windowStyle.h"

struct Context context;

int
main(int argc, char *argv[])
{

    int winid;
    struct Msg msg;
    int isRun = 1;
    int i = 0;
    int cwidth;
    int cheight;
    //PICNODE pic;

    winid = init_context(&context, 400, 300);
    cwidth = context.width;
    cheight = context.height;

    fill_rect(context, 0, 0, cwidth, cheight, 0xFFFF);

//    puts_str(context, "shell: welcome", 0x0, 0, 0);
//    draw_line(context, 0, 0, 50, 50, 0x0);
//    loadBitmap(&pic, "9.bmp");
//    draw_picture(context, pic, 0, 0);


//    draw_window(context, "finder");

    PICNODE pic;
    draw_line(context, 0, 0, context.width - 1, 0, BORDERLINE_COLOR);
    draw_line(context, context.width - 1, 0, context.width - 1, context.height - 1, BORDERLINE_COLOR);
    draw_line(context, context.width - 1, context.height - 1, 0, context.height - 1, BORDERLINE_COLOR);
    draw_line(context, 0, context.height - 1, 0, 0, BORDERLINE_COLOR);
    fill_rect(context, 1, 1, context.width - 2, TOPBAR_HEIGHT + TOOLSBAR_HEIGHT, TOOLSBAR_COLOR);

    loadBitmap(&pic, "close.bmp");
    draw_picture(context, pic, 3, 3);
    loadBitmap(&pic, "foldericon.bmp");
    draw_picture(context, pic, cwidth / 2 - 20, 3);
    puts_str(context, "Finder", 0x0, cwidth / 2 + 2, 3);

    loadBitmap(&pic, "viewingmode1.bmp");
    draw_picture(context, pic, cwidth - 66, TOPBAR_HEIGHT +TOOLSBAR_HEIGHT - 66);
    loadBitmap(&pic, "viewingmode2.bmp");
    draw_picture(context, pic, cwidth - 140, TOPBAR_HEIGHT +TOOLSBAR_HEIGHT - 66);

    loadBitmap(&pic, "createfolder.bmp");
    draw_picture(context, pic, 3, TOPBAR_HEIGHT +TOOLSBAR_HEIGHT - 66);

    loadBitmap(&pic, "createfile.bmp");
    draw_picture(context, pic, 70, TOPBAR_HEIGHT +TOOLSBAR_HEIGHT - 66);

    loadBitmap(&pic, "up.bmp");
    draw_picture(context, pic, 150, TOPBAR_HEIGHT +TOOLSBAR_HEIGHT - 66);

    // loadBitmap(&pic, "cdup.bmp");
    // draw_picture(context, pic, 10, TOPBAR_HEIGHT + 10);

    // loadBitmap(&pic, "newfolder.bmp");
    // draw_picture(context, pic, 50, TOPBAR_HEIGHT + 10);
    // loadBitmap(&pic, "newfile.bmp");
    // draw_picture(context, pic, 70, TOPBAR_HEIGHT + 10);

    while(isRun)
    {
        getMsg(&msg);
        switch(msg.msg_type)
        {
            case MSG_DOUBLECLICK:
                i++;
                puts_str(context, "shell double clicked!", 0x0, 0, 20*i);
                updateWindow(winid, context.addr);
                break;
            case MSG_UPDATE:
                updateWindow(winid, context.addr);
                printf(0, "shell");
                break;
            case MSG_LPRESS:
                i++;
                puts_str(context, "shell clicked!", 0x0, 0, 20*i);
                updateWindow(winid, context.addr);
                break;
            case MSG_KEYDOWN:
                i++;
                printf(0, "key pressed: %s\n", msg.concrete_msg.msg_key.key);
                char temp[2];
                temp[0] = msg.concrete_msg.msg_key.key;
                temp[1] = '\0';
                puts_str(context, temp, 0*0, 0, 20*i);
                updateWindow(winid, context.addr);
                break;
            default:
                break;
        }
    }
    
    free_context(&context, winid);
    exit();
}
