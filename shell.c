#include "types.h"
#include "stat.h"
#include "user.h"
#include "context.h"
#include "drawingAPI.h"

struct Context context;

int
main(int argc, char *argv[])
{
    int pid, wpid;

    init_context(&context, 400, 300); 
    fill_rect(context, 0, 0, context.width, context.height, 255);
    draw_character(context, 100, 100, 'W');

    free_context(&context);
    exit();
}
