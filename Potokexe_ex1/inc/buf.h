#ifndef __buf__
#define __buf_
//#define RASMER 1690
//#define RASMER_MONO 300
//#define XM 0
//#define XMONO 60
#define L_LEN 2048
#define M_LEN 512
#define R 2
#define Lmax 8
//#define L 4
#define nK 4
typedef struct Buf_HM_MONO
{
    int Hapka_HM[8];// Hapka_HM[0..3] ������������ ��� �������� ������� ������, ����� ������ � �������� ����������
    //Hapka_HM[0]-�������� ������� ������ � ������ ����->1-� ���������->2-��������� � �������
    //Hapka_HM[1]-��� ������
    //Hapka_HM[2]-�������� ���������, ���������� ������
    //Hapka_HM[3]-������� ������������ (������ 16 ���) � ���-�� ������� (������ 16 ���)
    union
    {
        struct
        {
            int HM_Re_Im_1[L_LEN];
            int HM_Re_Im_2[L_LEN];
            int Hapka_MONO[4];
            int MONO_Re_Im_1[M_LEN];
            int MONO_Re_Im_2[M_LEN];
            int pusto[4];
        }bin;
        struct
        {
            float HM_Re_Im_1[L_LEN];
            float HM_Re_Im_2[L_LEN];
            int Hapka_MONO[4];
            float MONO_Re_Im_1[M_LEN];
            float MONO_Re_Im_2[M_LEN];
            float pusto[4];
        }dsp;
    }dan;

}Buf_HM_MONO;
typedef struct Oz
{
	float Amp;	// ��������� ������
	float D;			// ��������� � ������
	float As;			// ������ � ��������
	float Time;			// ����� � ��������
}Oz;
typedef struct TIP_4
{
		int Hapka_HM[8];
		float HM_Re_Im_1[L_LEN];
	
}TIP_4;
/*
typedef struct RABOTA
{
    int rejim;                      // ����� ������
    int kod;                        // ��� ������
    float snach;                    // �������� ������
    int nomer;                      // ������ ����� ������
    int obzor;                      // ������� ����� ������
    struct                          // ������ ������������ ������ ������ (����� ����������)
    {
        int Hapka[4];               // �����
        float kus;                  // ����������� �������� ��� �����������
        unsigned char Buf[RASMER];  // ������
    }rejim4;
    struct
    {
        int Hapka1[4];              // ������ �� 1 ������
        float kus1;
        unsigned char Buf1[RASMER];
        int Hapka2[4];              // ������ �� 2 ������
        float kus2;
        unsigned char Buf2[RASMER];
        int HapkaM1[4];             // ������ ���� 1 ������
        float kusM1;
        unsigned char BufM1[RASMER_MONO];
        int HapkaM2[4];             // ������ ���� 2 ������
        float kusM2;
        unsigned char BufM2[RASMER_MONO];
    }sqr[3];
    struct
    {
        int nom_HM,nom_MONO;
        int bok_HM,bok_MONO;
        float cjatie[8];
        float mtd[4];
        float sig_filt[4][4];
        float nakop;
        float lamda[4][4];
    }test;

}RABOTA;
*/
#endif
