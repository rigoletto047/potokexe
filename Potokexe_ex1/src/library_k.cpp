//#include <vcl.h>
//#include "Uprov.h"
// TODO � ���� ���������� �������� �� PCH intelli sence  #pragma hdrstop
#pragma once
// TODO #include "brd.h"

//#include "stdafx.h"
#include "var.h"
#include <math.h>
#include "dsp_param.h"
#include <complex>
#include <ctime>

//void Graf(complex<double> *s,long N0,double FD,int F,char *text,double x1,double x2,...);
#include "Bsp_common.h"
#include "buf.h"
#include "library_k.h"
//#include <complex>
//#include "UThread.h"
//#include "Options.h"
#include "UTest.h"
#include "my_structs.h"
#include <windows.h>
#include <stdio.h>
#include "sundries.h"
//#include "Ohistka.h"

////////////////////////////////////#include "Init_Soprov.h"
extern int(*MyInit_Soprov)(char *c);
//int  Init_Soprov(char *c);
////////////////////////////////#include "Init_Soprov.h"
//extern "C" int __declspec(dllexport)Init_Soprov(void);
//int M0;long N0;
using namespace std;


HANDLE hThreadExit = NULL;
//int otobrajenie;
int otladka_otm=0;
int Oz_file;
extern int rejim,komanda0;
extern int Wkl_Sop;
extern Imitat dsp_imitat;
double VR,vrm,Daln;
// extern Form_data FormData; // �� ������ �������������������!!!
extern PARAM_DSP dsp_param;
PARAM_DSP dsp_param_file;
extern double T_obs;
extern char fini[256];
extern double k_usil;
char fname[256] = ".\\rpu.ini";
extern char frpu[256];// = ".\\Cfg\\KChel\\rpu.ini";
extern HANDLE hFileMapStatus;
extern string ProgrammPath;
extern char fname[256];
extern LPVOID lpViewStatus;
extern volatile PBsp_Status status;
extern volatile PBsp_Status status2;
extern HANDLE hFileMapBuffer;
HANDLE hFileMap;
LPVOID lpView;
extern float *test_buf;
extern HANDLE hFileMapBuffer2;
extern HANDLE hFileMapStatus2;
extern LPVOID lpViewBuffer2;
extern LPVOID lpViewBuffer;
extern LPVOID lpViewStatus2;
extern QQQ * buffer2;
extern Buf_HM_MONO* buffer;
extern HANDLE hMutex;
// TODO extern BRD_Handle handle;
extern int avt_per;
int nKK;
//extern int Objem;      // ?????
//  SAGR()
extern FILE *fq, *flog,*fp;
extern int TEST;
extern int N_XOCT;
int mes[4];
extern float test_cjat_hm1,test_cjat_hm2,test_cjat_mono1,test_cjat_mono2,test_bok_hm1,test_bok_hm2,test_bok_mono1,test_bok_mono2,test_mtd,test_nak;

extern complex<short int> test_sig[500];
extern complex<float> KU_HM_NU[L_LEN],KU_HM_VU[L_LEN],KU_MONO_NU[M_LEN],KU_MONO_VU[M_LEN];
extern int Period[10];
extern complex<float> Filter_MTD[2*10*nK*Lmax];

extern string ProgPath;
extern unsigned int items;// ����� ������� ��������� ���
extern unsigned int g_aProgArray[1000+8];// ������ ��� �������� ��������� ���

extern int nom_kom,sad_hm,sad_mono,mnoj,waru1hm,waru2hm,waru1mono,waru2mono,widhm1,widhm2,widmono1,widmono2;
extern float reg_hm,reg_mono,sglaj,con,con_m,por_kart,por_obn;
extern int LL;
extern int sap_ozenki;
extern int Exit;
extern int start;
//extern My_Thread *Thread_Card;

extern int perekluh;
string Section;
//extern int otobrajenie;
int S_Okno;
extern FILE *foz;
extern unsigned int Objem;
extern int sap_file,sap_file1;
extern unsigned int K_K;
FILE *tt;
int amp_hm,amp_mono;
int nT;
float sig;
struct
{
        float mnoj_okna;//,mno;
        int prop,L_Buf,n;
}okno,oknom;
extern HINSTANCE Dll;
int tip_obr=0;
//complex<double> *s1;
//extern int M0;
//extern long N0;
//double FD=1.56125e6;
 //double FD=1.56125e6;
// = NULL, hFileMapStatus2 = NULL,hFileMapBuffer = NULL,hFileMapBuffer2 = NULL;
//////////////////////////////////////////////////////////
DWORD WINAPI
ThreadCardExit( void );
/*
    �������� �����, ������������� � ������
*/
void OpenFileMap( void )
{
    DWORD tid;

    rejim=dsp_param.rejim=GetPrivateProfileInt( "���������", "����� ������", 4, fini );
     //rejim=1;

    // ������������ ����� ��� ����������� ������ � ������ 4
    hFileMapStatus = CreateFileMapping( (HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Bsp_Status), "Bsp_Status" );
    if( hFileMapStatus == NULL )
			{
				//SetCurrentDir(ProgrammPath);
				WritePrivateProfileString( "Bsp", "Message", "FileMapStatus Error", fname );
				return;
			}
	
		lpViewStatus = MapViewOfFile( hFileMapStatus, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0 );
    if( lpViewStatus == NULL )
     {
     	CloseHandle( hFileMapStatus );
     	hFileMapStatus = NULL;
      //SetCurrentDir(ProgrammPath);
     	WritePrivateProfileString( "Bsp", "Message", "lpViewStatus Error", fname );
     	return;
     }

    status = (PBsp_Status)lpViewStatus;
    BSP_RESET( status );
    ////////////////////////
    hFileMap = CreateFileMapping( (HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 50*sizeof(float), "Bsp_Test" );
    if( hFileMap == NULL )
     {
        //SetCurrentDir(ProgrammPath);
				WritePrivateProfileString( "Bsp", "Message", "FileMap Error", fname );
				return;
     }

    lpView = MapViewOfFile( hFileMap, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0 );
    if( lpView == NULL )
     {
     		CloseHandle( hFileMap );
     		hFileMap = NULL;
				//SetCurrentDir(ProgrammPath);
				WritePrivateProfileString( "Bsp", "Message", "lpView Error", fname );
     		return;
     }

    test_buf =(float*) lpView;
    ////////////////////////
    //if(rejim==4)     ////////////////////////////
    {
        hFileMapBuffer2 = CreateFileMapping( (HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,	BUFFER_SIZE * sizeof(QQQ), "Bsp_Buffer" );
        if( hFileMapBuffer2 != NULL )
        {
     	        lpViewBuffer2 = MapViewOfFile( hFileMapBuffer2, FILE_MAP_WRITE, 0, 0, 0 );
     	        if( lpViewBuffer2 == NULL )
                {
	                CloseHandle( hFileMapBuffer2 );
	                hFileMapBuffer2 = NULL;
                  // SetCurrentDir(ProgrammPath);
	                WritePrivateProfileString( "Bsp", "Message", "lpViewStatus Error 1", fname );
	        }
	        else
                        buffer2 = (QQQ *)lpViewBuffer2;
        }
        else
        {
                //SetCurrentDir(ProgrammPath);
       	        WritePrivateProfileString( "Bsp", "Message", "lpViewStatus Error 2", fname );
        }
    ////////////////////////////
    }

// ������������ ����� ��� ������ � ������ 0 ��� 1
    hFileMapStatus2 = CreateFileMapping( (HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, sizeof(Bsp_Status), "Bsp_Status2" );
    if( hFileMapStatus2 == NULL )
			{
				//SetCurrentDir(ProgrammPath);
				WritePrivateProfileString( "Bsp", "Message", "FileMapStatus2 Error", fname );
				return;
			}
    lpViewStatus2 = MapViewOfFile( hFileMapStatus2, FILE_MAP_READ|FILE_MAP_WRITE, 0, 0, 0 );
    if( lpViewStatus2 == NULL )
			{
			CloseHandle( hFileMapStatus2 );
			hFileMapStatus2 = NULL;
			//SetCurrentDir(ProgrammPath);
			WritePrivateProfileString( "Bsp", "Message", "lpViewStatus2 Error", fname );
			return;
			}

	status2 = (PBsp_Status)lpViewStatus2;
	BSP_RESET( status2 );
        //if(rejim<4)
        {
	        hFileMapBuffer = CreateFileMapping( (HANDLE)INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0,
					(BUFFER_SIZE * sizeof(Buf_HM_MONO)), "Bsp_Buffer_new" );
	        if( hFileMapBuffer != NULL )
            {
							lpViewBuffer = MapViewOfFile( hFileMapBuffer, FILE_MAP_WRITE, 0, 0, 0 );
							if( lpViewBuffer == NULL )
								{
									CloseHandle( hFileMapBuffer );
									hFileMapBuffer = NULL;
									//SetCurrentDir(ProgrammPath);
									WritePrivateProfileString( "Bsp", "Message", "lpViewBuffer == NULL", fname );
								}
							else
								buffer = (Buf_HM_MONO *)lpViewBuffer;
						}
	        else
            {	
							//SetCurrentDir(ProgrammPath);
							WritePrivateProfileString( "Bsp", "Message", "hFileMapBuffer == NULL", fname );
						}


        }
        //SetCurrentDir(ProgrammPath);
	WritePrivateProfileString( "Bsp", "Message", "FileMap Ok", fname );

    // �������� �������� ��� �������� ������ �� ����� ������
	hThreadExit = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCardExit, NULL, 0, &tid );
	SetThreadPriority( hThreadExit,THREAD_PRIORITY_TIME_CRITICAL );
/*
    // �������� �������� ��� ����� � ������
	hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCard, NULL, 0, &tid );
    //SetThreadPriority( hThread,THREAD_PRIORITY_TIME_CRITICAL );
*/
}


//////////////////////////////////////////////////////////
/*
   // �������� ����� ������
*/

void CloseFileMap( void )
{
    //priem->Timer1->Enabled=false;
	if( lpViewStatus )
    {
		BSP_RESET( lpViewStatus );
  	    UnmapViewOfFile( lpViewStatus );
		lpViewStatus = NULL;
	}

    if( lpViewStatus2 )
    {
		BSP_RESET( lpViewStatus2 );
  	    UnmapViewOfFile( lpViewStatus2 );
		lpViewStatus2 = NULL;
	}

    if( hFileMapStatus )
  	    CloseHandle( hFileMapStatus );
    if( hFileMapStatus2 )
  	    CloseHandle( hFileMapStatus2 );
	if( lpViewBuffer )
		UnmapViewOfFile( lpViewBuffer );
	if( hFileMapBuffer )
		CloseHandle( hFileMapBuffer );
    if( lpViewBuffer2 )
		UnmapViewOfFile( lpViewBuffer2 );
	if( hFileMapBuffer2 )
		CloseHandle( hFileMapBuffer2 );
	ReleaseMutex( hMutex );
	CloseHandle( hMutex );
    //SetCurrentDir(ProgrammPath);
	WritePrivateProfileString( "Bsp", "Message", "CloseFileMap Ok 7", fname );
  //SendMessage(priem->Handle,WM_CLOSE,0,0);
    //priem->Close();

}


//////////////////////////////////////////////////////////
/*
        // ������������� ����� � ������ ���������� ����������� �� ����������
*/
//int Ini_Start(char *file)
//{
//// ������������� ����� � ������ ���������� ����������� �� ����������
//    S32 err;
//    S32 pNum;
//    char ss[256];
//    U32			openMode;
//    U32 ite;
//    SetCurrentDir(ProgrammPath);
//    err=BRD_cleanup();
//
//    //BRD_reset(handle,0);
//    //BRD_reset(handle,1);
//            
//    // ������������� �����
//    err = BRD_initEx( BRDinit_FILE_KNOWN, (ProgrammPath+"brd.ini").c_str(), (ProgrammPath+"log.ini").c_str(), &pNum );
//        /*
//        ���������
//        ���������� pNum ���� ��������� ������ ���� ��������� � ������,
//        � �� � ��������. ����� ��� ���� ���, ����� ������� �� � ����
//        ������� �� ������ �������� pNum[1] ��� ���������� ����� ����������
//        */
//    if(BRD_errcmp(err,BRDerr_OK))
//        ;//MessageBox(NULL,"����� ������� �������������������","Message",MB_OK);
//    else
//    {
//        if( BRD_errcmp(err,BRDerr_NONE_DEVICE) )
//        {
//            MessageBox(NULL,"�� ���������� �������� ������","Error",MB_OK);
//            return -1;
//        }
//        else  if(BRD_errcmp(err,BRDerr_BAD_INI_FILE))
//        {
//            MessageBox(NULL,"���������� ������� ���� �������������","Error",MB_OK);
//            return -1;
//        }
//        else
//        {
//            MessageBox(NULL,"������ �������� �����","Error",MB_OK);
//            return -1;
//        }
//    }
//    // ����� ���� ������� �������
//    U32  lidArray[1], itemReal, g_lid;
//    BRD_lidList( lidArray, 1, &itemReal );
//    if( itemReal==0 )
//    {
//        MessageBox(NULL,"�� ���������� �������� ������","Error",MB_OK);
//        return -1;
//    }
//    g_lid = lidArray[0];// ����������� ������ �����
//    // �������� �����
//    handle = BRD_open( g_lid, BRDopen_EXCLUSIVE, &openMode );
//    if(handle<=0)
//    {
//        sprintf(ss, " ERROR: ������ ������� ���������� (LID=%d)", g_lid );
//        MessageBox(NULL,ss,"Error",MB_OK);
//        return -1;
//    }
//    err=BRD_reset(handle,0);
//    if(err<0)
//    {
//        MessageBox(NULL,"���������� ����� �����","Error",MB_OK);
//        return -1;
//    }
//    // �������� �����
//    GetPrivateProfileString( "�����", "��������� ����", "cd416s54.h10",ss,256, fini );
//    err=BRD_puLoad(handle,0x100,ss,&ite);
//    if(!BRD_errcmp(err,BRDerr_OK))
//    {
//        if(BRD_errcmp(err,BRDerr_BAD_ID))
//            MessageBox(NULL,"�������� �������������","Error",MB_OK);
//        if(BRD_errcmp(err,BRDerr_BAD_FILE))
//            MessageBox(NULL,"���� ����������","Error",MB_OK);
//        if(BRD_errcmp(err,BRDerr_HW_ERROR))
//            MessageBox(NULL,"���������� ������","Error",MB_OK);
//        //BRD_cleanup();
//        return -1;
//    }
//    if(ite==0)
//    {
//        MessageBox(NULL,"���� �� ���������","Error",MB_OK);
//        //BRD_cleanup();
//        return -1;
//    }
//    //BRD_SigList Prer[10];
//    //err=BRD_signalList(handle,0,&Prer[0],10,&itemReal);
//    // �������� ��������� � ���������� ���������
//    err=BRD_load( handle, 0, file, 0, NULL );
//    if(BRD_errcmp(err,BRDerr_OK))
//    ;   //MessageBox(NULL,"��������� ���������","Message",MB_OK);
//    else
//    {
//        //BRD_cleanup();
//        MessageBox(NULL,"������ ��� �������� ���������","Message",MB_OK);
//        return -1;
//    }
//
//    // ������ ��������� �� ����������
//    err=BRD_start( handle, 0 );
//    if(BRD_errcmp(err,BRDerr_OK))
//    ;
//    else if(BRD_errcmp(err,BRDerr_BAD_HANDLE))
//        MessageBox(NULL,"������������ ����������","Error",MB_OK);
//    else  if(BRD_errcmp(err,BRDerr_CLOSED_HANDLE))
//        MessageBox(NULL,"���������� ����������� �������� ������","Error",MB_OK);
//    if(!BRD_errcmp(err,BRDerr_OK))
//    {
//        MessageBox(NULL,"���������� ����������","Error",MB_OK);
//        //BRD_cleanup();
//        return -1;
//    }
//    return 0;
//}


////////////////////////////////////////////////////////////
///*
//        // ������ �����, ����������� ��������� ���
//*/
//int     ReadPrgFile( char *fileName, U32 *dst, S32 size )
//{
//// ������ �����, ����������� ��������� ���
//    FILE    *stream;
//	U32		page, adr, data;
//	S32		ind=0;
//    char    *endptr;
//    char    g_str[256];
//    float x;
//
//	stream = fopen( fileName, "rt" );
//	 
//
//	if(stream==NULL)
//	{
//		//sprintf( "*** ������ ������� PrgFile - '%s'\n", fileName );
//		return -1;
//	}
//
//	//printf( "\n****** Open PrgFile - '%s'******\n", fileName );
//	//printf( "Reading PrgFile...\n");
//
//        fgets( g_str, 80, stream );
//        fgets( g_str, 80, stream );
//        fgets( g_str, 80, stream );
//        sscanf(g_str,"Decimation Channel0= %f",&x);
//        dsp_param.aDECIMATION=(int)x;
//
//
//	//
//	// Search Register Area
//	//
//	while( !feof(stream) )
//	{
//        fgets( g_str, 80, stream );
//		if( 0 == strncmp( g_str, "Page Reg Val", 12 ) )
//			break;
//	}
//    while( 1 )
//    //while( !feof(stream) )
//    {
//        fgets( g_str, 80, stream );
//       	page = strtoul( g_str, &endptr, 0 );
//       	adr  = strtoul( endptr, &endptr, 0 );
//       	data = strtoul( endptr, &endptr, 0 );
//       	dst[ind] = ((page&0xff)<<16) | ((adr&0xff)<<8) | (data&0xff);
//		if( (ind>0) & (dst[ind] == dst[ind-1]) )
//			break;	// End of File
//        ind++;
//		if( ind>=size )
//			break;
//		
//    }
//	return	ind;
//}


//////////////////////////////////////////////////////////
/*
        // �������� ���������� �� ���-�����
*/
//---------------------------------------------------------------------------
int sagr(void)
{

    unsigned int size;
    int *q1,*q2;

    int mes[4];
    char prog_ddc[256]="10_02_11.prg";

    char ss[80],*s;
    double x1,x2,x3;
    int k,k1;
    int k2,k3,k4;
    char imj_file_test_hm1[256],imj_file_test_hm2[256],imj_file_test_mono1[256],imj_file_test_mono2[256],
    imj_file_cjat_hm1[256],imj_file_cjat_hm2[256],imj_file_cjat_mono1[256],imj_file_cjat_mono2[256],imj_file_mtd[256];
    string Nom_Kml;

    //double FD=1.56125e6;
    //int M0;long N0;
    //M0=11;N0=1<<M0;
    //s1=new complex<double>[N0];
		ProgrammPath = GetCurrentDirectoryMC();
    //ProgrammPath=ExtractFilePath(Application->ExeName);
    SetCurrentDirectoryA(ProgrammPath.c_str());

    // ���������� �� ������ ��� ������������
    //perekluh=!perekluh;
    //Form4->Button7Click(0);
    WritePrivateProfileString("Conrad","Exit","0",fname);
    // ������ ��������� ���������� ������ ���
    dsp_param.Daln_HM = GetPrivateProfileInt( "���������", "��", 174000, fini );
    dsp_param.Daln_MONO = GetPrivateProfileInt( "���������", "����", 23000, fini );
    dsp_param.Daln_VU = GetPrivateProfileInt( "���������", "������� �����", 100000, fini );
    if(avt_per)
    {
    fprintf(flog,"\n ��������� �� %d",dsp_param.Daln_HM);
    fprintf(flog,"\n ��������� ���� %d",dsp_param.Daln_MONO);
    fprintf(flog,"\n ��������� ������� ����� %d",dsp_param.Daln_VU);
    }
    //*********** ������ ��� ********************************************************************************************
    dsp_param.aDECIMONO = GetPrivateProfileInt( "���", "��������� ��� ����", 32, fini );


  // ************   ������ ���������  ***********************************************************************************
    dsp_param.sost_upr=GetPrivateProfileInt( "���������", "���������� �� ����������", 1, fini );
  // ��������� ����� ���������
    dsp_param.Nomer_komplekta=GetPrivateProfileInt( "���������", "����� ���������", 1, fini );
    // � ����������� �� ������ ��������� ��������� ����� ��������
    Section= "����� ��������� "+ std::to_string(dsp_param.Nomer_komplekta);
    /*
    if (dsp_param.Nomer_komplekta!=GetPrivateProfileInt( "System", "SectorName", 1, frpu ) )
     {
        MessageBox(NULL,"����� ��������� � mtd__n.ini � rpu.ini �� ���������, ����������","Error",MB_OK);
        Nom_Kml=IntToStr(int(dsp_param.Nomer_komplekta)); //???
        WritePrivateProfileString( "System", "SectorName", Nom_Kml.c_str(), frpu );
     }
     */
    // � ���������� ����� ���������
    //if (dsp_param.Nomer_komplekta==1)
    //  Form4->Button15->Caption = "1";
    //else
    //  Form4->Button15->Caption = "2";

   // if (GetPrivateProfileInt("System", "NoConrad", 0, frpu)==0)
   //  FormData.flag_work_VK = 2; // ������� ��
   // else
   //  FormData.flag_work_VK = 0; // �������� ��

//???    dsp_param.aMCLOCK=GetPrivateProfileInt( "���������", "������� �������� ����������", 50000000, fini );

    dsp_param.rejim=GetPrivateProfileInt( "���������", "����� ������", 4, fini );
    rejim=dsp_param.rejim;
    //dsp_param.otobrajenie=GetPrivateProfileInt( "���������", "����� �����������", 0, fini );
    dsp_param.f_proz=GetPrivateProfileInt( "���������", "������� ����������", 500000000, fini );
    if(avt_per )
    {
    fprintf(flog,"\n\n  *******  ��������� **********\n\n");
    fprintf(flog,"\n ���������� �� ���������� %d",dsp_param.sost_upr);
    fprintf(flog,"\n ����� ��������� %d",dsp_param.Nomer_komplekta);
    fprintf(flog,"\n ����� ������ %d",dsp_param.rejim);
    fprintf(flog,"\n ������� ���������� %d",dsp_param.f_proz);
    }
    
    dsp_param.aGEN=GetPrivateProfileInt( "���������", "��������� �������", 0, fini );
    if(dsp_param.aGEN ==0)
    {
        //err_gen=1;
        if(avt_per )
        {
        fprintf(flog,"\n\n  *******  ��������� **********\n\n");
        fprintf(flog,"\n ���������� ���������� �������� ���������. ������� %d",dsp_param.aMCLOCK);
        }
        //Form1->Panel1->Visible=true;
        dsp_param.aMCLOCK =GetPrivateProfileInt( "���������", "������� ����������� ����������", 100000000, fini );
        //Form4->Shape14->Brush->Color=clYellow;
        //Form4->Label14->Caption = "����������";
        //Form4->Label35->Caption = "������� " + FloatToStrF(dsp_param.aMCLOCK/(1000000),ffFixed, 7,2)+" ���" ;
    }
    else
    {
        //err_gen=0;

        //Form1->Panel1->Visible=false;
        dsp_param.aMCLOCK =GetPrivateProfileInt( "���������", "������� �������� ����������", 50000000, fini );
        //Form4->Shape14->Brush->Color=clLime;
        //Form4->Label14->Caption = "�������";
        //Form4->Label35->Caption = "������� " + FloatToStrF(dsp_param.aMCLOCK/(1000000),ffFixed, 7,2)+" ���" ;
        if(avt_per )
        fprintf(flog,"\n ���������� ������� �������� ���������. ������� %d",dsp_param.aMCLOCK);
    }

    dsp_param.f_getHM=GetPrivateProfileInt( "���������", "������� ���������� ��", 19000000, fini );
    dsp_param.f_getMONO=GetPrivateProfileInt( "���������", "������� ���������� ����", 21000000, fini );

    dsp_param.MTD=GetPrivateProfileInt( "���������", "��� ��������", 1, fini );
    GetPrivateProfileString( "���������", "�������� ��� ���", "1",ss,256, fini );
    dsp_param.koef[5]=atof(ss);
    dsp_param.per_lhm=GetPrivateProfileInt( "���������", "����� �������� ��", 5, fini );
    dsp_param.per_mono=GetPrivateProfileInt( "���������", "����� �������� ����", 5, fini );
    dsp_param.dal_ogr_HM=GetPrivateProfileInt( "���������", "��������� ����������� ��", 40000, fini );
    GetPrivateProfileString("���������", "����� ����������� ��", "1e1",ss,256, fini );
        dsp_param.con=atof(ss);
    GetPrivateProfileString("���������", "��������� ����������� ��", "1e-4",ss,256, fini );
        dsp_param.sq_con=atof(ss);
    GetPrivateProfileString("���������", "����� ����������� ����", "1e1",ss,256, fini );
        dsp_param.con_m=atof(ss);
    GetPrivateProfileString("���������", "��������� ����������� ����", "1e-4",ss,256, fini );
        dsp_param.sq_con_m=atof(ss);
    GetPrivateProfileString( "���������", "����� �����������", "1",ss,256, fini );
    dsp_param.aSM_THDAC2=atof(ss);
    dsp_param.nak=GetPrivateProfileInt( "���������", "����� ������� ��� ����������", 10, fini );
    dsp_param.HM=GetPrivateProfileInt( "���������", "�� �� ���� ���������", 1, fini );
    dsp_param.WKL_HM=GetPrivateProfileInt( "���������", "�� ��������", 1, fini );
    dsp_param.WKL_MONO=GetPrivateProfileInt( "���������", "���� ��������", 1, fini );
    if(dsp_param.WKL_HM==0 && dsp_param.WKL_MONO==0)
    {
        //MessageBox(NULL,"�� �������� ��������� �� � ���� ","Error",MB_OK);
        dsp_param.WKL_HM=dsp_param.WKL_MONO =1;
    }

    dsp_param.kol_oz=GetPrivateProfileInt( "���������", "����� ������������ ��� �������� ������", 1, fini );

    dsp_param.Nakoplenie=GetPrivateProfileInt( "���������", "���������� ��������", 1, fini );
    dsp_param.NU=GetPrivateProfileInt( "���������", "������ ����", 1, fini );
    dsp_param.VU=GetPrivateProfileInt( "���������", "������� ����", 1, fini );
    if(avt_per )
    {
    fprintf(flog,"\n ��� ��������=%d",dsp_param.MTD);

    fprintf(flog,"\n �������� ��� ���=%f",dsp_param.koef[5]);

    fprintf(flog,"\n ��������� ����������� ��=%d",dsp_param.dal_ogr_HM);

        //Form4->Edit16->Text = FloatToStrF(dsp_param.con,ffFixed,7,2);
    fprintf(flog,"\n ����� ����������� ��=%f",dsp_param.con);

        //Form4->Edit21->Text = FloatToStrF(dsp_param.sq_con,ffExponent,2,2);
    fprintf(flog,"\n ��������� ����������� ��=%f",dsp_param.sq_con);

        //Form4->Edit17->Text = FloatToStrF(dsp_param.con_m,ffFixed,7,2);
    fprintf(flog,"\n ����� ����������� ����=%f",dsp_param.con_m);

        //Form4->Edit27->Text = FloatToStrF(dsp_param.sq_con_m,ffExponent,2,2);
    fprintf(flog,"\n ��������� ����������� ����=%f",dsp_param.sq_con_m);

    //Form4->Edit12->Text = FloatToStrF(dsp_param.aSM_THDAC2,ffFixed,7,2);


    fprintf(flog,"\n �� ��������=%d",dsp_param.WKL_HM);

    fprintf(flog,"\n ���� ��������=%d",dsp_param.WKL_MONO);

    // ���������� ����� ������ �����

    fprintf(flog,"\n ���������� ��������=%d",dsp_param.Nakoplenie);
    //if(dsp_param.WKL_HM==0 && dsp_param.WKL_MONO==0)
    //{
    //    MessageBox(NULL,"�� �������� ��������� �� � ���� ","Error",MB_OK);
    //    dsp_param.WKL_HM=dsp_param.WKL_MONO =1;
    //}

    
    fprintf(flog,"\n ������ ���� ��������=%d",dsp_param.NU);
    fprintf(flog,"\n ������� ���� ��������=%d",dsp_param.VU);
    }
    // ���������� ����� ������ �����
    if(dsp_param.NU==0 && dsp_param.VU ==0)
      {
        //MessageBox(NULL,"�� �������� ������ � ������� ����","Error",MB_OK);
        //return 1;
         dsp_param.NU=1;  dsp_param.VU =1;
      }

    dsp_param.median=GetPrivateProfileInt( "���������", "��������� ����������", 1, fini );
     GetPrivateProfileString( "���������", "����� ������", "4.6",ss,256, fini );
    T_obs=dsp_param.tobs=atof(ss);
    nKK=GetPrivateProfileInt( "���������", "����� �������� ����� �����", 3, fini );
    nT=GetPrivateProfileInt( "���������", "����������� ���������", 3, fini );
    GetPrivateProfileString( "���������", "�������� �������", "10",ss,256, fini );
        sig=atof(ss);
    dsp_param.N_Buf_Oz= GetPrivateProfileInt( "���������", "����� ������ ������ ���������", 18, fini );
    dsp_param.met_o=GetPrivateProfileInt( "���������", "����� �����", 0, fini );
    dsp_param.meteo=GetPrivateProfileInt( "���������", "������ �����", 4, fini );
    dsp_param.koef[1]=GetPrivateProfileInt( "���������", "�������� 0 �����", 1, fini );
    dsp_param.koef[0]=GetPrivateProfileInt( "���������", "�������� ���", 1, fini );
    dsp_param.koef[2]=GetPrivateProfileInt( "���������", "�������� 1 �����", 1, fini );
    dsp_param.koef[3]=GetPrivateProfileInt( "���������", "�������� 2 �����", 1, fini );
    dsp_param.koef[4]=GetPrivateProfileInt( "���������", "�������� 3 �����", 1, fini );
    dsp_param.kor_as=GetPrivateProfileInt( "���������", "��������� �������", 0, fini );
    if(avt_per )
    {
    fprintf(flog,"\n ��������� ���������� ��������=%d",dsp_param.median);
    fprintf(flog,"\n ����� ������=%f",T_obs);


    if( dsp_param.N_Buf_Oz>18)
    {
        fprintf(flog,"\n����� ������ ������ ���������>18. ����������� =18\n");
        dsp_param.N_Buf_Oz=18;
    }

    fprintf(flog,"\n ����� �����=%d",dsp_param.met_o);

    fprintf(flog,"\n ������ �����=%d",dsp_param.meteo);

    fprintf(flog,"\n �������� ���=%f",dsp_param.koef[0]);


    fprintf(flog,"\n �������� 0 �����=%f",dsp_param.koef[1]);

    fprintf(flog,"\n �������� 1 �����=%f",dsp_param.koef[2]);

    fprintf(flog,"\n �������� 2 �����=%f",dsp_param.koef[3]);
    
    fprintf(flog,"\n �������� 3 �����=%f",dsp_param.koef[4]);
    
    fprintf(flog,"\n ��������� ������� � ���",dsp_param.kor_as);
    }
    
    // ************   ������ ���������    ***********************************************************************************

    GetPrivateProfileString( "���������", "����� ��� ����� �����", "10",ss,256, fini );
        dsp_param.porog=atof(ss);
        //Form4->Edit7->Text =FloatToStrF(dsp_param.porog,ffFixed,7,2);
    

    dsp_param.NR=GetPrivateProfileInt( "���������", "������ ����� ����� ��", 512, fini );

	if(dsp_param.NR<=0 || dsp_param.NR>4096)dsp_param.NR=512;
        //Form4->Edit6->Text = IntToStr(dsp_param.NR);

    dsp_param.NR_M=GetPrivateProfileInt( "���������", "������ ����� ����� ����", 512, fini );
	if(dsp_param.NR_M<=0 || dsp_param.NR_M>4096)dsp_param.NR_M=512;
        //Form4->Edit9->Text = IntToStr(dsp_param.NR_M);


    GetPrivateProfileString( "���������", "��������� �������������", "0.001 0.001",ss,256, fini );
    sscanf(ss,"%f%f",&dsp_param.PL_K_Phm,&dsp_param.PL_K_Pmono);
    fprintf(flog,"\n ��������� ������������� ��=%f",dsp_param.PL_K_Phm);
    fprintf(flog,"\n ��������� ������������� ����=%f",dsp_param.PL_K_Pmono);

    GetPrivateProfileString( "���������", "����������� ����������� ����� ����� ��", "0.9",ss,256, fini );
    dsp_param.beta=atof(ss);


    GetPrivateProfileString( "���������", "����������� ����������� ����� ����� ����", "0.9",ss,256, fini );
        dsp_param.beta_MONO=atof(ss);
    GetPrivateProfileString( "���������", "��������� ����", "0.1",ss,256, fini );
        okno.mnoj_okna=atof(ss);
    //GetPrivateProfileString( "���������", "��������� ����", "0",ss,256, fini );
    //    okno.mno=atof(ss);
    okno.prop=GetPrivateProfileInt( "���������", "����� ���������", 2, fini );
    okno.L_Buf=GetPrivateProfileInt( "���������", "������ ������", 10, fini );
    okno.n=GetPrivateProfileInt( "���������", "�������", 3, fini );
    GetPrivateProfileString( "���������", "��������� ���� ����", "0.1",ss,256, fini );
        oknom.mnoj_okna=atof(ss);
    //GetPrivateProfileString( "���������", "��������� ����", "0",ss,256, fini );
    //    okno.mno=atof(ss);
    oknom.prop=GetPrivateProfileInt( "���������", "����� ��������� ����", 2, fini );
    oknom.L_Buf=GetPrivateProfileInt( "���������", "������ ������ ����", 10, fini );
    oknom.n=GetPrivateProfileInt( "���������", "������� ����", 3, fini );
    tip_obr= GetPrivateProfileInt( "���������", "��� ���������", 0, fini );
    if(avt_per )
    {
    fprintf(flog,"\n 3 ����� ��� ������ � ����� �����=%f",dsp_param.porog);
    fprintf(flog,"\n ������ ����� �����=%d",dsp_param.NR);
    //sscanf(ss,"%f%f",&dsp_param.PL_K_Phm,&dsp_param.PL_K_Pmono);
    //fprintf(flog,"\n ��������� ������������� ��=%f",dsp_param.PL_K_Phm);
    //fprintf(flog,"\n ��������� ������������� ����=%f",dsp_param.PL_K_Pmono);
    fprintf(flog,"\n ����������� ����������� ����� �����=%f",dsp_param.beta);
    fprintf(flog,"\n ��� ���������=%d",tip_obr);
    }

// ***************   ������ �����������   *****************************************************************************


    GetPrivateProfileString( "�����������", "����� �����������", "600",ss,256, fini );
 	dsp_param.PorogL=atof(ss);
    

    GetPrivateProfileString( "�����������", "��������� �����������", "1",ss,256, fini );
 	dsp_param.delta=atof(ss);
    GetPrivateProfileString( "�����������", "��������� ������ ��� ����", "1",ss,256, fini );
 	dsp_param.energij=atof(ss);
    dsp_param.float_to_int=GetPrivateProfileInt( "�����������", "����� �� �����", 0, fini );
    // ������ � ����������� ���������� ���� �� �����
    dsp_param.d_Wary1=GetPrivateProfileInt( "�����������", "������������ ��������� ��������� ����������� �������� �� ��", 50000, fini );
    dsp_param.d_Wary2=GetPrivateProfileInt( "�����������", "������������ ��������� ��������� ����������� �������� �� ��", 50000, fini );
    dsp_param.d_Wary1m=GetPrivateProfileInt( "�����������", "������������ ��������� ��������� ����������� �������� ���� ��", 20000, fini );
    dsp_param.d_Wary2m=GetPrivateProfileInt( "�����������", "������������ ��������� ��������� ����������� �������� ���� ��", 20000, fini );
    dsp_param.wid1=GetPrivateProfileInt( "�����������", "��� ������ ���� ��  ��", 0, fini );
    dsp_param.wid2=GetPrivateProfileInt( "�����������", "��� ������ ���� ��  ��", 0, fini );
    dsp_param.wid1m=GetPrivateProfileInt( "�����������", "��� ������ ���� ��  ����", 0, fini );
    dsp_param.wid2m=GetPrivateProfileInt( "�����������", "��� ������ ���� ��  ����", 0, fini );
    GetPrivateProfileString("�����������", "�������� � ���� ���� �� ��", "0",ss,256, fini );
    dsp_param.us0hm1=atof(ss);
    GetPrivateProfileString("�����������", "�������� � ���� ���� �� ��", "0",ss,256, fini );
    dsp_param.us0hm2=atof(ss);
    GetPrivateProfileString("�����������", "�������� � ���� ���� ���� ��", "0",ss,256, fini );
    dsp_param.us0mono1=atof(ss);
    GetPrivateProfileString("�����������", "�������� � ���� ���� ���� ��", "0",ss,256, fini );
    dsp_param.us0mono2=atof(ss);
    Wkl_Sop=GetPrivateProfileInt( "�����������", "������", 0, fini );
    if(avt_per )
    {
    fprintf(flog,"\n ����� �����������=%f",dsp_param.PorogL);
    fprintf(flog,"\n ��������� �����������=%f",dsp_param.delta);
    fprintf(flog,"\n ��������� ������ ��� ����=%f",dsp_param.energij);
    fprintf(flog,"\n ������������ ��������� ��������� ����������� �������� �� ��=%d",dsp_param.d_Wary1);
    fprintf(flog,"\n ������������ ��������� ��������� ����������� �������� �� ��=%d",dsp_param.d_Wary2);
    fprintf(flog,"\n ������������ ��������� ��������� ����������� �������� ���� ��=%d",dsp_param.d_Wary1m);
    fprintf(flog,"\n ������������ ��������� ��������� ����������� �������� ���� ��=%d",dsp_param.d_Wary2m);
    fprintf(flog,"\n ���� ��  �� ��������=%d",dsp_param.wid1);
    fprintf(flog,"\n ���� D�  �� ��������=%d",dsp_param.wid2);
    fprintf(flog,"\n ���� ��  ���� ��������=%d",dsp_param.wid1m);
    fprintf(flog,"\n ���� ��  ���� ��������=%d",dsp_param.wid2m);
    fprintf(flog,"\n �������� � ���� ���� �� ��=%f",dsp_param.us0hm1);
    fprintf(flog,"\n �������� � ���� ���� �� ��=%f",dsp_param.us0hm2);
    fprintf(flog,"\n �������� � ���� ���� ���� ��=%f",dsp_param.us0mono1);
    fprintf(flog,"\n �������� � ���� ���� ���� ��=%f",dsp_param.us0mono2);
    fprintf(flog,"\n ������ ������������� �������=%d",Wkl_Sop);
    }

// ******************   ������ ����� ���������  **********************************************************************

    dsp_param.zad_lhm=GetPrivateProfileInt( Section.c_str(), "�������� ���� ��������� ��", 10, fini );
    dsp_param.zad_mono=GetPrivateProfileInt( Section.c_str(), "�������� ���� ��������� ����", 10, fini );

    // ������� ����� ��� ������ � ���
    GetPrivateProfileString( Section.c_str(), "���� ��������� ������� �� ��", ".\\dann\\Ishod_LHM_1.txt",imj_file_test_hm1,256, fini );
    GetPrivateProfileString( Section.c_str(), "���� ��������� ������� �� ��", ".\\dann\\Ishod_LHM_2.txt",imj_file_test_hm2,256, fini );
    GetPrivateProfileString( Section.c_str(), "���� ��������� ������� ���� ��", ".\\dann\\Ishod_MONO_1.txt",imj_file_test_mono1,256, fini );
    GetPrivateProfileString( Section.c_str(), "���� ��������� ������� ���� ��", ".\\dann\\Ishod_MONO_2.txt",imj_file_test_mono2,256, fini );

    GetPrivateProfileString( Section.c_str(), "���� ������� ������ �� ��", ".\\dann\\Filter_nat_hm_1.txt",imj_file_cjat_hm1,256, fini );
    GetPrivateProfileString( Section.c_str(), "���� ������� ������ �� ��", ".\\dann\\Filter_nat_hm_2.txt",imj_file_cjat_hm2,256, fini );
    GetPrivateProfileString( Section.c_str(), "���� ������� ������ ���� ��", ".\\dann\\Filter_nat_mono_1.txt",imj_file_cjat_mono1,256, fini );
    GetPrivateProfileString( Section.c_str(), "���� ������� ������ ���� ��", ".\\dann\\Filter_nat_mono_2.txt",imj_file_cjat_mono2,256, fini );

    GetPrivateProfileString( "�����", "���� ��� �������� MTD", ".\\dann\\pci5_lhm_4__0.1GZ_cplx.txt",imj_file_mtd,256, fini );
    GetPrivateProfileString( "�����", "��������� ���", "100_15625.prg",prog_ddc,256, fini );







// ******************   ������ ����  **********************************************************************
        
        TEST=GetPrivateProfileInt( "����", "������������", 0, fini );
        
        dsp_param.nom_hm=GetPrivateProfileInt( "����", "����� ��", 1868, fini );
        dsp_param.nom_mono=GetPrivateProfileInt( "����", "����� ����", 279, fini );

        amp_hm=GetPrivateProfileInt( "����", "��������� ��", 1, fini );
        amp_mono=GetPrivateProfileInt( "����", "��������� ����", 1, fini );

        S_Okno= GetPrivateProfileInt( "����", "������� ����", 0, fini );
        otladka_otm=GetPrivateProfileInt( "����", "������� �������������", 0, fini );
        if(otladka_otm)
                tt=fopen("tt.txt","w");
        sap_ozenki=GetPrivateProfileInt( "����", "������ ������ � ����", 0, fini );
        //if(Oz_file)
        //        foz=fopen("oz.txt","w");
        if(avt_per )
        {
        fprintf(flog,"\n �������� ������������=%d",TEST);
        fprintf(flog,"\n ����� ��������� ������� ��=%d",dsp_param.nom_hm);
        fprintf(flog,"\n ����� ��������� ������� MONO=%d",dsp_param.nom_mono);
        fprintf(flog,"\n ��������� ��������� ������� ��=%d",amp_hm);
        fprintf(flog,"\n ��������� ��������� ������� MONO=%d",amp_mono);
        }

// ******************   ������ ��������  **********************************************************************
    dsp_param.otladka=GetPrivateProfileInt( "��������", "��������", 0, fini );
    //if( dsp_param.otladka)

    dsp_param.tip_filt= GetPrivateProfileInt( "��������", "��� ������� ��", 1, fini );
    GetPrivateProfileString( "��������", "������ ��", "2.5",ss,256, fini ); // � ��������
    double da;
    da=atof(ss);
    dsp_param.da=(float)da;


// ���� ������ ���� �������� � ��������
    dsp_param.DALN=GetPrivateProfileInt( "��������", "������ ���� ��", 225, fini ); // � ��������
    dsp_param.DALN=dsp_param.DALN*4096.0/360.0-47;
    if(dsp_param.DALN<0)
        dsp_param.DALN+=4096;
    dsp_param.DALNm=GetPrivateProfileInt( "��������", "������ ���� ����", 225, fini ); // � ��������
    dsp_param.DALNm=dsp_param.DALNm*4096.0/360.0-47;
    if(dsp_param.DALNm<0)
        dsp_param.DALNm+=4096;
// ���� ������ ���� �������� � ��������
    dsp_param.Das = GetPrivateProfileInt( "��������", "������ 2 ���� �� ������������ 1", 5, fini );  // � ��������
    dsp_param.Das = dsp_param.Das*4096.0/360.0;
    dsp_param.Dasm = GetPrivateProfileInt( "��������", "������ 2 ���� ���� ������������ 1", 5, fini );  // � ��������
    dsp_param.Dasm = dsp_param.Dasm*4096.0/360.0;
// ���������� ���� ����� � ��������
    GetPrivateProfileString( "��������", "���������� ���� 1 ���� ��", "1",ss,256, fini );
    dsp_param.deltaas1HM =atof(ss)*4096.0/360.0;
    GetPrivateProfileString( "��������", "���������� ���� 2 ���� ��", "1",ss,256, fini );
    dsp_param.deltaas2HM =atof(ss)*4096.0/360.0;
    GetPrivateProfileString( "��������", "���������� ���� 1 ���� ����", "1",ss,256, fini );
    dsp_param.deltaas1MONO =atof(ss)*4096.0/360.0;
    GetPrivateProfileString( "��������", "���������� ���� 2 ���� ����", "1",ss,256, fini );
    dsp_param.deltaas2MONO =atof(ss)*4096.0/360.0;
// ��������� 1-� ���� � ���-����� � ������
    double dr;
    dsp_param.aDECIMATION=GetPrivateProfileInt( "���", "��������� ��� ��", 64, fini );
    dr=dsp_param.aDECIMATION*1.5e8/dsp_param.aMCLOCK;
    dsp_param.DMax=GetPrivateProfileInt( "��������", "��������� ���� ��", 70000, fini );
    dsp_param.DMax/=dr;

    dsp_param.DMaxm=GetPrivateProfileInt( "��������", "��������� ���� ����", 18000, fini );
    if(dsp_param.aDECIMONO==32)
        dsp_param.DMaxm/=0.5*dr;
    else
        dsp_param.DMaxm/=dr;
// ��������� ������ ���� ������������ 1 � ��������
    dsp_param.Ddalm = GetPrivateProfileInt( "��������", "��������� 2 ���� ���� ������������ 1", 5, fini );
    dsp_param.Ddal=GetPrivateProfileInt( "��������", "��������� 2 ���� �� ������������ 1", 5, fini );
// ���������� ��������� ���� � ��������
    dsp_param.DELTAD=GetPrivateProfileInt( "��������", "���������� ��������� ��", 10, fini );
    dsp_param.DELTADm=GetPrivateProfileInt( "��������", "���������� ��������� ����", 10, fini );
    dsp_param.DELTAD2=GetPrivateProfileInt( "��������", "���������� ��������� �� 2", 10, fini );
    dsp_param.DELTAD2m=GetPrivateProfileInt( "��������", "���������� ��������� ���� 2", 10, fini );
// ��������� 2-� ���� � ���-����� � ������
    //dsp_imitat.daln2 = GetPrivateProfileInt( "��������", "��������� ����2", 2000, fini );

    GetPrivateProfileString( "��������", "��������� ������� ��", "1",ss,256, fini );
    dsp_param.amplituda =atof(ss);
    GetPrivateProfileString( "��������", "��������� ������� ����", "1",ss,256, fini );
    dsp_param.amplitudam =atof(ss);
    GetPrivateProfileString( "��������", "��������� ������� �� 2", "1",ss,256, fini );
    dsp_param.amplituda2 =atof(ss);
    GetPrivateProfileString( "��������", "��������� ������� ���� 2", "1",ss,256, fini );
    dsp_param.amplituda2m =atof(ss);
// ��� ��������� ���� � ���-����� � ������
    //dsp_imitat.min_daln = GetPrivateProfileInt( "��������", "��� ��������� ��", 20000, fini );
    dsp_param.md=GetPrivateProfileInt( "��������", "��� ��������� ��", 20000, fini );
    //if(dsp_param.md>dsp_param.Daln_MONO)
        dsp_param.md /= 96;
    //else
    //{
    //    if(dsp_param.aDECIMONO==32)
    //            dsp_param.md /= 48;
    //    else
    //            dsp_param.md /= 96;
    //}
// ������� ������� 1-� ���� �� � ���-����� � ��
    dsp_param.fd  = GetPrivateProfileInt( "��������", "������� ������� ���� 1 ��", 0, fini );
// ������� ������� 2-� ���� �� � ���-����� � ��
    dsp_param.fd2 = GetPrivateProfileInt( "��������", "������� ������� ���� 2 ��", 0, fini );
// ������� ������� 1-� ���� ���� � ���-����� � ��
    dsp_param.fdm  = GetPrivateProfileInt( "��������", "������� ������� ���� 1 ����", 0, fini );
// ������� ������� 2-� ���� ���� � ���-����� � ��
    dsp_param.fd2m = GetPrivateProfileInt( "��������", "������� ������� ���� 2 ����", 0, fini );

// �������� 1-� ���� � ���-����� � ��/�
    //dsp_imitat.vr1 = GetPrivateProfileInt( "��������", "�������� ����1", 0, fini );
// �������� 2-� ���� � ���-����� � ��/�
    //dsp_imitat.vr2 = GetPrivateProfileInt( "��������", "�������� ����2", 0, fini );
//////////////

// ���� ������ ���� �������� � ��������
    //dsp_param.DALNm = GetPrivateProfileInt( "��������", "������ ���� MONO", 300, fini ); // � ��������
    //dsp_param.DALNm*=(4096.0/360.0);
    //dsp_param.DALNm -=47;
    //if(dsp_param.DALNm <0)
    //     dsp_param.DALNm +=4096;
// ���� ������ ���� �������� � ��������
    //dsp_param.Dasm = GetPrivateProfileInt( "��������", "������ ����2MONO", 250, fini );  // � ��������
    //dsp_param.Dasm =dsp_param.DALNm - dsp_param.Dasm*(4096.0/360.0);
// ��������� 1-� ���� � ���-����� � ������
    //dsp_param.DMaxm = GetPrivateProfileInt( "��������", "��������� ���� MONO", 18000, fini );
    //dsp_param.DMaxm=(dsp_param.aDECIMONO==32)?(dsp_param.DMaxm/48):(dsp_param.DMaxm/96);
// ��������� 2-� ���� � ���-����� � ������

    //dsp_param.DMaxm-(dsp_param.aDECIMONO==32)?(dsp_param.Ddalm/48):(dsp_param.Ddalm/96);

    //GetPrivateProfileString( "��������", "��������� ������� MONO", "1",ss,256, fini );
    //dsp_param.amplitudam =atof(ss);
    //GetPrivateProfileString( "��������", "��������� ������� MONO 2", "1",ss,256, fini );
    //dsp_param.amplituda2m =atof(ss);

// ��� ��������� ���� � ���-����� � ������
    //dsp_imitat.min_dalnm = GetPrivateProfileInt( "��������", "��� ��������� �����MONO", 20000, fini );

// ������� ������� 1-� ���� � ���-����� � ��
    //dsp_param.fd  = GetPrivateProfileInt( "��������", "������� ������� ����1", 0, fini );
// ������� ������� 2-� ���� � ���-����� � ��
    //dsp_param.fd2 = GetPrivateProfileInt( "��������", "������� ������� ����2", 0, fini );

// �������� 1-� ���� � ���-����� � ��/�
    //dsp_imitat.vr1 = GetPrivateProfileInt( "��������", "�������� ����1", 0, fini );
// �������� 2-� ���� � ���-����� � ��/�
    //dsp_imitat.vr2 = GetPrivateProfileInt( "��������", "�������� ����2", 0, fini );
    //dsp_param.DELTAD=GetPrivateProfileInt( "��������", "���������� ��������� ��", 10, fini );
    //dsp_param.DELTADm=GetPrivateProfileInt( "��������", "���������� ��������� ����", 10, fini );
    //dsp_param.DELTAD2m=10;
///////////////

// ������� ������ ��������� ��� ��������
    //perevod_ed();
    //otobrajenie = GetPrivateProfileInt( "���������", "����������� �� �����", 1, fini );
/*
// ******************   ������ �������  **********************************************************************
    // ��������� �������
   GetPrivateProfileString( "�������", "���������", "160000",ss,256, fini );
   Daln=atof(ss);
   GetPrivateProfileString( "�������", "��������", "0",ss,256, fini );
   VR=atof(ss);
   vrm=VR/3.6;
*/

   fprintf(flog,"\n\n **************** ��������� �� ������� ************************\n\n");

 // *******************************************************************************
    // ��������� �������� ������
 // *******************************************************************************

    //if(kom!=0)
    {
        //if(OpenDialog1->Execute())
        {
            fq=fopen(imj_file_test_hm1,"r");
            if(fq==NULL)
            {
                //Nom_Kml=
                MessageBox(NULL,imj_file_test_hm1,"������� ��� ����� �������� �������� �� 1",MB_OK);
                return 1;
            }
            fscanf(fq,"%d",&k);//���������� �������� �� 1 ������
            test_sig[0]=complex<short int>(k,k);
            if(k<=0 || k>400)
            {
                MessageBox(NULL,imj_file_test_hm1,"������� ������ �  ����� �������� ��������",MB_OK);
                return 1;
            }

            for(int i=0;i<k;i++)
            {
                //fscanf(fq,"%d%d",&k2,&k3);
                fscanf(fq,"%lf%lf",&x1,&x2);
                k2=x1/32768.0 * amp_hm;
                k3=x2/32768.0 * amp_hm;
                test_sig[i+1]=complex<short int>(k2,k3);
                //k2=k;k3=k1;
               //test_sig[i+1]=complex<short int>(k2>>1,k3>>1);
            }
            fclose(fq);

 //s1=new complex<double>[N0];
 //for(int i=1;i<k;i++)
 //       s1[i-1]=test_sig[i];
 //Graf(s1,N0,FD,0,"",0,k/FD,0);
            fq=fopen(imj_file_test_hm2,"r");
            if(fq==NULL)
            {
                MessageBox(NULL,imj_file_test_hm2,"������� ��� ����� �������� �������� �� 2",MB_OK);
                return 1;
            }
            fscanf(fq,"%d",&k4);//���������� �������� �� 2 ������
            if(k4<=0 || k4>400)
            {
                MessageBox(NULL,imj_file_test_hm2,"������� ������ �  ����� �������� ��������",MB_OK);
                return 1;
            }
            test_sig[k+1]=complex<short int>(k4,k4);
            k+=2;

            for(int i=0;i<k4;i++)
            {
                //fscanf(fq,"%d%d",&k2,&k3);
                //test_sig[i+k]=complex<short int>(k2>>1,k3>>1);
                fscanf(fq,"%lf%lf",&x1,&x2);
                k2=x1/32768.0 * amp_hm;
                k3=x2/32768.0 * amp_hm;
                test_sig[i+k]=complex<short int>(k2,k3);
            }
            k+=k4;
            fclose(fq);

            fq=fopen(imj_file_test_mono1,"r");
            if(fq==NULL)
            {
                MessageBox(NULL,imj_file_test_mono1,"������� ��� ����� �������� �������� ���� 1",MB_OK);
                return 1;
            }
            fscanf(fq,"%d",&k4);//���������� �������� ���� 1 ������
            if(k4<=0 || k4>400)
            {
                MessageBox(NULL,imj_file_test_mono1,"������� ������ �  ����� �������� ��������",MB_OK);
                return 1;
            }
            //k+=k4+1;
            test_sig[k]=complex<short int>(k4,k4);
            for(int i=0;i<k4;i++)
            {
                //fscanf(fq,"%d%d",&k2,&k3);
                //test_sig[i+1+k]=complex<short int>(k2>>1,k3>>1);
                fscanf(fq,"%lf%lf",&x1,&x2);
                k2=x1/32768.0 * amp_mono;
                k3=x2/32768.0 * amp_mono;
                test_sig[i+1+k]=complex<short int>(k2,k3);
            }
            fclose(fq);

            fq=fopen(imj_file_test_mono2,"r");
            if(fq==NULL)
            {
                MessageBox(NULL,imj_file_test_mono2,"������� ��� ����� �������� �������� ���� 2",MB_OK);
                return 1;
            }
            k+=k4+1;
            fscanf(fq,"%d",&k4);//���������� �������� ���� 2 ������
            if(k4<=0 || k4>400)
            {
                MessageBox(NULL,imj_file_test_mono2,"������� ������ �  ����� �������� ��������",MB_OK);
                return 1;
            }
            
            test_sig[k]=complex<short int>(k4,k4);
            for(int i=0;i<k4;i++)
            {
                //fscanf(fq,"%d%d",&k2,&k3);
                //test_sig[i+1+k]=complex<short int>(k2>>1,k3>>1);
                fscanf(fq,"%lf%lf",&x1,&x2);
                k2=x1/32768.0 * amp_mono;
                k3=x2/32768.0 * amp_mono;
                test_sig[i+1+k]=complex<short int>(k2,k3);
            }
            fclose(fq);fq=NULL;
        }

        x3=1/sqrt(L_LEN);
 // *******************************************************************************
    // ��������� ������ ������ - � ��� ����� ���� ��� �������???
 // *******************************************************************************
        //if(OpenDialog1->Execute())
        {
            //fq=fopen(OpenDialog1->FileName.c_str(),"r");
            fq=fopen(imj_file_cjat_hm1,"r");
            if(fq==NULL)
            {
                MessageBox(NULL,imj_file_cjat_hm1,"������� ��� ����� �������� ������ HM1",MB_OK);
                return 1;
            }
            for(int i=0;i<L_LEN;i++)
            {
                fscanf(fq,"%lf%lf",&x1,&x2); //x1*=x3;x2*=x3;
                KU_HM_NU[i]=complex<float>(x1,x2);
            }
            fclose(fq);

            fq=fopen(imj_file_cjat_hm2,"r");
            if(fq==NULL)
            {
                MessageBox(NULL,imj_file_cjat_hm2,"������� ��� ����� �������� ������ HM2",MB_OK);
                return 1;
            }
            for(int i=0;i<L_LEN;i++)
            {
                fscanf(fq,"%lf%lf",&x1,&x2);  //x1*=x3;x2*=x3;
                KU_HM_VU[i]=complex<float>(x1,x2);
            }
            fclose(fq);

            fq=fopen(imj_file_cjat_mono1,"r");
            if(fq==NULL)
            {
                MessageBox(NULL,imj_file_cjat_mono1,"������� ��� ����� �������� ������ MONO1",MB_OK);
                return 1;
            }
            x3=1/sqrt(M_LEN);
            for(int i=0;i<M_LEN;i++)
            {
                fscanf(fq,"%lf%lf",&x1,&x2); //x1*=x3;x2*=x3;
                KU_MONO_NU[i]=complex<float>(x1,x2);
            }
            fclose(fq);
            fq=fopen(imj_file_cjat_mono2,"r");
            if(fq==NULL)
            {
                MessageBox(NULL,imj_file_cjat_mono2,"������� ��� ����� �������� ������ MONO2",MB_OK);
                return 1;
            }
            for(int i=0;i<M_LEN;i++)
            {
                fscanf(fq,"%lf%lf",&x1,&x2); //x1*=x3;x2*=x3;
                KU_MONO_VU[i]=complex<float>(x1,x2);
            }
        }
        fclose(fq);fq=NULL;


   // *******************************************************************************
      //  ��������� ������� MTD
   // *******************************************************************************
    //if(OpenDialog1->Execute())
    {
        fq=fopen(imj_file_mtd,"r");
        if(fq==NULL)
        {
            MessageBox(NULL,imj_file_mtd,"������� ��� ����� ��D",MB_OK);
            return 1;
        }
        if(dsp_param.per_lhm>10)
        {
            MessageBox(NULL,"������� ����� �������� (����<=10)","Error",MB_OK);
            return 1;
        }
        for(unsigned int i=0;i<dsp_param.per_lhm+1;i++)
        {
            fscanf(fq,"%d",&Period[i]);
            //x1=Period[i]*0.1+0.05;
            //Period[i]=x1;
        }
        LL=Period[dsp_param.per_lhm];
// ������������ ������� �� ����
        for(unsigned int i=0;i<nK*LL*dsp_param.per_lhm;i++)
        {
            fscanf(fq,"%lf%lf",&x1,&x2);
            Filter_MTD[2*i]=complex<float>(x1,x2);
            Filter_MTD[2*i+1]=complex<float>(x1,x2);
        }
    }
    fclose(fq); fq=NULL;
    }

 // *******************************************************************************
    // �������� ��������� ���
 // *******************************************************************************

    //ProgrammPath=ExtractFilePath(Application->ExeName);
    //SetCurrentDir(ProgrammPath);
    
		ProgrammPath = GetCurrentDirectoryMC();
		SetCurrentDirectoryA(ProgrammPath.c_str());
		
		ProgPath= ProgrammPath+ prog_ddc ;
    // --- TODO  items=ReadPrgFile(ProgPath.c_str() /*prog_ddc*/, g_aProgArray, sizeof(g_aProgArray)/sizeof(g_aProgArray[0] ));
    if(items==-1)
    {
        sprintf(ss,"������ ������� ����: %s",  prog_ddc);
        MessageBox(NULL,ss,"Error",MB_OK);
        //BRD_cleanup();
        return 1;
    }


    items+=4;
    items&=0xfffffffc;
    dsp_param.items=items;
    copy_struct();
    /*
    if(dsp_param.sost_upr==1)
    {
                Form4->Button7->Caption="�� ����������";

                Form4->Button8->Visible=true;
                Form4->Button11->Visible=true;
                Form4->Button13->Visible=true;
                Form4->Button33->Visible=true;
                Form4->Button12->Visible=true;
                //Form4->Button18->Visible=true;
                //Form4->Button22->Visible=true;
                Form4->Button3->Enabled=true;
    }
    else
    {
                Form4->Button7->Caption="�������������";

                Form4->Button8->Visible=false;
                Form4->Button11->Visible=false;
                Form4->Button13->Visible=false;
                Form4->Button33->Visible=false;
                Form4->Button12->Visible=false;
                Form4->Button18->Visible=false;
                Form4->Button22->Visible=false;
                Form4->Button3->Enabled=false;
    }
    */
    /*
    if(dsp_param.met_o==1)// ��������� �����
    {
                Form4->Shape15->Brush->Color=clLime;//RadioGroup1->ItemIndex=0;
                switch(dsp_param.meteo)
                {
                        case 1:
                                Form4->Shape16->Brush->Color=clYellow;
                                Form4->Shape17->Brush->Color=clRed;
                                Form4->Shape18->Brush->Color=clRed;
                                Form4->Shape19->Brush->Color=clRed;
                                break;
                         case 2:
                                Form4->Shape16->Brush->Color=clRed;
                                Form4->Shape17->Brush->Color=clYellow;
                                Form4->Shape18->Brush->Color=clRed;
                                Form4->Shape19->Brush->Color=clRed;
                                break;
                         case 3:
                                Form4->Shape16->Brush->Color=clRed;
                                Form4->Shape17->Brush->Color=clRed;
                                Form4->Shape18->Brush->Color=clYellow;
                                Form4->Shape19->Brush->Color=clRed;
                                break;
                         case 4:
                                Form4->Shape16->Brush->Color=clRed;
                                Form4->Shape17->Brush->Color=clRed;
                                Form4->Shape18->Brush->Color=clRed;
                                Form4->Shape19->Brush->Color=clYellow;
                                break;
                }
            }
            if(dsp_param.met_o==0) // �������� �����
            {
                Form4->Shape15->Brush->Color=clRed;//RadioGroup1->ItemIndex=0;
                switch(dsp_param.meteo)
                {
                        case 1:
                                Form4->Shape16->Brush->Color=clLime;
                                Form4->Shape17->Brush->Color=clRed;
                                Form4->Shape18->Brush->Color=clRed;
                                Form4->Shape19->Brush->Color=clRed;
                                break;
                         case 2:
                                Form4->Shape16->Brush->Color=clRed;
                                Form4->Shape17->Brush->Color=clLime;
                                Form4->Shape18->Brush->Color=clRed;
                                Form4->Shape19->Brush->Color=clRed;
                                break;
                         case 3:
                                Form4->Shape16->Brush->Color=clRed;
                                Form4->Shape17->Brush->Color=clRed;
                                Form4->Shape18->Brush->Color=clLime;
                                Form4->Shape19->Brush->Color=clRed;
                                break;
                         case 4:
                                Form4->Shape16->Brush->Color=clRed;
                                Form4->Shape17->Brush->Color=clRed;
                                Form4->Shape18->Brush->Color=clRed;
                                Form4->Shape19->Brush->Color=clLime;
                                break;
                }
            }
    */
    return 0;
}



void copy_struct()
{
 /*     ****
       �������� ��������� dsp_param ��� �������������� �������� (���� � ��� �����) � ������ ������ �� �����
*/
    unsigned int size;
    int *q1,*q2;

    size=sizeof(dsp_param);
    size&=0xfffffff0;

    // ��������� �������� ��� ���������
    q1=(int*)&dsp_param;
    q2=(int*)&dsp_param_file;

     for(unsigned int i=0;i<size/sizeof(int);i++)
       q2[i]= q1[i];
}

/*
void exit()
{

//     ������� ���������� ������ � ������

    U32 err;
    double tim;
    err=0;
    tim=Now();
        fprintf(flog,"\n%-60s%s","����� ������ ",DateTimeToStr(tim).c_str());
        fclose(flog);
    Exit=1;
}
*/
/*
char *S[256]={"����� ������","����� ������ ���������","������ �������� ���� ���������","����� ��� ������ � ����� �����",
"��������� ������������� �� � ����","����������� ����������� ����� ����� ��","����� ����������� ��","����� �� �����",
"��������� ��������� ���� 1 ������ �� (��)","��������� ��������� ���� 2 ������ �� (��)","��������� ��������� ���� 1 ������ ���� (��)",
"��������� ��������� ���� 2 ������ ���� (��)","��������� ������ ���� 1 ������ �� (��)","��������� ������ ���� 2 ������ �� (��)",
"��������� ������ ���� 1 ������ ���� (��)","��������� ������ ���� 2 ������ ���� (��)","����� ����������� ��","����� ����������� MONO",
"��������� ����������","Test ����� HM","Test ����� MONO","�������� ������������","����� ������� ��� ���������� ",
"�������� � ���� ���� �� ��","�������� � ���� ���� �� ��","�������� � ���� ���� ���� ��","�������� � ���� ���� ���� ��",
"����������� ������ ��� ����","��������� NU","��������� VU","��������� ����������� ��","��������� ����������� ����",
"������������� ����������","���/���� ���","���/���� ����������","���/���� ��","���/���� ����","����������� �������� �����",
"���/���� ����� �����","�������� ������� ������ ���������� � ������ �����","�������� ������������ �������� ��� �����������",
"�������� ���������.���������� �������������","������ � ���� �����","������ � ���� �����","������ � ����","�������� ����� �����"};
*/
char S[70][80];
/*
//S[0]=
strcpy(S[0],"����� ������");
//S[1]=
strcpy(S[1],"����� ������ ���������");
//S[2]=
strcpy(S[2],"������ �������� ���� ���������");
//S[3]=
strcpy(S[3],"����� ��� ������ � ����� �����");
//S[4]=
strcpy(S[4],"��������� ������������� �� � ����");
//S[5]=
strcpy(S[5],"����������� ����������� ����� ����� ��");
//S[6]=
strcpy(S[6],"����� ����������� ��");
//S[7]=
strcpy(S[7],"����� �� �����");
//S[8]=
strcpy(S[8],"��������� ��������� ���� 1 ������ �� (��)");
//S[9]=
strcpy(S[9],"��������� ��������� ���� 2 ������ �� (��)");
//S[10]=
strcpy(S[10],"��������� ��������� ���� 1 ������ ���� (��)");
//S[11]=
strcpy(S[11,"��������� ��������� ���� 2 ������ ���� (��)"]);
//S[12]=
strcpy(S[12],"��������� ������ ���� 1 ������ �� (��)");
//S[13]=
strcpy(S[13],"��������� ������ ���� 2 ������ �� (��)");
//S[14]=
strcpy(S[14],"��������� ������ ���� 1 ������ ���� (��)");
//S[15]=
strcpy(S[15],"��������� ������ ���� 2 ������ ���� (��)");
//S[16]=
strcpy(S[16],"����� ����������� ��");
//S[17]=
strcpy(S[17],"����� ����������� MONO");
//S[18]=
strcpy(S[18],"��������� ����������");
//S[19]=
strcpy(S[19],"Test ����� HM");
//S[20]=
strcpy(S[20],"Test ����� MONO");
//S[21]=
strcpy(S[21],"�������� ������������");
//S[22]=
strcpy(S[22],"����� ������� ��� ���������� ");
//S[23]=
strcpy(S[23],"�������� � ���� ���� �� ��");
//S[24]=
strcpy(S[24],"�������� � ���� ���� �� ��");
//S[25]=
strcpy(S[25],"�������� � ���� ���� ���� ��");
//S[26]=
strcpy(S[26],"�������� � ���� ���� ���� ��");
//S[27]=
strcpy(S[27],"����������� ������ ��� ����");
//S[28]=
strcpy(S[28],"��������� NU");
//S[29]=
strcpy(S[29],"��������� VU");
//S[30]=
strcpy(S[30],"��������� ����������� ��");
//S[31]=
strcpy(S[31],"��������� ����������� ����");
//S[32]=
strcpy(S[32],"������������� ����������");
//S[33]=
strcpy(S[33],"���/���� ���");
//S[34]=
strcpy(S[34],"���/���� ����������");
//S[35]=
strcpy(S[35],"���/���� ��");
//S[36]=
strcpy(S[36],"���/���� ����");
//S[37]=
strcpy(S[37],"����������� �������� �����");
//S[40]=
strcpy(S[39],"���/���� ����� �����");
//S[50]=
strcpy(S[40],"�������� ������� ������ ���������� � ������ �����");
//S[51]=
strcpy(,S[41],"�������� ������������ �������� ��� �����������");
//S[52]=
strcpy(S[42],"�������� ���������.���������� �������������");
//S[54]=
strcpy(S[43],"������ � ���� �����");
//S[55]=
strcpy(S[44],"����� ������ � ���� �����");
//S[56]=
strcpy(S[60],"������������");
//S[57]=
strcpy(S[45],"�������� ����� �����");
*/

extern int chet_err;
int komandos(int kom_da) // ������� �������� ������� � ���������
{ // kom_da �� 0 �� ... � ������������ �� �������
  // ������� ���������� 1, ���� ��� �������,
  // ��� 0, ���� ������
    unsigned int size;
    int mes[4],mes1[4];
    char ss[256];
    int err;
    //double tim;
		time_t tim;
    float *y;
    y=(float*)mes;
    float z,z1;
    int kmmm;
    float mmm;
    int *mes_test;
    //char ss[256];
//////////////////////////////////////////////////////////////////
//S[0]=
strcpy(S[0],"����� ������");
//S[1]=
strcpy(S[1],"����� ������ ���������");
//S[2]=
strcpy(S[2],"������ �������� ���� ���������");
//S[3]=
strcpy(S[3],"����� ��� ������ � ����� �����");
//S[4]=
strcpy(S[4],"��������� ������������� �� � ����");
//S[5]=
strcpy(S[5],"����������� ����������� ����� ����� ��");
//S[6]=
strcpy(S[6],"����� ����������� ��");
//S[7]=
strcpy(S[7],"����� �� �����");
//S[8]=
strcpy(S[8],"��������� ��������� ���� 1 ������ �� (��)");
//S[9]=
strcpy(S[9],"��������� ��������� ���� 2 ������ �� (��)");
//S[10]=
strcpy(S[10],"��������� ��������� ���� 1 ������ ���� (��)");
//S[11]=
strcpy(S[11],"��������� ��������� ���� 2 ������ ���� (��)");
//S[12]=
strcpy(S[12],"��������� ������ ���� 1 ������ �� (��)");
//S[13]=
strcpy(S[13],"��������� ������ ���� 2 ������ �� (��)");
//S[14]=
strcpy(S[14],"��������� ������ ���� 1 ������ ���� (��)");
//S[15]=
strcpy(S[15],"��������� ������ ���� 2 ������ ���� (��)");
//S[16]=
strcpy(S[16],"����� ����������� ��");
//S[17]=
strcpy(S[17],"����� ����������� MONO");
//S[18]=
strcpy(S[18],"��������� ����������");
//S[19]=
strcpy(S[19],"Test ����� HM");
//S[20]=
strcpy(S[20],"Test ����� MONO");
//S[21]=
strcpy(S[21],"�������� ������������");
//S[22]=
strcpy(S[22],"����� ������� ��� ���������� ");
//S[23]=
strcpy(S[23],"�������� � ���� ���� �� ��");
//S[24]=
strcpy(S[24],"�������� � ���� ���� �� ��");
//S[25]=
strcpy(S[25],"�������� � ���� ���� ���� ��");
//S[26]=
strcpy(S[26],"�������� � ���� ���� ���� ��");
//S[27]=
strcpy(S[27],"����������� ������ ��� ����");
//S[28]=
strcpy(S[28],"��������� NU");
//S[29]=
strcpy(S[29],"��������� VU");
//S[30]=
strcpy(S[30],"��������� ����������� ��");
//S[31]=
strcpy(S[31],"��������� ����������� ����");
//S[32]=
strcpy(S[32],"������������� ����������");
//S[33]=
strcpy(S[33],"���/���� ���");
//S[34]=
strcpy(S[34],"���/���� ����������");
//S[35]=
strcpy(S[35],"���/���� ��");
//S[36]=
strcpy(S[36],"���/���� ����");
//S[37]=
strcpy(S[37],"����������� �������� �����");
strcpy(S[38],"���/���� ����� �����");
//S[40]=
strcpy(S[39],"������");//"���/���� ����� �����");
//S[50]=
strcpy(S[40],"�������� ������� ������ ���������� � ������ �����");
//S[51]=
strcpy(S[41],"�������� ������������ �������� ��� �����������");
//S[52]=
strcpy(S[42],"�������� ���������.���������� �������������");
//S[54]=
strcpy(S[43],"������ � ���� �����");
//S[55]=
strcpy(S[44],"����� ������ � ���� �����");
//S[56]=
strcpy(S[60],"��������� ��������� �������");
strcpy(S[61],"���������� ��������� �������");
//S[57]=
strcpy(S[45],"�������� ����� �����");


//////////////////////////////////////////////////////////////////

    z=1.5e8/((float)dsp_param.aMCLOCK/(float)dsp_param.aDECIMATION );
    z1=1.5e8/((float)dsp_param.aMCLOCK/(float)dsp_param.aDECIMONO );
//    BRD_readDPRAM(handle,0,512*sizeof(ULONG),mes,4*sizeof(ULONG));
//    while(mes[0]!=0)
//        BRD_readDPRAM(handle,0,512*sizeof(ULONG),mes,4*sizeof(ULONG));
    mes_test=(int *)&test_buf[40];
    for(int i=0;i<4;i++)
        //mes[i]=status2->mes[i];//ErrorData[i];//BSP_GET_ERROR_DATA(status2,i);
        //mes[i]=test_buf[40+i];
        mes[i]=mes_test[i];
    //z=dsp_param.aMCLOCK/(dsp_param.aDECIMATION*1.5e8);

    //mes[0]=rejim;
    //rejim= mes[0];
    mes[0]|=(kom_da<<16); // �������� 0-�� ���������� ����� �������
    kmmm=kom_da;
    mmm=mes[1];
//    z=y[2];
    
    switch(kom_da)
        {
          case 0:
              rejim= mes[0];
/**/          //rejim=rejim; // ����� ������
              mmm=rejim;
              break;
/**/      case 1:   // ����� ������ ���������
              //mes[1] = dsp_param.Nomer_komplekta;
              break;
/**/      case 2:   // �������� ���� ��������� �� � ����
             // mes[1] = dsp_param.zad_lhm;
             // mes[2] = dsp_param.zad_mono;
              mmm=y[1];
              break;
/**/      case 3: // ����� ��� ������ � ����� �����
              //y[1] = dsp_param.porog;
              mmm=y[1];
              break;
/**/      case 4: // ��������� ������������� �� � ����
              //y[1] = dsp_param.PL_K_Phm;
              //y[2] = dsp_param.PL_K_Pmono;
                mmm=y[1];
              break;
/**/      case 5: // ����������� ����������� ����� ����� ��
              //y[1] = dsp_param.beta;
              mmm=y[1];
              break;
/**/      case 6: // ����� ����������� ��
              //y[1] = dsp_param.PorogL;
              mmm=y[1];
              break;
/*--*/      case 7: // ��������� �������������� ����� � ��������� ������ � �����
              //mes[1]= dsp_param.float_to_int;
              break;
/**/      case 8: // ��������� ��������� ���� 1 ������ �� (��)
              //mes[1]= dsp_param.d_Wary1;
	      //mes[1]=mes[1]*z+0.5;
              mmm=mes[1]*z+0.5;
              break;
/**/      case 9: // ��������� ��������� ���� 2 ������ �� (��)
              //mes[1]= dsp_param.d_Wary2;
              //mes[1]=mes[1]*z+0.5;
              mmm=mes[1]*z+0.5;
              break;
/**/      case 10: // ��������� ��������� ���� 1 ������ ���� (��)
              //mes[1] = dsp_param.d_Wary1m;
              //mes[1]=mes[1]*z+0.5;
              mmm=mes[1]*z1+0.5;
              break;
/**/      case 11: // ��������� ��������� ���� 2 ������ ���� (��)
              //mes[1] = dsp_param.d_Wary1m;
              //mes[1]=mes[1]*z+0.5;
              mmm=mes[1]*z1+0.5;
              break;
/**/      case 12: // ��������� ������ ���� 1 ������ �� (��)
              //mes[1]= dsp_param.wid1;
              break;
/**/      case 13: // ��������� ������ ���� 2 ������ �� (��)
              //mes[1]= dsp_param.wid2;
              break;
/**/      case 14: // C
              //mes[1]= dsp_param.wid1m;
              break;
/**/      case 15: // ��������� ������ ���� 2 ������ ���� (��)
              //mes[1]= dsp_param.wid2m;
              break;
/**/      case 16: // ����� ����������� ��
              //y[1] = dsp_param.con;
              mmm=y[1];
              break;
/**/      case 17:// ����� ����������� MONO
              //y[1] = dsp_param.con_m;
              mmm=y[1];
              break;
/*--*/      case 18: // ��������� ����������
              //mes[1] =dsp_param.median;
              break;
/**/      case 19: // Test ����� HM ?
              //mes[1] =dsp_param.nom_hm;
              break;
/**/      case 20: // Test ����� MONO ?
              //mes[1] =dsp_param.nom_mono;
              break;
          case 21:  // �������� ������������
              //      N_XOCT=StrToInt(Dial);
              return 1; // ������ ��� ��� ������
/*--*/      case 22: // ����� ������� ��� ���������� (=10)
              //mes[1] = dsp_param.nak;
              break;
/**/      case 23:// �������� � ���� ���� �� ��
              //y[1] = dsp_param.us0hm1;
              mmm=y[1];
              break;
/**/      case 24:// �������� � ���� ���� �� ��
              //y[1] = dsp_param.us0hm2;
              mmm=y[1];
              break;
/**/      case 25:// �������� � ���� ���� ���� ��
              //y[1] = dsp_param.us0mono1;
              mmm=y[1];
              break;
/**/      case 26:// �������� � ���� ���� ���� ��
              //y[1] = dsp_param.us0mono2;
              mmm=y[1];
              break;
/**/      case 27: // ����������� ������ ��� ����
              //y[1] = dsp_param.energij;
              mmm=y[1];
              break;
          case 28: // ��������� NU
              //mes[1]=dsp_param.NU;
              break;
          case 29: // ��������� VU
              //mes[1]=dsp_param.VU;
              break;
          case 30: // ��������� ����������� ��
              //y[1] = dsp_param.sq_con;
              mmm=y[1];
              break;
          case 31: // ��������� ����������� ����
              //y[1] = dsp_param.sq_con_m;
              mmm=y[1];
              break;
          case 32:
                //mes[1]=dsp_param.sost_upr;
		//		ms &= ~(1<<16);
		//		ms |= (sost_upr<<16);
		//		komanda=0;
				break;

          case 33:
                //mes[1]=dsp_param.MTD;
		//		ms &= ~(1<<8);
		//		ms|=dsp_param.MTD<<8;// ���/���� ���
				break;

          case 34:
                //mes[1]=dsp_param.Nakoplenie;
				//ms &= ~(1<<9);
				//ms|=dsp_param.Nakoplenie<<9;// ���/���� ����������
                break;

			case 35:
				//dsp_param.WKL_HM=;
				//ms &= ~(1<<6);
				//ms|=dsp_param.WKL_HM<<6;// ���/���� ��
                                //mes[1] = dsp_param.WKL_HM ;
				break;
			case 36:
				//dsp_param.WKL_MONO=mes[1];
				//ms &= ~(1<<7);
				//ms|=dsp_param.WKL_MONO<<7;// ���/���� ����
                                //mes[1]= dsp_param.WKL_MONO ;
				break;

          case 37:

          // ����������� �������� �����
                mmm=y[2];
                break;
          case 38:

                //dsp_param.meteo=mes[1];
				//ms&=0xffff1fff;
				//ms|=dsp_param.meteo<<13;
				//komanda=0;
				break;
                        /*
			case 39:
			// ������
				if(mes[1]==1)
				{
					i=~0x100;
				}
				else
					i=0x100;
				DRV_ctrl( g_hDrv, DDDcmd_PIOWRITE, &i );	// ������ ��� �������� ������ "������"
				break;
                        */
          case 39:
          kmmm=39;
          //mes[1]=39;
          break;
          case 40:
          kmmm=40;
          // ���/���� ����� �����
                 //mes[1]=dsp_param.met_o^1;
				//ms&=~(1<<12);
				//ms |= mes[1];
                 //mes[1]=1;
				break;
          case 50:
          kmmm=40;
          // �������� ������� ������ ���������� � ������ �����
          break;
          case 51:
          kmmm=41;
          // �������� ������������ �������� ��� �����������
                k_usil=y[1];
                return 0;
          case 52:
          kmmm=42;
          // �������� ���������.���������� �������������
                Wkl_Sop=mes[1];
                if (Wkl_Sop) // ���� ������ ����������, �� ��� �������������
                {
                /*
                        MyInit_Soprov=(int(__import*)(char *c))GetProcAddress(Dll,"_Init_Soprov");
                        if(MyInit_Soprov==NULL)
                        {
                                MessageBox(NULL,"��� ������ Init_soprov � ���������� DLL","Error",MB_OK);
                                FreeLibrary(Dll);
                                return 0;
                        }
                */
                        strcpy(ss,ProgrammPath.c_str()) ;
                        MyInit_Soprov(ss);
                        //Init_Soprov(ss);
                }
                return 0;
          case 54:
          case 55:
          kmmm=43;
          // ������ � ���� �����
                sap_file=1;
                //GetPrivateProfileString("���������", "��������� ����������� ��", "1e-4",ss,256, fini );
                GetPrivateProfileString("������ � ����","����","",ss,256,fini);
                Objem=GetPrivateProfileInt("������ � ����","�����",200,fini);
                fp=fopen(ss,"wb");
                return 0;
          case 56:
          kmmm=44;
                K_K=0;
                sap_file=0;
                sap_file1=1;
                fclose(fp);
                fp=NULL;
                WritePrivateProfileString("������ � ����","������","0",fini);
                return 0;
          case 57:
          kmmm=45;
                // �������� ����� �����
                break;

        }

    chet_err=0;
//do

//for(int i=0;i<2;i++)
{
        //int mes1[4];
    // TODO err=BRD_signalSend(handle,0,0);
    //chet_err++;
    if(1)
		// TODO  ----- if(BRD_errcmp(err,BRDerr_OK))
      {
         size=sizeof(mes);
				 // TODO  -----  BRD_putMsg(handle,0,mes ,&size,-1);
         //BRD_getMsg(handle,0,mes1 ,&size,-1);
         //for(int i=0;i<4;i++)
         //       mes[i]=0;
         //size=sizeof(err);
         err=0;
         //BRD_getMsg(handle,0,&err ,&size,-1);
         //if(err!=0)
         //       MessageBox(NULL,"������� �� �������� �� 2 ���������","Error",MB_OK);
         mes_test[3]=kmmm;
      }
      Sleep(100);
}
time(&tim);// =Now();
mmm=mes[1];
if(kmmm == 40)
{
        if(mes[1]==1)
                fprintf(flog,"\n  %s koef=%f %s","������ � ��� �����",y[1],DateTimeToStr(tim).c_str());
        else
                fprintf(flog,"\n  %s koef=%f %s","����� �� ����� � ������ �����",y[1],DateTimeToStr(tim).c_str());
}
else
if(kmmm == 37)
{
    fprintf(flog,"\n  %s dsp_param.MTD=%d koef=%f %s",S[kmmm],dsp_param.MTD,y[1],DateTimeToStr(tim).c_str());
}
else if(kmmm == 38)
{
        if(mes[1]==1)
           strcpy(S[38],"��� ����� ����� ����� 0");
        if(mes[1]==2)
           strcpy(S[38],"��� ����� ����� ����� 1");
        if(mes[1]==3)
           strcpy(S[38],"��� ����� ����� ����� 2");
        if(mes[1]==4)
           strcpy(S[38],"��� ����� ����� ����� 3");
        fprintf(flog,"\n  %s mes=%d koef=%f %s",S[kmmm],mes[1],y[2],DateTimeToStr(tim).c_str());
}
else if(kmmm == 39)
{
         if(mes[1]==1)
                fprintf(flog,"\n �������� �������: ������           %s",DateTimeToStr(tim).c_str());
         if(mes[1]==0)
                fprintf(flog,"\n �������� �������: ������ ��������� %s",DateTimeToStr(tim).c_str());
         fflush(flog);
}
else if(kmmm == 6)
         fprintf(flog,"\n ��������� ������ �����������=%f %s",y[1],DateTimeToStr(tim).c_str());
else if(kmmm == 27)
         fprintf(flog,"\n ��������� ������ ����������� ��� ����=%f %s",y[1],DateTimeToStr(tim).c_str());
else
        fprintf(flog,"\n  %s mes=%f y[2]=%f %s",S[kmmm],mmm,y[2],DateTimeToStr(tim).c_str());
//fprintf(flog,"\n%-60s%s","������������ ��-�� ������ ������������� ",DateTimeToStr(tim).c_str());

//BRD_writeDPRAM(handle,0,512*sizeof(ULONG),mes,4*sizeof(ULONG));
//err=BRD_signalSend(handle,0,0);
//mes[0]=0;
//Sleep(1000);
//BRD_readDPRAM(handle,0,512*sizeof(ULONG),mes,4*sizeof(ULONG));
//kom_da=mes[0]>>16;
err=0;

//while(kom_da!=komanda0 && chet_err<100);
      return err;

//BRD_writeDPRAM(handle,0,sizeof(ULONG),mes,4*sizeof(int));
/*
     BRD_writeDPRAM(handle,0,sizeof(ULONG),mes,4*sizeof(ULONG));
     for(int i=0;i<4;i++)
                mes[i]=0;
     while(mes[0]==0)
        BRD_readDPRAM(handle,0,sizeof(ULONG),mes,4*sizeof(ULONG));
     return 1;
*/
    //  {
    //     MessageBox(NULL,"������ ��� �������� ������� � 1 ���������","Error",MB_OK);
    //     return 0;        // ������
    //  }

}


void perevod_ed()
{       
    dsp_param.DALN=dsp_imitat.az1*4096.0/360.0-47;  // ������ 1-� ���� � ���
    if(dsp_param.DALN<0)
        dsp_param.DALN+=4096;
    //dsp_param.DALN=dsp_param.DALN%4096; // ����������� ���� ���
    dsp_param.Das=(dsp_imitat.az2-dsp_imitat.az1)*4096.0/360.0; // ������ 2-� ���� ������������ 1-� � ���
    if(dsp_param.Das>=4096)
        dsp_param.Das-=4096;
    if(dsp_param.Das<0)
        dsp_param.Das+=4096;
    //dsp_param.Das=dsp_param.Das%4096; // ����������� ���� ���
    if( dsp_imitat.daln1>dsp_param.Daln_MONO)
        dsp_param.DMax = dsp_imitat.daln1/96; // ��������� 1-� ���� � ���������
    else if( dsp_imitat.daln1<dsp_param.Daln_MONO && dsp_imitat.daln1 >0)
        dsp_param.DMax = dsp_imitat.daln1/48;
    else
    {
        //MessageBox(NULL,"������� ������� ������������ ��������� ������������ �������","Error",MB_OK);
        dsp_param.DMax=0;
    }
    dsp_param.DMax = dsp_param.DMax%1695; // ����������� ���� ���������

    dsp_param.Ddal = (dsp_imitat.daln2 - dsp_imitat.daln1)/96;//*1695.0/dsp_param.Daln_HM; // ���������� 2-� ���� ������������� 1-� � ���������
    dsp_param.Ddal = dsp_param.Ddal%1695; // ����������� ���� ���������
    if(dsp_imitat.min_daln>dsp_param.Daln_MONO)
        dsp_param.md = dsp_imitat.min_daln/96;//*1695.0/dsp_param.Daln_HM; // ��� ��������� � ���������
    else
        dsp_param.md = dsp_imitat.min_daln/48;
    dsp_param.md = dsp_param.md%1695;  // ����������� ��� ���������
    dsp_param.DELTAD = (dsp_imitat.vr1*1000.0/3600.0)*dsp_param.tobs; // ������� �����������, ������� ������� ����1 � vr (� ������) �� ����� ������

    dsp_param.DELTAD = dsp_param.DELTAD/96;//*1695.0/dsp_param.Daln_HM; // ������� ����������� � �������� � ������� ��
    dsp_param.DELTAD = dsp_param.DELTAD%1695; // ����������� ���� ���������

    dsp_param.DELTAD2 = (dsp_imitat.vr2*1000.0/3600.0)*dsp_param.tobs; // ������� �����������, ������� ������� ����1 � vr (� ������) �� ����� ������
    dsp_param.DELTAD2 = dsp_param.DELTAD2/96;//*1695.0/dsp_param.Daln_HM; // ������� ����������� � ��������
    dsp_param.DELTAD2 = dsp_param.DELTAD2%1695; // ����������� ���� ���������

}
