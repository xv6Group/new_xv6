#include "types.h"
#include "stat.h"
#include "user.h"
#include "context.h"
#include "drawingAPI.h"
#include "message.h"
#include "bitmap.h"

#define CHARWIDTH 7
#define CHARHEIGHT 20
#define CHARS 80
#define LINES 20

#define BUFSIZE 1024
char printer_buf[LINES][CHARS];
int line_index = 0;
int char_index = 0;
int isFull = 0;

void
init_printer()
{
    int i;
    for (i = 0; i < LINES; i++) {
        memset(printer_buf[i], 0, sizeof(char) * CHARS);
    }
}

void
clear_printer(struct Context context)
{
    fill_rect(context, 0, CHARHEIGHT+2, context.width, context.height-2*(CHARHEIGHT+2), 0x0);
}

void 
shell_printer(struct Context context, char* read_buf, int len)
{
    int i, j;

    if (len < 0) {
        for (i = 0; i > len; i--) {
            if (char_index > 0) {
                printer_buf[line_index][--char_index] = 0;
            } else {
                break;
            }
        }
    }
    else {
        for (i = 0; i < len; i++) {
            if (read_buf[i] == '\n' || char_index >= CHARS) {
                line_index++;
                if (line_index >= LINES) {
                    isFull = 1;
                    line_index = 0;
                }
                memset(printer_buf[line_index], 0, sizeof(char) * CHARS);
                char_index = 0;
            } else {
                printer_buf[line_index][char_index++] = read_buf[i]; 
            } 
        }
    }
    
    clear_printer(context);
    if (!isFull) {
        for (i = 0; i <= line_index; i++) {
            puts_str(context, printer_buf[i], 0xffff, CHARWIDTH, CHARHEIGHT * (i + 1) + 2);
        }
    }
    else {
        for (i = (line_index + 1) % LINES, j = 1; j <= LINES; i = (i + 1) % LINES, j++) {
            puts_str(context, printer_buf[i], 0xffff, CHARWIDTH, CHARHEIGHT * j + 2);
        }
    }
}

int
main(int argc, char *argv[])
{
    int winid;
    struct Msg msg;
    struct Context context;
    int isRun = 1;

    winid = init_context(&context, (CHARS+2)*CHARWIDTH, (LINES+2)*CHARHEIGHT);
    fill_rect(context, 0, 0, context.width, context.height, 0x0);
    draw_window(context, "I'm SHELL");

    char *sh_argv[] = { "shell_sh", 0, 0 };
    int sh_pid;//, sh_wpid;
    char rfd[2], wfd[2];
    int gui2sh_fd[2], sh2gui_fd[2];

    printf(1, "init pipe: starting pipe\n");
    if(pipe(gui2sh_fd) != 0){
        printf(1, "init pipe: pipe() failed\n");
        exit();
    }
    if(pipe(sh2gui_fd) != 0){
        printf(1, "init pipe: pipe() failed\n");
        exit();
    }
    printf(1, "init pipe: pipe is ok\n");
    memset(rfd, 0, sizeof(char) * 2);
    memset(wfd, 0, sizeof(char) * 2);
    sh_argv[1] = rfd;
    sh_argv[2] = wfd;
    
    printf(1, "init sh: starting sh\n");
    sh_pid = fork();
    if(sh_pid < 0){
        printf(1, "init sh: fork failed\n");
        close(gui2sh_fd[0]);
        close(gui2sh_fd[1]);
        close(sh2gui_fd[0]);
        close(sh2gui_fd[1]);
        exit();
    }
    else if(sh_pid == 0) {
        close(gui2sh_fd[1]);
        rfd[0] = gui2sh_fd[0];
        close(sh2gui_fd[0]);
        wfd[0] = sh2gui_fd[1];
        exec("shell_sh", sh_argv);
        printf(1, "init sh: exec sh failed\n");
        exit();
    } 
    else {
        close(gui2sh_fd[0]);
        wfd[0] = gui2sh_fd[1];
        close(sh2gui_fd[1]);
        rfd[0] = sh2gui_fd[0];
    }

    //while((sh_wpid=wait()) >= 0 && sh_wpid != sh_pid)
    //    printf(1, "init sh: sh finish\n");

    //if(write(fds[1], buf, 1033) != 1033){
    //while((n = read(fds[0], buf, cc)) > 0){

    char write_cmd[1024], cmd_ch;
    int write_index = 0;
    char read_buf[1024];
    int n = 0, tmp = 0;
    memset(write_cmd, 0, sizeof(char) * 1024);
    memset(read_buf, 0, sizeof(char) * 1024);
    init_printer();

    while (n < strlen("$ ")) {
        if ((tmp = read(rfd[0], read_buf, sizeof(read_buf))) > 0) {
            n += tmp;
            shell_printer(context, read_buf, tmp);
        }
    }

    while(isRun)
    {
        getMsg(&msg);
        switch(msg.msg_type)
        {
            case MSG_UPDATE:
                updateWindow(winid, context.addr);
                break;
            case MSG_KEYDOWN:
                cmd_ch = msg.concrete_msg.msg_key.key;
                //printf(1, "%d\n", cmd_ch);
                if (cmd_ch == 8) {
                    if (write_index > 0) {
                        write_cmd[--write_index] = 0;
                        shell_printer(context, 0, -1);
                    }
                    updateWindow(winid, context.addr);
                    break;
                }
                write_cmd[write_index++] = cmd_ch;
                shell_printer(context, &cmd_ch, 1);
                if (strlen(write_cmd) == 1023) {
                    shell_printer(context, "\nThe command is too long!\n", 
                                  strlen("\nThe command is too long!\n"));
                    memset(write_cmd, 0, sizeof(char) * 1024);
                    write_index = 0;
                }
                if (cmd_ch == '\n') {
                    if (write(wfd[0], write_cmd, strlen(write_cmd)) != strlen(write_cmd)) {
                        printf(1, "gui pipe write: failed");
                    }
                    else {
                        while (1) {
                            if ((n = read(rfd[0], read_buf, sizeof(read_buf))) > 0) {
                                shell_printer(context, read_buf, n);
                                if (read_buf[n - 2] == '$' && read_buf[n - 1] == ' ') {
                                    break;
                                }
                            }
                        }
                    }
                    memset(write_cmd, 0, sizeof(char) * 1024);
                    write_index = 0;
                }

            //printf(1, "hahahhahah");
            //read_buf[n] = 0;
            //printf(1, read_buf);
                updateWindow(winid, context.addr);
                break;
            case MSG_LPRESS:
                break;
            case MSG_DOUBLECLICK:
                break;
            case MSG_DRAG:
                break;
            case MSG_PARTIAL_UPDATE:
                updatePartialWindow(winid, context.addr, msg.concrete_msg.msg_partial_update.x1, msg.concrete_msg.msg_partial_update.y1, msg.concrete_msg.msg_partial_update.x2, msg.concrete_msg.msg_partial_update.y2);
                break;
            default:
                break;
        }
    }
    
    free_context(&context, winid);
    exit();
}
