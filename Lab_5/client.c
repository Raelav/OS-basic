/******************************************************************************
     * File: Lab_5.c
     * Description: Лабораторная работа 5
     * Created: 12 jan 2020
     * Author: Melnikov Valery
     * Email: m-valeron-n@mail.ru
     * Task: Создать два процесса (две разные программы), один будет сервером, 
     *      второй клиентом. Обеспечить общение между этими программами в 
     *      произвольном порядке. Клиент может отправлять и получать сообщения 
     *      от сервера, и сревер может получать и отправлять сообщения от клиента. 
     *      При этом и сервер и клиент могут отправлять несколько сообщений подряд. 
     *      Для реализации такого обмена, необходимо у сервера создать два 
     *      канала/сокета, а клиенту к ним подключаться. В один канал/сокет 
     *      сервер пишет, соответственно клиент из него читает, в другой 
     *      канал/сокет пишет клиент, соответственно сервер из него читает. 
     *      Для параллельности работы необходимо использовать отдельные потоки 
     *      для каждого канала/сокета. В итоге, сервер от клиента отличается 
     *      только тем, что создает каналы/сокеты, а клиент только к ним подключается.
******************************************************************************/

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include "msgtype.h"

int get_msqid(char *pathname);
void *reading();
void *sending();
// sem_t semaphore1;
// sem_t semaphore2;

/// Пользовательская структура для сообщения
    // struct mymsgbuf
    // { 
    //     long mtype;
    //     char mtext[MESSAGE_LENGTH]; 
    // } mybuf;
Msgbuf mysendbuf;
Msgbuf myreadbuf;

int main()
{
    pthread_t receive, send;

    // sem_init(&semaphore1, 0, 0);
    // sem_init(&semaphore2, 0, 0);
    // sem_post(&semaphore1);

    pthread_create(&receive, NULL, reading, NULL);
    pthread_create(&send, NULL, sending, NULL);
    pthread_join(receive, NULL);
    pthread_join(send, NULL);

    // sem_destroy(&semaphore1);
    // sem_destroy(&semaphore2);

    return 0;
}

int get_msqid(char * pathname) {
    int msqid;   
    key_t key; 

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if((msqid = msgget(key, 0)) < 0){
        printf("Can\'t get msqid\n");
        exit(-1);
    } 
    return msqid;
}

void *reading() {
    int msqid = get_msqid(SERVER_SENDER);

    int len, maxlen;
    while(1){
        maxlen = MESSAGE_LENGTH;
        if(( len = msgrcv(msqid, 
            (struct msgbuf *) &myreadbuf, maxlen, 0, 0) < 0)){
            printf("Can\'t receive message from queue\n");
            exit(-1);
        }
        
        if (myreadbuf.mtype == SESION_OFF) {
            msgctl(msqid, IPC_RMID, (struct msqid_ds *) NULL);
            printf("Server disconnectet you!\n");
            exit(-1);
        }

        //sem_wait(&semaphore2);
        printf("SERVER: %s", myreadbuf.mtext);
        //sem_post(&semaphore1);
    }
    return 0;
}

void *sending() {
    int msqid = get_msqid(SERVER_READER);
    char text[MESSAGE_LENGTH];

    while (1) {
        //sem_wait(&semaphore1);
        fgets(text, MESSAGE_LENGTH, stdin);
        //sem_post(&semaphore2);

        mysendbuf.mtype = REGULAR_MESSAGE;
        strcpy(mysendbuf.mtext, text);
        int len = strlen(mysendbuf.mtext)+1;
        if (msgsnd(msqid, (struct msgbuf *) &mysendbuf, 
        len, 0) < 0) {
            printf("Can\'t send message to queue\n");
            msgctl(msqid, IPC_RMID, 
                (struct msqid_ds *) NULL);
            exit(-1);
        }
    }

    return 0;
}