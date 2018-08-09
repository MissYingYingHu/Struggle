#pragma once

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

typedef struct Coordinate
{
    int x;
    int y;
}Coordinate;


// ��������
int InitSocket(char *ip, int port);

// �̺߳������1
void *Fighting1(void* arg);

// �̺߳������2
void *Fighting2(void* arg);

// �����߳�
void CreateWorker(int sock);
