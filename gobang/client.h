#pragma once

#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>

typedef struct Coordinate
{
    int x;
    int y;
}Coordinate;

// ��Ϸ�˵�
int menu();

// ��ʼ������
void InitArr();

// ����
void Chessboard();

// ����Ϸ��ж�
Coordinate InputToDecide();

// �ж���Ӯ
int DecideWinLoss(Coordinate coor, char c);

// �Ƚ������ӵ�
void FirstTalk(int sock);

// ������
void SecondTalk(int sock);

// ��������
int Connect(char* ip, char* port);

// �˻���ս
// �ж����·���
Coordinate UpDown(Coordinate coor);

// �ж����Һ�������
Coordinate UpRight(Coordinate coor);

// �ж����ҷ���
Coordinate LeftRight(Coordinate coor);

// �ж����º����Ϸ���
Coordinate RightDown(Coordinate coor);

// �ҳ��Է������������л�����
Coordinate AroundHave(Coordinate coor, char c);

// Ϊ��������Ҫ�µ�λ�õĺ��������п��������㷨
Coordinate ProductPos(Coordinate coor);

// �˼Ҷ�ս���
void PeopleFightMachine();