#include "types.h"
#include "stat.h"
#include "user.h"
#include "context.h"
#include "drawingAPI.h"

struct Context context;

int
main(int argc, char *argv[])
{
    int winid;

    winid = init_context(&context, 400, 300); 
    fill_rect(context, 0, 0, context.width, context.height, 255);
    char str[5] = "haha";
    puts_str(context, str, 100, 100, 100);

    free_context(&context, winid);
    exit();
}
