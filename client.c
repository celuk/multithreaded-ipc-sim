#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <signal.h>

#define MSGLENGTH 1024
#define IDLENGTH 100

struct message {
    long type;
    char text[MSGLENGTH];
    char sender[MSGLENGTH];
    char reciever[MSGLENGTH];
};

int msgid = 0;

struct message msg;
struct message firstMsg;

unsigned long hashed;

void signalHandler(int signal){
    exit(0);
}

unsigned long hash(char* str){
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;

    return hash;
}

int main(int argc, char *argv[])
{
    if(argc <= 1){
        printf("Istemci id'si belirtilmedi, kapatiliyor...\nKullanim sekli: ./client <client_id>\n");
        return 1;
    }

    printf("%s istemcisi baslatiliyor...\n", argv[1]);

    char input[IDLENGTH];

    key_t key = ftok(".", -1);
    msgid = msgget(key, 0666 | IPC_CREAT);

    signal(SIGINT, signalHandler); // CTRL+C

    strcpy(firstMsg.sender, argv[1]);
    firstMsg.type = 1;
    msgsnd(msgid, &firstMsg, sizeof(struct message), 0);

    strcpy(msg.sender, argv[1]);

    hashed = hash(argv[1]);

    pid_t pid = fork();
    if (pid == 0) {
        struct message received;

        while(1){
            if(msgrcv(msgid, &received, sizeof(struct message), hashed, 0) != -1)
                printf("\n%s istemcisinden gelen mesaj: %s\n", received.sender, received.text);
        }

        exit(0);
    }

    while(1){
        printf("\nYeni mesajlar bekleniyor...\n");

        printf("Mesaj gondermek için 'y', cikmak icin 'c' tusuna ve daha sonra entera basin\n\n");
        fgets(input, 10, stdin);
        input[strlen(input)-1] = '\0';

        if(strcmp(input, "y") == 0 || strcmp(input, "Y") == 0){
            printf("Mesaj gondermek istediginiz istemcinin id'sini girin: ");
            fgets(msg.reciever, MSGLENGTH, stdin);

            msg.reciever[strlen(msg.reciever)-1] = '\0';
            char serverReciever[strlen(msg.reciever)+2];
            strcpy(serverReciever, msg.reciever);
            strcat(serverReciever, "0");
            msg.type = hash(serverReciever);

            printf("%s istemcisine gondermek istediginiz mesaji yazin: ", msg.reciever);
            fgets(msg.text, MSGLENGTH, stdin);
            msg.text[strlen(msg.text)-1] = '\0';

            msgsnd(msgid, &msg, sizeof(struct message), 0);
        }
        else if(strcmp(input, "c") == 0 || strcmp(input, "C") == 0){
            return 0;
        }
        else{
            printf("\nHatali giris!\n");
        }
    }

    return 0;
}
