#include "stdafx.h"
#ifndef var_dspH
#define var_dspH

// ������� ������ ��� ������ ��������� � ������ ������� Bsp
#define N_L 1695 //1296 //1278 //1296
#define N_M 565 //432 //426 //432
#define Nh 4
#define N_RAS_1 1000

struct QQQ
{
    int Hapka[4];
    float Buf[1691];
};



struct Imitat
{
    int az1; // ������ 1-� ����  � ����
    int az2; // ������ 2-� ����
    int daln1; // ��������� 1-� ����   � ������
    int daln2; // ��������� 2-� ����
    int min_daln; // ��� ��������� �����       � ������
    int vr1; // ��������� 1-� ���� � ��/�
    int vr2; // ��������� 2-� ���� � ��/�
};


typedef struct RRR
{
        union
        {
                struct
                {
                        int Hapka[4];
                        float Buf[1691];
                }hm;
                struct
                {
                        int Hapka[4];
                        float Buf[561];
                }mono[3];
        }dsp;
}RRR;
typedef struct BBB
{
        union
        {
                struct
                {
                        int Hapka[4];
                        int Buf[1691];
                }hm;
                struct
                {
                        int Hapka[4];
                        int Buf[561];
                }mono[3];
        }dsp;
}BBB;
#endif
