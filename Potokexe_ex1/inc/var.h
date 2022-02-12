#include "stdafx.h"
#ifndef var_dspH
#define var_dspH

// Создаем запись для выдачи информаци в старом формате Bsp
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
    int az1; // азимут 1-й цели  в град
    int az2; // азимут 2-й цели
    int daln1; // дальность 1-й цели   в метрах
    int daln2; // дальность 2-й цели
    int min_daln; // мин дальность целей       в метрах
    int vr1; // дальность 1-й цели в км/ч
    int vr2; // дальность 2-й цели в км/ч
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
