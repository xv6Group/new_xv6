#include "types.h"
#include "stat.h"
#include "user.h"
#include "context.h"
#include "drawingAPI.h"
#include "message.h"
#include "bitmap.h"
#include "clickable.h"

struct Context context;

int main(int argc, char *argv[]) {
	int winid;
	struct Msg msg;
	int isRun = 1;
	int pid;
	int isInit = 1;

	winid = init_context(&context, 400, 300);

	fill_rect(context, 0, 0, context.width, context.height, 0xf800);

	draw_window(context, "finder");

	while (isRun) {
		getMsg(&msg);
		switch (msg.msg_type) {
		case MSG_UPDATE:

			if (isInit) {
				printf(1, "init ls: starting ls\n");
				pid = fork();
				if (pid < 0) {
					printf(1, "init ls: ls failed\n");
					exit();
				}
				if (pid == 0) {
					exec("ls", argv);
					printf(1, "init ls: exec ls failed\n");
					exit();
				}
				isInit = 0;
			}


			updateWindow(winid, context.addr);
			break;
		default:
			break;
		}
	}

	free_context(&context, winid);
	exit();
}
