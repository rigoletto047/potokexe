#ifndef dsp_param_h
#define dsp_param_h
#define L_LEN 2048
#define M_LEN 512
#define R 2
#define L 4
typedef struct
{
// ��������� ��������
    unsigned int    Daln_HM;      // ��������� �� � ������
    unsigned int    Daln_MONO;    // ��������� ���� � ������
// ��������� ���
    unsigned int    aOPENMODE;
    unsigned int    aGEN;       // 1-������� ���������, 0-����������
    unsigned int    aMCLOCK;    // ������� �������� ���������� � ������
    unsigned int    aDECIMATION;// ��������� � ���
    unsigned int    aCHAN;      // ����� ������� ��� (0x00000000- ��� ������ ���������, 0xffffffff-��� ��������)
    int	            aFc[16];    // ������� �������� ���������� ������� ������ ��� � ������
    int	            aInput[16]; // �����, ����������� � ������� ������ (aInput[1]=3 - 1-� ��� �������� � 3 ������� ���)

  	unsigned int	aSYNC_MODE; // ��� ������������� (1 - �� ��������� �������, 0 - ����������� �����)
	unsigned int	aPGA;
	unsigned int	aRAND;
	unsigned int	aDITH;
	unsigned int	aADC16;
	unsigned int	aBIT24;

	int             aCADR_ENABLE;
	int             aCADR_RESTART;
	int             aCADR_MSTART;
	int             aCADR_INVSTART;
	int             aCADR_CNT0;
	int             aCADR_CNT1;
	int             aCADR_CNT2;
	int             aCADR_SIZE;
	int             aCADR_DATA;


  	unsigned int	aSM_MSTART; // 2 - ����� �� ��������� �������, 0 - ����������� �����)
	unsigned int	aSM_INVSTART;
	unsigned int    aSM_MSTOP;
	unsigned int	aSM_INVSTOP;
	unsigned int	aSM_TRSTART;
	float           aSM_THDAC1;
	float           aSM_THDAC2; // ����� ����������� � �������

    unsigned int    items;		// ������ ��������� ���
// ���������
    unsigned int    Nomer_komplekta ;   // ����� ���������
    unsigned int    f_proz ;            // ������� ����������
    unsigned int    zad_lhm ;           // �������� ���� ��������� ��
    unsigned int    sizeHM ;            // ����� ������ ��, ��������������� ��������� ���������+��������
    unsigned int    zad_mono ;          // �������� ���� ��������� ����
    unsigned int    sizeMONO;           // ����� ������ ����, ��������������� ��������� ���������+��������
    unsigned int    size_HM ;            // ����� ������ ��, ��������������� ��������� ���������
    unsigned int    size_MONO;           // ����� ������ ����, ��������������� ��������� ���������
    unsigned int    f_getHM ;           // ������� ���������� ��
    unsigned int    f_getMONO ;         // ������� ���������� ����
    unsigned int 	float_to_int;		// ��������� �������������� ����� float � int
// ��������� ������������ ��������� � �����������
    float    		porog;              // �����, ��� ���������� �������� ������ ������������ � ����� �����
    unsigned int    NR ;                // ������ ����� ����� ��
    unsigned int    NR_M;               // ������ ����� ����� ����
    float           PL_K_Phm;            // ��������� ������������� ��
    float           PL_K_Pmono;          // ��������� ������������� ����
    unsigned int    per_lhm ;           // ����� �������� ��
    unsigned int    per_mono;           // ����� �������� ����
    float           beta;               // ����������� ����������� ����� ����� ��
    float           beta_MONO ;         // ����������� ����������� ����� ����� ����
    float           PorogL;             // ����� �����������
    float			con;				// ���������, ��� ���������� ������� ������� ������ ��
    									// �����������: s*=sq_con
    float			sq_con;				// ���������, �� ������� ���������� ��	
    float			con_m;				// ���������, ��� ���������� ������� ������� ������ ����
    									// �����������: s*=sq_conm
    float			sq_con_m;			// ���������, �� ������� ���������� ����
    unsigned int	d_Wary1;			// ��������� �������� ���� �� ��
    unsigned int	wid1;				// ��� ������ ���� �� (0-����� ��������, 1-��������,2-������������)
    unsigned int	d_Wary2;			// ��������� �������� ���� �� ��
    unsigned int	wid2;				// ��� ������ ���� �� (0-����� ��������, 1-��������,2-������������)
    unsigned int	d_Wary1m;			// ��������� �������� ���� ���� ��
    unsigned int	wid1m;				// ��� ������ ���� �� (0-����� ��������, 1-��������,2-������������)
    unsigned int	d_Wary2m;			// ��������� �������� ���� ���� ��
    unsigned int	wid2m;				// ��� ������ ���� �� (0-����� ��������, 1-��������,2-������������)
    float           us0hm1;             // �������� � 0 ���� �� 1 ������
    float           us0hm2;             // �������� � 0 ���� �� 2 ������
    float           us0mono1;             // �������� � 0 ���� ���� 1 ������
    float           us0mono2;             // �������� � 0 ���� ���� 2 ������
// ������ ������
    unsigned int    CJATIE;             // ������
    unsigned int    Nakoplenie;         // ����� ���������� �������
    unsigned int    meteo;              // ����� ����� �������
    unsigned int    MTD;                // ����� MTD �������
    unsigned int    NU;                 // ������ ���� ��������
    unsigned int    VU;                 // ������� ���� ��������
    unsigned int	HM;					// ��������� ��������� ��� �� �� ���� ��������� (1) ��� ������ ����� ���� (0)
    unsigned int    nak;                // ���������� ������� ��� ��������� ������ ���������� ����� �����
    unsigned int    otladka;            // 1- ����� �������, ����� ������ ���������� � ������� Buf_HM_MONO
    unsigned int    median;             // 1- �������� ��������� ���������� 0 - ���������
    unsigned int    nom_hm;             // ����� �� ��� ����������� �������� ������
	unsigned int    nom_mono;           // ����� MONO ��� ����������� �������� ������
	float 			energij;			// ���������� ������� �� �������� � ����
	int				kol_oz;				// ����� ������� ������������ ������ �������� ����������� 
    unsigned int    WKL_HM;             // �� ��������
    unsigned int    WKL_MONO;           // ���� ��������
    unsigned int    tip_filt;			// ��� ������� ��. 0-�������������, 1- ��������, 2 -��� ����������
    float			da;					// ������ �� � ����
    float			tobs;				// ����� ������ � �
    // �������� ���� ��������� ���������� �����
    int				DALN;				// ���� ������ �� 1-� ���� (� ���)
	int 			Das;				// ���������� ���� 2-� ���� ������������ 1-� (� ���)
	int 			Ddal;				// ���������� �� ��������� 2-� ���� ������������ 1-� � ���������
	float 			amplituda;			// ��������� ���� 1
	float 			amplituda2;			// ��������� ���� 2
	int 			DMax;				// ��������� ���������� ��������� ����� � ���������
	int 			DELTAD;				// ���������� ��������� ��� �������� �����
	int 			DELTAD2;			// ���������� ��������� ��� �������� �����
	int 			N_Buf_Oz;			// ����� ������ ��� ���������� ������ ���������
	int				fd;					// ������� ������� 1-� ����
	int				fd2;				// ������� ������� 2-� ����
	int				md;					// ����������� ���������
	
	//unsigned int    aDECIMONO;// ��������� � ��� MONO

}PARAM_DSP;

#endif
