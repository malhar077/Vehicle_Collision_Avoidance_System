#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>

struct msg_buffer {
    long msg_type;
    char msg_text[100];
};

int msgid;

void init_msg_queue() {
    msgid = msgget(1234, 0666 | IPC_CREAT);
}

void send_message(char *text) {
    struct msg_buffer msg;
    msg.msg_type = 1;

    strcpy(msg.msg_text, text);
    msgsnd(msgid, &msg, sizeof(msg), 0);
}