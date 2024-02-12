#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

#define MSGSIZE 1024

struct message {
    long type;
    char text[MSGSIZE];
    char sender[MSGSIZE];
    char reciever[MSGSIZE];
};

int msgid = 0;

void signalHandler(int signal){
    msgctl(msgid, IPC_RMID, NULL);
    exit(0);
}

unsigned long hash(char* str){
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

int main()
{
    printf("Sunucu baslatiliyor...\n");
    printf("Sunucuyu kapatmak icin CTRL+C tus kombinasyonunu kullanin.\n");

    key_t key = ftok(".", -1);
    msgid = msgget(key, 0666 | IPC_CREAT);
    
    signal(SIGINT, signalHandler); // CTRL+C
    
    struct message received;

    while(1){
        msgrcv(msgid, &received, sizeof(struct message), 1, 0);
        printf("%s istemcisi icin worker thread olusturuluyor...\n", received.sender);

        pid_t pid = fork();

        if (pid == 0) {
            char type[strlen(received.sender)+2];
            strcpy(type, received.sender);
            strcat(type, "0");
            unsigned long hashed = hash(type);

            struct message received;

            while(1){
                if (msgrcv(msgid, &received, sizeof(struct message), hashed, 0) != -1){
                    printf("\n%s istemcisinden alinan mesaj: %s\n", received.sender, received.text);
                    printf("%s istemcisinden --> %s istemcisine gönderiliyor...\n", received.sender, received.reciever);
                }

                received.type = hash(received.reciever);
                msgsnd(msgid, &received, sizeof(struct message), 0);
            }

            exit(0);
        }
    }

    return 0;
}
