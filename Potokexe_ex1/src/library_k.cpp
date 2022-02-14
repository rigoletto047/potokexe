//#include <vcl.h>
//#include "Uprov.h"
// TODO с этой директивой ругается на PCH intelli sence  #pragma hdrstop
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
// extern Form_data FormData; // не забыть проинициализировать!!!
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
extern unsigned int items;// Число записей программы ДДС
extern unsigned int g_aProgArray[1000+8];// Массив для хранения программы ДДС

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
    Создание файла, отображаемого в памяти
*/
void OpenFileMap( void )
{
    DWORD tid;

    rejim=dsp_param.rejim=GetPrivateProfileInt( "Параметры", "Режим работы", 4, fini );
     //rejim=1;

    // Формирование файла для отображения данных в режиме 4
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

// Формирование файла для вывода в режиме 0 или 1
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

    // Создание процесса для проверки выхода из карты памяти
	hThreadExit = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCardExit, NULL, 0, &tid );
	SetThreadPriority( hThreadExit,THREAD_PRIORITY_TIME_CRITICAL );
/*
    // Создание процесса для связи с платой
	hThread = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)ThreadCard, NULL, 0, &tid );
    //SetThreadPriority( hThread,THREAD_PRIORITY_TIME_CRITICAL );
*/
}


//////////////////////////////////////////////////////////
/*
   // Закрытие карты памяти
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
        // Инициализация платы и запуск сигнальных процессоров на выполнение
*/
//int Ini_Start(char *file)
//{
//// Инициализация платы и запуск сигнальных процессоров на выполнение
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
//    // Инициализация платы
//    err = BRD_initEx( BRDinit_FILE_KNOWN, (ProgrammPath+"brd.ini").c_str(), (ProgrammPath+"log.ini").c_str(), &pNum );
//        /*
//        Замечание
//        Переменная pNum типа указатель должна быть размещена в памяти,
//        а не в регистре. Чтобы это было так, нужно создать ее в виде
//        массива из одного элемента pNum[1] или передавать адрес переменной
//        */
//    if(BRD_errcmp(err,BRDerr_OK))
//        ;//MessageBox(NULL,"Плата успешно проинициализирована","Message",MB_OK);
//    else
//    {
//        if( BRD_errcmp(err,BRDerr_NONE_DEVICE) )
//        {
//            MessageBox(NULL,"Не обнаружено базового модуля","Error",MB_OK);
//            return -1;
//        }
//        else  if(BRD_errcmp(err,BRDerr_BAD_INI_FILE))
//        {
//            MessageBox(NULL,"Невозможно открыть файл инициализации","Error",MB_OK);
//            return -1;
//        }
//        else
//        {
//            MessageBox(NULL,"Ошибка открытия платы","Error",MB_OK);
//            return -1;
//        }
//    }
//    // Поиск всех базовых модулей
//    U32  lidArray[1], itemReal, g_lid;
//    BRD_lidList( lidArray, 1, &itemReal );
//    if( itemReal==0 )
//    {
//        MessageBox(NULL,"Не обнаружено базового модуля","Error",MB_OK);
//        return -1;
//    }
//    g_lid = lidArray[0];// Определение номера платы
//    // Открытие платы
//    handle = BRD_open( g_lid, BRDopen_EXCLUSIVE, &openMode );
//    if(handle<=0)
//    {
//        sprintf(ss, " ERROR: Нельзя открыть устройство (LID=%d)", g_lid );
//        MessageBox(NULL,ss,"Error",MB_OK);
//        return -1;
//    }
//    err=BRD_reset(handle,0);
//    if(err<0)
//    {
//        MessageBox(NULL,"Невозможен сброс платы","Error",MB_OK);
//        return -1;
//    }
//    // Загрузка ПЛИСС
//    GetPrivateProfileString( "Файлы", "Программа ПЛИС", "cd416s54.h10",ss,256, fini );
//    err=BRD_puLoad(handle,0x100,ss,&ite);
//    if(!BRD_errcmp(err,BRDerr_OK))
//    {
//        if(BRD_errcmp(err,BRDerr_BAD_ID))
//            MessageBox(NULL,"Неверный идентификатор","Error",MB_OK);
//        if(BRD_errcmp(err,BRDerr_BAD_FILE))
//            MessageBox(NULL,"Файл недоступен","Error",MB_OK);
//        if(BRD_errcmp(err,BRDerr_HW_ERROR))
//            MessageBox(NULL,"Аппаратная ошибка","Error",MB_OK);
//        //BRD_cleanup();
//        return -1;
//    }
//    if(ite==0)
//    {
//        MessageBox(NULL,"ПЛИС не загружена","Error",MB_OK);
//        //BRD_cleanup();
//        return -1;
//    }
//    //BRD_SigList Prer[10];
//    //err=BRD_signalList(handle,0,&Prer[0],10,&itemReal);
//    // Загрузка программы в сигнальный процессор
//    err=BRD_load( handle, 0, file, 0, NULL );
//    if(BRD_errcmp(err,BRDerr_OK))
//    ;   //MessageBox(NULL,"Программа загружена","Message",MB_OK);
//    else
//    {
//        //BRD_cleanup();
//        MessageBox(NULL,"Ошибки при загрузке программы","Message",MB_OK);
//        return -1;
//    }
//
//    // Запуск программы на выполнение
//    err=BRD_start( handle, 0 );
//    if(BRD_errcmp(err,BRDerr_OK))
//    ;
//    else if(BRD_errcmp(err,BRDerr_BAD_HANDLE))
//        MessageBox(NULL,"Неправильный дескриптор","Error",MB_OK);
//    else  if(BRD_errcmp(err,BRDerr_CLOSED_HANDLE))
//        MessageBox(NULL,"Дескриптор неоткрытого базового модуля","Error",MB_OK);
//    if(!BRD_errcmp(err,BRDerr_OK))
//    {
//        MessageBox(NULL,"Невозможно стартовать","Error",MB_OK);
//        //BRD_cleanup();
//        return -1;
//    }
//    return 0;
//}


////////////////////////////////////////////////////////////
///*
//        // Чтение файла, содержащего программу ДДС
//*/
//int     ReadPrgFile( char *fileName, U32 *dst, S32 size )
//{
//// Чтение файла, содержащего программу ДДС
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
//		//sprintf( "*** Нельзя открыть PrgFile - '%s'\n", fileName );
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
        // Загрузка параметров из ИНИ-файла
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

    // Управление от кнопок или дистанционно
    //perekluh=!perekluh;
    //Form4->Button7Click(0);
    WritePrivateProfileString("Conrad","Exit","0",fname);
    // менять дальность оперативно смысла нет
    dsp_param.Daln_HM = GetPrivateProfileInt( "Дальность", "ЧМ", 174000, fini );
    dsp_param.Daln_MONO = GetPrivateProfileInt( "Дальность", "МОНО", 23000, fini );
    dsp_param.Daln_VU = GetPrivateProfileInt( "Дальность", "Верхних углов", 100000, fini );
    if(avt_per)
    {
    fprintf(flog,"\n Дальность ЧМ %d",dsp_param.Daln_HM);
    fprintf(flog,"\n Дальность МОНО %d",dsp_param.Daln_MONO);
    fprintf(flog,"\n Дальность верхних углов %d",dsp_param.Daln_VU);
    }
    //*********** СЕКЦИЯ ДДС ********************************************************************************************
    dsp_param.aDECIMONO = GetPrivateProfileInt( "ДДС", "Децимация ДДС МОНО", 32, fini );


  // ************   СЕКЦИЯ ПАРАМЕТРЫ  ***********************************************************************************
    dsp_param.sost_upr=GetPrivateProfileInt( "Параметры", "Управление от компьютера", 1, fini );
  // считываем номер комплекта
    dsp_param.Nomer_komplekta=GetPrivateProfileInt( "Параметры", "Номер комплекта", 1, fini );
    // В ЗАВИСИМОСТИ ОТ НОМЕРА КОМПЛЕКТА СЧИТЫВАЕМ ФАЙЛЫ ФИЛЬТРОВ
    Section= "Файлы комплекта "+ std::to_string(dsp_param.Nomer_komplekta);
    /*
    if (dsp_param.Nomer_komplekta!=GetPrivateProfileInt( "System", "SectorName", 1, frpu ) )
     {
        MessageBox(NULL,"Номер комплекта в mtd__n.ini и rpu.ini не совпадают, исправлено","Error",MB_OK);
        Nom_Kml=IntToStr(int(dsp_param.Nomer_komplekta)); //???
        WritePrivateProfileString( "System", "SectorName", Nom_Kml.c_str(), frpu );
     }
     */
    // и отображаем номер комплекта
    //if (dsp_param.Nomer_komplekta==1)
    //  Form4->Button15->Caption = "1";
    //else
    //  Form4->Button15->Caption = "2";

   // if (GetPrivateProfileInt("System", "NoConrad", 0, frpu)==0)
   //  FormData.flag_work_VK = 2; // включен ВК
   // else
   //  FormData.flag_work_VK = 0; // выключен ВК

//???    dsp_param.aMCLOCK=GetPrivateProfileInt( "Параметры", "Частота внешнего генератора", 50000000, fini );

    dsp_param.rejim=GetPrivateProfileInt( "Параметры", "Режим работы", 4, fini );
    rejim=dsp_param.rejim;
    //dsp_param.otobrajenie=GetPrivateProfileInt( "Параметры", "Режим отображения", 0, fini );
    dsp_param.f_proz=GetPrivateProfileInt( "Параметры", "Частота процессора", 500000000, fini );
    if(avt_per )
    {
    fprintf(flog,"\n\n  *******  Параметры **********\n\n");
    fprintf(flog,"\n Управление от компьютера %d",dsp_param.sost_upr);
    fprintf(flog,"\n Номер комплекта %d",dsp_param.Nomer_komplekta);
    fprintf(flog,"\n Режим работы %d",dsp_param.rejim);
    fprintf(flog,"\n Частота процессора %d",dsp_param.f_proz);
    }
    
    dsp_param.aGEN=GetPrivateProfileInt( "Параметры", "Генератор внешний", 0, fini );
    if(dsp_param.aGEN ==0)
    {
        //err_gen=1;
        if(avt_per )
        {
        fprintf(flog,"\n\n  *******  Параметры **********\n\n");
        fprintf(flog,"\n Установлен внутренний тактовый генератор. Частота %d",dsp_param.aMCLOCK);
        }
        //Form1->Panel1->Visible=true;
        dsp_param.aMCLOCK =GetPrivateProfileInt( "Параметры", "Частота внутреннего генератора", 100000000, fini );
        //Form4->Shape14->Brush->Color=clYellow;
        //Form4->Label14->Caption = "Внутренний";
        //Form4->Label35->Caption = "Частота " + FloatToStrF(dsp_param.aMCLOCK/(1000000),ffFixed, 7,2)+" МГц" ;
    }
    else
    {
        //err_gen=0;

        //Form1->Panel1->Visible=false;
        dsp_param.aMCLOCK =GetPrivateProfileInt( "Параметры", "Частота внешнего генератора", 50000000, fini );
        //Form4->Shape14->Brush->Color=clLime;
        //Form4->Label14->Caption = "Внешний";
        //Form4->Label35->Caption = "Частота " + FloatToStrF(dsp_param.aMCLOCK/(1000000),ffFixed, 7,2)+" МГц" ;
        if(avt_per )
        fprintf(flog,"\n Установлен внешний тактовый генератор. Частота %d",dsp_param.aMCLOCK);
    }

    dsp_param.f_getHM=GetPrivateProfileInt( "Параметры", "Частота гетеродина ЧМ", 19000000, fini );
    dsp_param.f_getMONO=GetPrivateProfileInt( "Параметры", "Частота гетеродина МОНО", 21000000, fini );

    dsp_param.MTD=GetPrivateProfileInt( "Параметры", "МТД включено", 1, fini );
    GetPrivateProfileString( "Параметры", "Усиление без МТД", "1",ss,256, fini );
    dsp_param.koef[5]=atof(ss);
    dsp_param.per_lhm=GetPrivateProfileInt( "Параметры", "Число периодов ЧМ", 5, fini );
    dsp_param.per_mono=GetPrivateProfileInt( "Параметры", "Число периодов МОНО", 5, fini );
    dsp_param.dal_ogr_HM=GetPrivateProfileInt( "Параметры", "Дальность ограничения ЧМ", 40000, fini );
    GetPrivateProfileString("Параметры", "Порог ограничения ЧМ", "1e1",ss,256, fini );
        dsp_param.con=atof(ss);
    GetPrivateProfileString("Параметры", "Множитель ограничения ЧМ", "1e-4",ss,256, fini );
        dsp_param.sq_con=atof(ss);
    GetPrivateProfileString("Параметры", "Порог ограничения МОНО", "1e1",ss,256, fini );
        dsp_param.con_m=atof(ss);
    GetPrivateProfileString("Параметры", "Множитель ограничения МОНО", "1e-4",ss,256, fini );
        dsp_param.sq_con_m=atof(ss);
    GetPrivateProfileString( "Параметры", "Порог компаратора", "1",ss,256, fini );
    dsp_param.aSM_THDAC2=atof(ss);
    dsp_param.nak=GetPrivateProfileInt( "Параметры", "Число обзоров для накопления", 10, fini );
    dsp_param.HM=GetPrivateProfileInt( "Параметры", "ЧМ на всей дистанции", 1, fini );
    dsp_param.WKL_HM=GetPrivateProfileInt( "Параметры", "ЧМ включено", 1, fini );
    dsp_param.WKL_MONO=GetPrivateProfileInt( "Параметры", "МОНО включено", 1, fini );
    if(dsp_param.WKL_HM==0 && dsp_param.WKL_MONO==0)
    {
        //MessageBox(NULL,"Не включена обработка ЧМ и МОНО ","Error",MB_OK);
        dsp_param.WKL_HM=dsp_param.WKL_MONO =1;
    }

    dsp_param.kol_oz=GetPrivateProfileInt( "Параметры", "Число зондирований для передачи оценок", 1, fini );

    dsp_param.Nakoplenie=GetPrivateProfileInt( "Параметры", "Накопление включено", 1, fini );
    dsp_param.NU=GetPrivateProfileInt( "Параметры", "Нижние углы", 1, fini );
    dsp_param.VU=GetPrivateProfileInt( "Параметры", "Верхние углы", 1, fini );
    if(avt_per )
    {
    fprintf(flog,"\n МТД включено=%d",dsp_param.MTD);

    fprintf(flog,"\n Усиление без МТД=%f",dsp_param.koef[5]);

    fprintf(flog,"\n Дальность ограничения ЧМ=%d",dsp_param.dal_ogr_HM);

        //Form4->Edit16->Text = FloatToStrF(dsp_param.con,ffFixed,7,2);
    fprintf(flog,"\n Порог ограничения ЧМ=%f",dsp_param.con);

        //Form4->Edit21->Text = FloatToStrF(dsp_param.sq_con,ffExponent,2,2);
    fprintf(flog,"\n Множитель ограничения ЧМ=%f",dsp_param.sq_con);

        //Form4->Edit17->Text = FloatToStrF(dsp_param.con_m,ffFixed,7,2);
    fprintf(flog,"\n Порог ограничения МОНО=%f",dsp_param.con_m);

        //Form4->Edit27->Text = FloatToStrF(dsp_param.sq_con_m,ffExponent,2,2);
    fprintf(flog,"\n Множитель ограничения МОНО=%f",dsp_param.sq_con_m);

    //Form4->Edit12->Text = FloatToStrF(dsp_param.aSM_THDAC2,ffFixed,7,2);


    fprintf(flog,"\n ЧМ включено=%d",dsp_param.WKL_HM);

    fprintf(flog,"\n МОНО включено=%d",dsp_param.WKL_MONO);

    // обновление через таймер формы

    fprintf(flog,"\n Накопление включено=%d",dsp_param.Nakoplenie);
    //if(dsp_param.WKL_HM==0 && dsp_param.WKL_MONO==0)
    //{
    //    MessageBox(NULL,"Не включена обработка ЧМ и МОНО ","Error",MB_OK);
    //    dsp_param.WKL_HM=dsp_param.WKL_MONO =1;
    //}

    
    fprintf(flog,"\n Нижние углы включены=%d",dsp_param.NU);
    fprintf(flog,"\n Верхние углы включены=%d",dsp_param.VU);
    }
    // обновление через таймер формы
    if(dsp_param.NU==0 && dsp_param.VU ==0)
      {
        //MessageBox(NULL,"Не включены нижние и верхние углы","Error",MB_OK);
        //return 1;
         dsp_param.NU=1;  dsp_param.VU =1;
      }

    dsp_param.median=GetPrivateProfileInt( "Параметры", "Медианная фильтрация", 1, fini );
     GetPrivateProfileString( "Параметры", "Время обзора", "4.6",ss,256, fini );
    T_obs=dsp_param.tobs=atof(ss);
    nKK=GetPrivateProfileInt( "Параметры", "Число фильтров карты помех", 3, fini );
    nT=GetPrivateProfileInt( "Параметры", "Коэффициент децимации", 3, fini );
    GetPrivateProfileString( "Параметры", "Мощность сигнала", "10",ss,256, fini );
        sig=atof(ss);
    dsp_param.N_Buf_Oz= GetPrivateProfileInt( "Параметры", "Длина буфера оценок координат", 18, fini );
    dsp_param.met_o=GetPrivateProfileInt( "Параметры", "Вывод метео", 0, fini );
    dsp_param.meteo=GetPrivateProfileInt( "Параметры", "Каналы метео", 4, fini );
    dsp_param.koef[1]=GetPrivateProfileInt( "Параметры", "Усиление 0 карты", 1, fini );
    dsp_param.koef[0]=GetPrivateProfileInt( "Параметры", "Усиление МТД", 1, fini );
    dsp_param.koef[2]=GetPrivateProfileInt( "Параметры", "Усиление 1 карты", 1, fini );
    dsp_param.koef[3]=GetPrivateProfileInt( "Параметры", "Усиление 2 карты", 1, fini );
    dsp_param.koef[4]=GetPrivateProfileInt( "Параметры", "Усиление 3 карты", 1, fini );
    dsp_param.kor_as=GetPrivateProfileInt( "Параметры", "Коррекция азимута", 0, fini );
    if(avt_per )
    {
    fprintf(flog,"\n Медианная фильтрация включена=%d",dsp_param.median);
    fprintf(flog,"\n Время обзора=%f",T_obs);


    if( dsp_param.N_Buf_Oz>18)
    {
        fprintf(flog,"\nДлина буфера оценок координат>18. Установлена =18\n");
        dsp_param.N_Buf_Oz=18;
    }

    fprintf(flog,"\n Вывод метео=%d",dsp_param.met_o);

    fprintf(flog,"\n Каналы метео=%d",dsp_param.meteo);

    fprintf(flog,"\n Усиление МТД=%f",dsp_param.koef[0]);


    fprintf(flog,"\n Усиление 0 карты=%f",dsp_param.koef[1]);

    fprintf(flog,"\n Усиление 1 карты=%f",dsp_param.koef[2]);

    fprintf(flog,"\n Усиление 2 карты=%f",dsp_param.koef[3]);
    
    fprintf(flog,"\n Усиление 3 карты=%f",dsp_param.koef[4]);
    
    fprintf(flog,"\n Коррекция азимута в МАИ",dsp_param.kor_as);
    }
    
    // ************   СЕКЦИЯ АДАПТАЦИЯ    ***********************************************************************************

    GetPrivateProfileString( "Адаптация", "Порог для карты помех", "10",ss,256, fini );
        dsp_param.porog=atof(ss);
        //Form4->Edit7->Text =FloatToStrF(dsp_param.porog,ffFixed,7,2);
    

    dsp_param.NR=GetPrivateProfileInt( "Адаптация", "Размер карты помех ЧМ", 512, fini );

	if(dsp_param.NR<=0 || dsp_param.NR>4096)dsp_param.NR=512;
        //Form4->Edit6->Text = IntToStr(dsp_param.NR);

    dsp_param.NR_M=GetPrivateProfileInt( "Адаптация", "Размер карты помех МОНО", 512, fini );
	if(dsp_param.NR_M<=0 || dsp_param.NR_M>4096)dsp_param.NR_M=512;
        //Form4->Edit9->Text = IntToStr(dsp_param.NR_M);


    GetPrivateProfileString( "Адаптация", "Константа регуляризации", "0.001 0.001",ss,256, fini );
    sscanf(ss,"%f%f",&dsp_param.PL_K_Phm,&dsp_param.PL_K_Pmono);
    fprintf(flog,"\n Константа регуляризации ЧМ=%f",dsp_param.PL_K_Phm);
    fprintf(flog,"\n Константа регуляризации МОНО=%f",dsp_param.PL_K_Pmono);

    GetPrivateProfileString( "Адаптация", "Коэффициент сглаживания карты помех ЧМ", "0.9",ss,256, fini );
    dsp_param.beta=atof(ss);


    GetPrivateProfileString( "Адаптация", "Коэффициент сглаживания карты помех МОНО", "0.9",ss,256, fini );
        dsp_param.beta_MONO=atof(ss);
    GetPrivateProfileString( "Адаптация", "Множитель окна", "0.1",ss,256, fini );
        okno.mnoj_okna=atof(ss);
    //GetPrivateProfileString( "Адаптация", "Слагаемое окна", "0",ss,256, fini );
    //    okno.mno=atof(ss);
    okno.prop=GetPrivateProfileInt( "Адаптация", "Число пропусков", 2, fini );
    okno.L_Buf=GetPrivateProfileInt( "Адаптация", "Размер буфера", 10, fini );
    okno.n=GetPrivateProfileInt( "Адаптация", "Уровень", 3, fini );
    GetPrivateProfileString( "Адаптация", "Множитель окна МОНО", "0.1",ss,256, fini );
        oknom.mnoj_okna=atof(ss);
    //GetPrivateProfileString( "Адаптация", "Слагаемое окна", "0",ss,256, fini );
    //    okno.mno=atof(ss);
    oknom.prop=GetPrivateProfileInt( "Адаптация", "Число пропусков МОНО", 2, fini );
    oknom.L_Buf=GetPrivateProfileInt( "Адаптация", "Размер буфера МОНО", 10, fini );
    oknom.n=GetPrivateProfileInt( "Адаптация", "Уровень МОНО", 3, fini );
    tip_obr= GetPrivateProfileInt( "Адаптация", "Тип обработки", 0, fini );
    if(avt_per )
    {
    fprintf(flog,"\n 3 сигма для записи в карту помех=%f",dsp_param.porog);
    fprintf(flog,"\n Размер карты помех=%d",dsp_param.NR);
    //sscanf(ss,"%f%f",&dsp_param.PL_K_Phm,&dsp_param.PL_K_Pmono);
    //fprintf(flog,"\n Константа регуляризации ЧМ=%f",dsp_param.PL_K_Phm);
    //fprintf(flog,"\n Константа регуляризации МОНО=%f",dsp_param.PL_K_Pmono);
    fprintf(flog,"\n Коэффициент сглаживания карты помех=%f",dsp_param.beta);
    fprintf(flog,"\n Тип обработки=%d",tip_obr);
    }

// ***************   СЕКЦИЯ ОБНАРУЖЕНИЕ   *****************************************************************************


    GetPrivateProfileString( "Обнаружение", "Порог обнаружения", "600",ss,256, fini );
 	dsp_param.PorogL=atof(ss);
    

    GetPrivateProfileString( "Обнаружение", "Множитель обнаружения", "1",ss,256, fini );
 	dsp_param.delta=atof(ss);
    GetPrivateProfileString( "Обнаружение", "Множитель порога для МОНО", "1",ss,256, fini );
 	dsp_param.energij=atof(ss);
    dsp_param.float_to_int=GetPrivateProfileInt( "Обнаружение", "Порог из файла", 0, fini );
    // чтение и отображение параметров ВАРУ на форме
    dsp_param.d_Wary1=GetPrivateProfileInt( "Обнаружение", "Максимальная дальность временной регулировки усиления ЧМ НУ", 50000, fini );
    dsp_param.d_Wary2=GetPrivateProfileInt( "Обнаружение", "Максимальная дальность временной регулировки усиления ЧМ ВУ", 50000, fini );
    dsp_param.d_Wary1m=GetPrivateProfileInt( "Обнаружение", "Максимальная дальность временной регулировки усиления МОНО НУ", 20000, fini );
    dsp_param.d_Wary2m=GetPrivateProfileInt( "Обнаружение", "Максимальная дальность временной регулировки усиления МОНО ВУ", 20000, fini );
    dsp_param.wid1=GetPrivateProfileInt( "Обнаружение", "Вид кривой ВАРУ НУ  ЧМ", 0, fini );
    dsp_param.wid2=GetPrivateProfileInt( "Обнаружение", "Вид кривой ВАРУ ВУ  ЧМ", 0, fini );
    dsp_param.wid1m=GetPrivateProfileInt( "Обнаружение", "Вид кривой ВАРУ НУ  МОНО", 0, fini );
    dsp_param.wid2m=GetPrivateProfileInt( "Обнаружение", "Вид кривой ВАРУ ВУ  МОНО", 0, fini );
    GetPrivateProfileString("Обнаружение", "Усиление в нуле ВАРУ ЧМ НУ", "0",ss,256, fini );
    dsp_param.us0hm1=atof(ss);
    GetPrivateProfileString("Обнаружение", "Усиление в нуле ВАРУ ЧМ ВУ", "0",ss,256, fini );
    dsp_param.us0hm2=atof(ss);
    GetPrivateProfileString("Обнаружение", "Усиление в нуле ВАРУ МОНО НУ", "0",ss,256, fini );
    dsp_param.us0mono1=atof(ss);
    GetPrivateProfileString("Обнаружение", "Усиление в нуле ВАРУ МОНО ВУ", "0",ss,256, fini );
    dsp_param.us0mono2=atof(ss);
    Wkl_Sop=GetPrivateProfileInt( "Обнаружение", "Фильтр", 0, fini );
    if(avt_per )
    {
    fprintf(flog,"\n Порог обнаружения=%f",dsp_param.PorogL);
    fprintf(flog,"\n Множитель обнаружения=%f",dsp_param.delta);
    fprintf(flog,"\n Множитель порога для МОНО=%f",dsp_param.energij);
    fprintf(flog,"\n Максимальная дальность временной регулировки усиления ЧМ НУ=%d",dsp_param.d_Wary1);
    fprintf(flog,"\n Максимальная дальность временной регулировки усиления ЧМ ВУ=%d",dsp_param.d_Wary2);
    fprintf(flog,"\n Максимальная дальность временной регулировки усиления МОНО НУ=%d",dsp_param.d_Wary1m);
    fprintf(flog,"\n Максимальная дальность временной регулировки усиления МОНО ВУ=%d",dsp_param.d_Wary2m);
    fprintf(flog,"\n ВАРУ НУ  ЧМ включена=%d",dsp_param.wid1);
    fprintf(flog,"\n ВАРУ DУ  ЧМ включена=%d",dsp_param.wid2);
    fprintf(flog,"\n ВАРУ НУ  МОНО включена=%d",dsp_param.wid1m);
    fprintf(flog,"\n ВАРУ ВУ  МОНО включена=%d",dsp_param.wid2m);
    fprintf(flog,"\n Усиление в нуле ВАРУ ЧМ НУ=%f",dsp_param.us0hm1);
    fprintf(flog,"\n Усиление в нуле ВАРУ ЧМ ВУ=%f",dsp_param.us0hm2);
    fprintf(flog,"\n Усиление в нуле ВАРУ МОНО НУ=%f",dsp_param.us0mono1);
    fprintf(flog,"\n Усиление в нуле ВАРУ МОНО ВУ=%f",dsp_param.us0mono2);
    fprintf(flog,"\n Фильтр сопровождения включен=%d",Wkl_Sop);
    }

// ******************   СЕКЦИЯ ФАЙЛЫ КОМПЛЕКТА  **********************************************************************

    dsp_param.zad_lhm=GetPrivateProfileInt( Section.c_str(), "Задержка нуля дальности ЧМ", 10, fini );
    dsp_param.zad_mono=GetPrivateProfileInt( Section.c_str(), "Задержка нуля дальности МОНО", 10, fini );

    // Выберем файлы для сжатия и МТД
    GetPrivateProfileString( Section.c_str(), "Файл тестового сигнала ЧМ НУ", ".\\dann\\Ishod_LHM_1.txt",imj_file_test_hm1,256, fini );
    GetPrivateProfileString( Section.c_str(), "Файл тестового сигнала ЧМ ВУ", ".\\dann\\Ishod_LHM_2.txt",imj_file_test_hm2,256, fini );
    GetPrivateProfileString( Section.c_str(), "Файл тестового сигнала МОНО НУ", ".\\dann\\Ishod_MONO_1.txt",imj_file_test_mono1,256, fini );
    GetPrivateProfileString( Section.c_str(), "Файл тестового сигнала МОНО ВУ", ".\\dann\\Ishod_MONO_2.txt",imj_file_test_mono2,256, fini );

    GetPrivateProfileString( Section.c_str(), "Файл фильтра сжатия ЧМ НУ", ".\\dann\\Filter_nat_hm_1.txt",imj_file_cjat_hm1,256, fini );
    GetPrivateProfileString( Section.c_str(), "Файл фильтра сжатия ЧМ ВУ", ".\\dann\\Filter_nat_hm_2.txt",imj_file_cjat_hm2,256, fini );
    GetPrivateProfileString( Section.c_str(), "Файл фильтра сжатия МОНО НУ", ".\\dann\\Filter_nat_mono_1.txt",imj_file_cjat_mono1,256, fini );
    GetPrivateProfileString( Section.c_str(), "Файл фильтра сжатия МОНО ВУ", ".\\dann\\Filter_nat_mono_2.txt",imj_file_cjat_mono2,256, fini );

    GetPrivateProfileString( "Файлы", "Файл для фильтров MTD", ".\\dann\\pci5_lhm_4__0.1GZ_cplx.txt",imj_file_mtd,256, fini );
    GetPrivateProfileString( "Файлы", "Программа ДДС", "100_15625.prg",prog_ddc,256, fini );







// ******************   СЕКЦИЯ ТЕСТ  **********************************************************************
        
        TEST=GetPrivateProfileInt( "Тест", "Тестирование", 0, fini );
        
        dsp_param.nom_hm=GetPrivateProfileInt( "Тест", "Номер ЧМ", 1868, fini );
        dsp_param.nom_mono=GetPrivateProfileInt( "Тест", "Номер МОНО", 279, fini );

        amp_hm=GetPrivateProfileInt( "Тест", "Амплитуда ЧМ", 1, fini );
        amp_mono=GetPrivateProfileInt( "Тест", "Амплитуда МОНО", 1, fini );

        S_Okno= GetPrivateProfileInt( "Тест", "Скрытое окно", 0, fini );
        otladka_otm=GetPrivateProfileInt( "Тест", "Отладка сопровождения", 0, fini );
        if(otladka_otm)
                tt=fopen("tt.txt","w");
        sap_ozenki=GetPrivateProfileInt( "Тест", "Выдача оценок в файл", 0, fini );
        //if(Oz_file)
        //        foz=fopen("oz.txt","w");
        if(avt_per )
        {
        fprintf(flog,"\n Включить тестирование=%d",TEST);
        fprintf(flog,"\n Номер тестового сигнала ЧМ=%d",dsp_param.nom_hm);
        fprintf(flog,"\n Номер тестового сигнала MONO=%d",dsp_param.nom_mono);
        fprintf(flog,"\n Амплитуда тестового сигнала ЧМ=%d",amp_hm);
        fprintf(flog,"\n Амплитуда тестового сигнала MONO=%d",amp_mono);
        }

// ******************   СЕКЦИЯ ИМИТАЦИЯ  **********************************************************************
    dsp_param.otladka=GetPrivateProfileInt( "Имитация", "Включена", 0, fini );
    //if( dsp_param.otladka)

    dsp_param.tip_filt= GetPrivateProfileInt( "Имитация", "Тип фильтра ДН", 1, fini );
    GetPrivateProfileString( "Имитация", "Ширина ДН", "2.5",ss,256, fini ); // в градусах
    double da;
    da=atof(ss);
    dsp_param.da=(float)da;


// Угол первой цели задается в градусах
    dsp_param.DALN=GetPrivateProfileInt( "Имитация", "Азимут цели ЧМ", 225, fini ); // в градусах
    dsp_param.DALN=dsp_param.DALN*4096.0/360.0-47;
    if(dsp_param.DALN<0)
        dsp_param.DALN+=4096;
    dsp_param.DALNm=GetPrivateProfileInt( "Имитация", "Азимут цели МОНО", 225, fini ); // в градусах
    dsp_param.DALNm=dsp_param.DALNm*4096.0/360.0-47;
    if(dsp_param.DALNm<0)
        dsp_param.DALNm+=4096;
// Угол второй цели задается в градусах
    dsp_param.Das = GetPrivateProfileInt( "Имитация", "Азимут 2 цели ЧМ относительно 1", 5, fini );  // в градусах
    dsp_param.Das = dsp_param.Das*4096.0/360.0;
    dsp_param.Dasm = GetPrivateProfileInt( "Имитация", "Азимут 2 цели МОНО относительно 1", 5, fini );  // в градусах
    dsp_param.Dasm = dsp_param.Dasm*4096.0/360.0;
// Приращение угла целей в градусах
    GetPrivateProfileString( "Имитация", "Приращение угла 1 цели ЧМ", "1",ss,256, fini );
    dsp_param.deltaas1HM =atof(ss)*4096.0/360.0;
    GetPrivateProfileString( "Имитация", "Приращение угла 2 цели ЧМ", "1",ss,256, fini );
    dsp_param.deltaas2HM =atof(ss)*4096.0/360.0;
    GetPrivateProfileString( "Имитация", "Приращение угла 1 цели МОНО", "1",ss,256, fini );
    dsp_param.deltaas1MONO =atof(ss)*4096.0/360.0;
    GetPrivateProfileString( "Имитация", "Приращение угла 2 цели МОНО", "1",ss,256, fini );
    dsp_param.deltaas2MONO =atof(ss)*4096.0/360.0;
// Дальность 1-й цели в ини-файле в метрах
    double dr;
    dsp_param.aDECIMATION=GetPrivateProfileInt( "ДДС", "Децимация ДДС ЧМ", 64, fini );
    dr=dsp_param.aDECIMATION*1.5e8/dsp_param.aMCLOCK;
    dsp_param.DMax=GetPrivateProfileInt( "Имитация", "Дальность цели ЧМ", 70000, fini );
    dsp_param.DMax/=dr;

    dsp_param.DMaxm=GetPrivateProfileInt( "Имитация", "Дальность цели МОНО", 18000, fini );
    if(dsp_param.aDECIMONO==32)
        dsp_param.DMaxm/=0.5*dr;
    else
        dsp_param.DMaxm/=dr;
// Дальность второй цели относительно 1 в отсчетах
    dsp_param.Ddalm = GetPrivateProfileInt( "Имитация", "Дальность 2 цели МОНО относительно 1", 5, fini );
    dsp_param.Ddal=GetPrivateProfileInt( "Имитация", "Дальность 2 цели ЧМ относительно 1", 5, fini );
// Приращение дальности целе в отсчетах
    dsp_param.DELTAD=GetPrivateProfileInt( "Имитация", "Приращение дальности ЧМ", 10, fini );
    dsp_param.DELTADm=GetPrivateProfileInt( "Имитация", "Приращение дальности МОНО", 10, fini );
    dsp_param.DELTAD2=GetPrivateProfileInt( "Имитация", "Приращение дальности ЧМ 2", 10, fini );
    dsp_param.DELTAD2m=GetPrivateProfileInt( "Имитация", "Приращение дальности МОНО 2", 10, fini );
// Дальность 2-й цели в ини-файле в метрах
    //dsp_imitat.daln2 = GetPrivateProfileInt( "Имитация", "Дальность цели2", 2000, fini );

    GetPrivateProfileString( "Имитация", "Амплитуда сигнала ЧМ", "1",ss,256, fini );
    dsp_param.amplituda =atof(ss);
    GetPrivateProfileString( "Имитация", "Амплитуда сигнала МОНО", "1",ss,256, fini );
    dsp_param.amplitudam =atof(ss);
    GetPrivateProfileString( "Имитация", "Амплитуда сигнала ЧМ 2", "1",ss,256, fini );
    dsp_param.amplituda2 =atof(ss);
    GetPrivateProfileString( "Имитация", "Амплитуда сигнала МОНО 2", "1",ss,256, fini );
    dsp_param.amplituda2m =atof(ss);
// Мин дальность цели в ини-файле в метрах
    //dsp_imitat.min_daln = GetPrivateProfileInt( "Имитация", "Мин дальность ЧМ", 20000, fini );
    dsp_param.md=GetPrivateProfileInt( "Имитация", "Мин дальность ЧМ", 20000, fini );
    //if(dsp_param.md>dsp_param.Daln_MONO)
        dsp_param.md /= 96;
    //else
    //{
    //    if(dsp_param.aDECIMONO==32)
    //            dsp_param.md /= 48;
    //    else
    //            dsp_param.md /= 96;
    //}
// Частота Доплера 1-й цели ЧМ в ини-файле в Гц
    dsp_param.fd  = GetPrivateProfileInt( "Имитация", "Частота Доплера цели 1 ЧМ", 0, fini );
// Частота Доплера 2-й цели ЧМ в ини-файле в Гц
    dsp_param.fd2 = GetPrivateProfileInt( "Имитация", "Частота Доплера цели 2 ЧМ", 0, fini );
// Частота Доплера 1-й цели МОНО в ини-файле в Гц
    dsp_param.fdm  = GetPrivateProfileInt( "Имитация", "Частота Доплера цели 1 МОНО", 0, fini );
// Частота Доплера 2-й цели МОНО в ини-файле в Гц
    dsp_param.fd2m = GetPrivateProfileInt( "Имитация", "Частота Доплера цели 2 МОНО", 0, fini );

// Скорость 1-й цели в ини-файле в км/ч
    //dsp_imitat.vr1 = GetPrivateProfileInt( "Имитация", "Скорость цели1", 0, fini );
// Скорость 2-й цели в ини-файле в км/ч
    //dsp_imitat.vr2 = GetPrivateProfileInt( "Имитация", "Скорость цели2", 0, fini );
//////////////

// Угол первой цели задается в градусах
    //dsp_param.DALNm = GetPrivateProfileInt( "Имитация", "Азимут цели MONO", 300, fini ); // в градусах
    //dsp_param.DALNm*=(4096.0/360.0);
    //dsp_param.DALNm -=47;
    //if(dsp_param.DALNm <0)
    //     dsp_param.DALNm +=4096;
// Угол второй цели задается в градусах
    //dsp_param.Dasm = GetPrivateProfileInt( "Имитация", "Азимут цели2MONO", 250, fini );  // в градусах
    //dsp_param.Dasm =dsp_param.DALNm - dsp_param.Dasm*(4096.0/360.0);
// Дальность 1-й цели в ини-файле в метрах
    //dsp_param.DMaxm = GetPrivateProfileInt( "Имитация", "Дальность цели MONO", 18000, fini );
    //dsp_param.DMaxm=(dsp_param.aDECIMONO==32)?(dsp_param.DMaxm/48):(dsp_param.DMaxm/96);
// Дальность 2-й цели в ини-файле в метрах

    //dsp_param.DMaxm-(dsp_param.aDECIMONO==32)?(dsp_param.Ddalm/48):(dsp_param.Ddalm/96);

    //GetPrivateProfileString( "Имитация", "Амплитуда сигнала MONO", "1",ss,256, fini );
    //dsp_param.amplitudam =atof(ss);
    //GetPrivateProfileString( "Имитация", "Амплитуда сигнала MONO 2", "1",ss,256, fini );
    //dsp_param.amplituda2m =atof(ss);

// Мин дальность цели в ини-файле в метрах
    //dsp_imitat.min_dalnm = GetPrivateProfileInt( "Имитация", "Мин дальность целейMONO", 20000, fini );

// Частота Доплера 1-й цели в ини-файле в Гц
    //dsp_param.fd  = GetPrivateProfileInt( "Имитация", "Частота Доплера цели1", 0, fini );
// Частота Доплера 2-й цели в ини-файле в Гц
    //dsp_param.fd2 = GetPrivateProfileInt( "Имитация", "Частота Доплера цели2", 0, fini );

// Скорость 1-й цели в ини-файле в км/ч
    //dsp_imitat.vr1 = GetPrivateProfileInt( "Имитация", "Скорость цели1", 0, fini );
// Скорость 2-й цели в ини-файле в км/ч
    //dsp_imitat.vr2 = GetPrivateProfileInt( "Имитация", "Скорость цели2", 0, fini );
    //dsp_param.DELTAD=GetPrivateProfileInt( "Имитация", "Приращение дальности ЧМ", 10, fini );
    //dsp_param.DELTADm=GetPrivateProfileInt( "Имитация", "Приращение дальности МОНО", 10, fini );
    //dsp_param.DELTAD2m=10;
///////////////

// перевод единиц измерения для имитации
    //perevod_ed();
    //otobrajenie = GetPrivateProfileInt( "Параметры", "Отображение из файла", 1, fini );
/*
// ******************   СЕКЦИЯ ОЧИСТКА  **********************************************************************
    // Параметры очистки
   GetPrivateProfileString( "Очистка", "Дальность", "160000",ss,256, fini );
   Daln=atof(ss);
   GetPrivateProfileString( "Очистка", "Скорость", "0",ss,256, fini );
   VR=atof(ss);
   vrm=VR/3.6;
*/

   fprintf(flog,"\n\n **************** Сообщения об ошибках ************************\n\n");

 // *******************************************************************************
    // Загружаем тестовый сигнал
 // *******************************************************************************

    //if(kom!=0)
    {
        //if(OpenDialog1->Execute())
        {
            fq=fopen(imj_file_test_hm1,"r");
            if(fq==NULL)
            {
                //Nom_Kml=
                MessageBox(NULL,imj_file_test_hm1,"Неверно имя файла тестовых сигналов ЧМ 1",MB_OK);
                return 1;
            }
            fscanf(fq,"%d",&k);//Количество отсчетов ЧМ 1 канала
            test_sig[0]=complex<short int>(k,k);
            if(k<=0 || k>400)
            {
                MessageBox(NULL,imj_file_test_hm1,"Неверны данные в  файле тестовых сигналов",MB_OK);
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
                MessageBox(NULL,imj_file_test_hm2,"Неверно имя файла тестовых сигналов ЧМ 2",MB_OK);
                return 1;
            }
            fscanf(fq,"%d",&k4);//Количество отсчетов ЧМ 2 канала
            if(k4<=0 || k4>400)
            {
                MessageBox(NULL,imj_file_test_hm2,"Неверны данные в  файле тестовых сигналов",MB_OK);
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
                MessageBox(NULL,imj_file_test_mono1,"Неверно имя файла тестовых сигналов МОНО 1",MB_OK);
                return 1;
            }
            fscanf(fq,"%d",&k4);//Количество отсчетов МОНО 1 канала
            if(k4<=0 || k4>400)
            {
                MessageBox(NULL,imj_file_test_mono1,"Неверны данные в  файле тестовых сигналов",MB_OK);
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
                MessageBox(NULL,imj_file_test_mono2,"Неверно имя файла тестовых сигналов МОНО 2",MB_OK);
                return 1;
            }
            k+=k4+1;
            fscanf(fq,"%d",&k4);//Количество отсчетов МОНО 2 канала
            if(k4<=0 || k4>400)
            {
                MessageBox(NULL,imj_file_test_mono2,"Неверны данные в  файле тестовых сигналов",MB_OK);
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
    // ЗАГРУЖАЕМ Фильтр Сжатия - а что будет если имя неверно???
 // *******************************************************************************
        //if(OpenDialog1->Execute())
        {
            //fq=fopen(OpenDialog1->FileName.c_str(),"r");
            fq=fopen(imj_file_cjat_hm1,"r");
            if(fq==NULL)
            {
                MessageBox(NULL,imj_file_cjat_hm1,"Неверно имя файла фильтров сжатия HM1",MB_OK);
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
                MessageBox(NULL,imj_file_cjat_hm2,"Неверно имя файла фильтров сжатия HM2",MB_OK);
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
                MessageBox(NULL,imj_file_cjat_mono1,"Неверно имя файла фильтров сжатия MONO1",MB_OK);
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
                MessageBox(NULL,imj_file_cjat_mono2,"Неверно имя файла фильтров сжатия MONO2",MB_OK);
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
      //  загружаем фильтры MTD
   // *******************************************************************************
    //if(OpenDialog1->Execute())
    {
        fq=fopen(imj_file_mtd,"r");
        if(fq==NULL)
        {
            MessageBox(NULL,imj_file_mtd,"Неверно имя файла МТD",MB_OK);
            return 1;
        }
        if(dsp_param.per_lhm>10)
        {
            MessageBox(NULL,"Слишком много периодов (надо<=10)","Error",MB_OK);
            return 1;
        }
        for(unsigned int i=0;i<dsp_param.per_lhm+1;i++)
        {
            fscanf(fq,"%d",&Period[i]);
            //x1=Period[i]*0.1+0.05;
            //Period[i]=x1;
        }
        LL=Period[dsp_param.per_lhm];
// Переставлять периоды не надо
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
    // Загрузка программы ДДС
 // *******************************************************************************

    //ProgrammPath=ExtractFilePath(Application->ExeName);
    //SetCurrentDir(ProgrammPath);
    
		ProgrammPath = GetCurrentDirectoryMC();
		SetCurrentDirectoryA(ProgrammPath.c_str());
		
		ProgPath= ProgrammPath+ prog_ddc ;
    // --- TODO  items=ReadPrgFile(ProgPath.c_str() /*prog_ddc*/, g_aProgArray, sizeof(g_aProgArray)/sizeof(g_aProgArray[0] ));
    if(items==-1)
    {
        sprintf(ss,"Нельзя открыть файл: %s",  prog_ddc);
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
                Form4->Button7->Caption="От компьютера";

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
                Form4->Button7->Caption="Дистанционное";

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
    if(dsp_param.met_o==1)// Выключено метео
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
            if(dsp_param.met_o==0) // Включено метео
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
       Копируем структуру dsp_param для восстановления значений (ВАРУ и так далее) в случае отказа от ввода
*/
    unsigned int size;
    int *q1,*q2;

    size=sizeof(dsp_param);
    size&=0xfffffff0;

    // скопируем побайтно всю структуру
    q1=(int*)&dsp_param;
    q2=(int*)&dsp_param_file;

     for(unsigned int i=0;i<size/sizeof(int);i++)
       q2[i]= q1[i];
}

/*
void exit()
{

//     Функция завершения работы с платой

    U32 err;
    double tim;
    err=0;
    tim=Now();
        fprintf(flog,"\n%-60s%s","Конец работы ",DateTimeToStr(tim).c_str());
        fclose(flog);
    Exit=1;
}
*/
/*
char *S[256]={"Смена режима","прием номера комплекта","Кнопка задержка нуля дальности","порог для записи в карту помех",
"константы регуляризации ЧМ и МОНО","коэффициент сглаживания карты помех ЧМ","порог обнаружения ЧМ","Порог из карты",
"изменение дистанции ВАРУ 1 канала ЧМ (НУ)","изменение дистанции ВАРУ 2 канала ЧМ (ВУ)","изменение дистанции ВАРУ 1 канала МОНО (НУ)",
"изменение дистанции ВАРУ 2 канала МОНО (ВУ)","изменение кривой ВАРУ 1 канала ЧМ (НУ)","изменение кривой ВАРУ 2 канала ЧМ (ВУ)",
"изменение кривой ВАРУ 1 канала МОНО (ВУ)","изменение кривой ВАРУ 2 канала МОНО (ВУ)","Порог ограничения ЧМ","Порог ограничения MONO",
"Медианная фильтрация","Test номер HM","Test номер MONO","ожидание перезагрузки","Число обзоров для накопления ",
"Усиление в нуле ВАРУ ЧМ НУ","Усиление в нуле ВАРУ ЧМ ВУ","Усиление в нуле ВАРУ МОНО НУ","Усиление в нуле ВАРУ МОНО ВУ",
"коэффициент порога для МОНО","Включение NU","Включение VU","Множитель ограничения ЧМ","Множитель ограничения МОНО",
"Дистанционное управление","Вкл/выкл МТД","Вкл/выкл накопление","Вкл/выкл ЧМ","Вкл/выкл МОНО","Коэффициент усиления метео",
"Вкл/выкл режим метео","Передача команды выдачи информации в режиме метео","Передача коэффициента усиления для отображения",
"Передача включение.выключение сопровождения","Запись в файл видео","Запись в файл видео","Запись в файл","Фиксация карты помех"};
*/
char S[70][80];
/*
//S[0]=
strcpy(S[0],"Смена режима");
//S[1]=
strcpy(S[1],"Прием номера комплекта");
//S[2]=
strcpy(S[2],"Кнопка задержка нуля дальности");
//S[3]=
strcpy(S[3],"Порог для записи в карту помех");
//S[4]=
strcpy(S[4],"Константы регуляризации ЧМ и МОНО");
//S[5]=
strcpy(S[5],"Коэффициент сглаживания карты помех ЧМ");
//S[6]=
strcpy(S[6],"Порог обнаружения ЧМ");
//S[7]=
strcpy(S[7],"Порог из карты");
//S[8]=
strcpy(S[8],"Изменение дистанции ВАРУ 1 канала ЧМ (НУ)");
//S[9]=
strcpy(S[9],"Изменение дистанции ВАРУ 2 канала ЧМ (ВУ)");
//S[10]=
strcpy(S[10],"Изменение дистанции ВАРУ 1 канала МОНО (НУ)");
//S[11]=
strcpy(S[11,"Изменение дистанции ВАРУ 2 канала МОНО (ВУ)"]);
//S[12]=
strcpy(S[12],"Изменение кривой ВАРУ 1 канала ЧМ (НУ)");
//S[13]=
strcpy(S[13],"Изменение кривой ВАРУ 2 канала ЧМ (ВУ)");
//S[14]=
strcpy(S[14],"Изменение кривой ВАРУ 1 канала МОНО (ВУ)");
//S[15]=
strcpy(S[15],"Изменение кривой ВАРУ 2 канала МОНО (ВУ)");
//S[16]=
strcpy(S[16],"Порог ограничения ЧМ");
//S[17]=
strcpy(S[17],"Порог ограничения MONO");
//S[18]=
strcpy(S[18],"Медианная фильтрация");
//S[19]=
strcpy(S[19],"Test номер HM");
//S[20]=
strcpy(S[20],"Test номер MONO");
//S[21]=
strcpy(S[21],"Ожидание перезагрузки");
//S[22]=
strcpy(S[22],"Число обзоров для накопления ");
//S[23]=
strcpy(S[23],"Усиление в нуле ВАРУ ЧМ НУ");
//S[24]=
strcpy(S[24],"Усиление в нуле ВАРУ ЧМ ВУ");
//S[25]=
strcpy(S[25],"Усиление в нуле ВАРУ МОНО НУ");
//S[26]=
strcpy(S[26],"Усиление в нуле ВАРУ МОНО ВУ");
//S[27]=
strcpy(S[27],"Коэффициент порога для МОНО");
//S[28]=
strcpy(S[28],"Включение NU");
//S[29]=
strcpy(S[29],"Включение VU");
//S[30]=
strcpy(S[30],"Множитель ограничения ЧМ");
//S[31]=
strcpy(S[31],"Множитель ограничения МОНО");
//S[32]=
strcpy(S[32],"Дистанционное управление");
//S[33]=
strcpy(S[33],"Вкл/выкл МТД");
//S[34]=
strcpy(S[34],"Вкл/выкл накопление");
//S[35]=
strcpy(S[35],"Вкл/выкл ЧМ");
//S[36]=
strcpy(S[36],"Вкл/выкл МОНО");
//S[37]=
strcpy(S[37],"Коэффициент усиления метео");
//S[40]=
strcpy(S[39],"Вкл/выкл режим метео");
//S[50]=
strcpy(S[40],"Передача команды выдачи информации в режиме метео");
//S[51]=
strcpy(,S[41],"Передача коэффициента усиления для отображения");
//S[52]=
strcpy(S[42],"Передача включение.выключение сопровождения");
//S[54]=
strcpy(S[43],"Запись в файл видео");
//S[55]=
strcpy(S[44],"Конец записи в файл видео");
//S[56]=
strcpy(S[60],"Перезагрузка");
//S[57]=
strcpy(S[45],"Фиксация карты помех");
*/

extern int chet_err;
int komandos(int kom_da) // функция передачи команды в процессор
{ // kom_da от 0 до ... в соответствии со списком
  // функция возвращает 1, если все успешно,
  // или 0, если ошибка
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
strcpy(S[0],"Смена режима");
//S[1]=
strcpy(S[1],"Прием номера комплекта");
//S[2]=
strcpy(S[2],"Кнопка задержка нуля дальности");
//S[3]=
strcpy(S[3],"Порог для записи в карту помех");
//S[4]=
strcpy(S[4],"Константы регуляризации ЧМ и МОНО");
//S[5]=
strcpy(S[5],"Коэффициент сглаживания карты помех ЧМ");
//S[6]=
strcpy(S[6],"Порог обнаружения ЧМ");
//S[7]=
strcpy(S[7],"Порог из карты");
//S[8]=
strcpy(S[8],"Изменение дистанции ВАРУ 1 канала ЧМ (НУ)");
//S[9]=
strcpy(S[9],"Изменение дистанции ВАРУ 2 канала ЧМ (ВУ)");
//S[10]=
strcpy(S[10],"Изменение дистанции ВАРУ 1 канала МОНО (НУ)");
//S[11]=
strcpy(S[11],"Изменение дистанции ВАРУ 2 канала МОНО (ВУ)");
//S[12]=
strcpy(S[12],"Изменение кривой ВАРУ 1 канала ЧМ (НУ)");
//S[13]=
strcpy(S[13],"Изменение кривой ВАРУ 2 канала ЧМ (ВУ)");
//S[14]=
strcpy(S[14],"Изменение кривой ВАРУ 1 канала МОНО (ВУ)");
//S[15]=
strcpy(S[15],"Изменение кривой ВАРУ 2 канала МОНО (ВУ)");
//S[16]=
strcpy(S[16],"Порог ограничения ЧМ");
//S[17]=
strcpy(S[17],"Порог ограничения MONO");
//S[18]=
strcpy(S[18],"Медианная фильтрация");
//S[19]=
strcpy(S[19],"Test номер HM");
//S[20]=
strcpy(S[20],"Test номер MONO");
//S[21]=
strcpy(S[21],"Ожидание перезагрузки");
//S[22]=
strcpy(S[22],"Число обзоров для накопления ");
//S[23]=
strcpy(S[23],"Усиление в нуле ВАРУ ЧМ НУ");
//S[24]=
strcpy(S[24],"Усиление в нуле ВАРУ ЧМ ВУ");
//S[25]=
strcpy(S[25],"Усиление в нуле ВАРУ МОНО НУ");
//S[26]=
strcpy(S[26],"Усиление в нуле ВАРУ МОНО ВУ");
//S[27]=
strcpy(S[27],"Коэффициент порога для МОНО");
//S[28]=
strcpy(S[28],"Включение NU");
//S[29]=
strcpy(S[29],"Включение VU");
//S[30]=
strcpy(S[30],"Множитель ограничения ЧМ");
//S[31]=
strcpy(S[31],"Множитель ограничения МОНО");
//S[32]=
strcpy(S[32],"Дистанционное управление");
//S[33]=
strcpy(S[33],"Вкл/выкл МТД");
//S[34]=
strcpy(S[34],"Вкл/выкл накопление");
//S[35]=
strcpy(S[35],"Вкл/выкл ЧМ");
//S[36]=
strcpy(S[36],"Вкл/выкл МОНО");
//S[37]=
strcpy(S[37],"Коэффициент усиления метео");
strcpy(S[38],"Вкл/выкл режим метео");
//S[40]=
strcpy(S[39],"Авария");//"Вкл/выкл режим метео");
//S[50]=
strcpy(S[40],"Передача команды выдачи информации в режиме метео");
//S[51]=
strcpy(S[41],"Передача коэффициента усиления для отображения");
//S[52]=
strcpy(S[42],"Передача включение.выключение сопровождения");
//S[54]=
strcpy(S[43],"Запись в файл видео");
//S[55]=
strcpy(S[44],"Конец записи в файл видео");
//S[56]=
strcpy(S[60],"Включение тестового сигнала");
strcpy(S[61],"Выключение тестового сигнала");
//S[57]=
strcpy(S[45],"Фиксация карты помех");


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
    mes[0]|=(kom_da<<16); // передаем 0-ым параметром номер команды
    kmmm=kom_da;
    mmm=mes[1];
//    z=y[2];
    
    switch(kom_da)
        {
          case 0:
              rejim= mes[0];
/**/          //rejim=rejim; // смена режима
              mmm=rejim;
              break;
/**/      case 1:   // прием номера комплекта
              //mes[1] = dsp_param.Nomer_komplekta;
              break;
/**/      case 2:   // задержка нуля дальности ЧМ и МОНО
             // mes[1] = dsp_param.zad_lhm;
             // mes[2] = dsp_param.zad_mono;
              mmm=y[1];
              break;
/**/      case 3: // порог для записи в карту помех
              //y[1] = dsp_param.porog;
              mmm=y[1];
              break;
/**/      case 4: // константы регуляризации ЧМ и МОНО
              //y[1] = dsp_param.PL_K_Phm;
              //y[2] = dsp_param.PL_K_Pmono;
                mmm=y[1];
              break;
/**/      case 5: // коэффициент сглаживания карты помех ЧМ
              //y[1] = dsp_param.beta;
              mmm=y[1];
              break;
/**/      case 6: // порог обнаружения ЧМ
              //y[1] = dsp_param.PorogL;
              mmm=y[1];
              break;
/*--*/      case 7: // множитель преобразования чисел с плавающей точкой в целые
              //mes[1]= dsp_param.float_to_int;
              break;
/**/      case 8: // изменение дистанции ВАРУ 1 канала ЧМ (НУ)
              //mes[1]= dsp_param.d_Wary1;
	      //mes[1]=mes[1]*z+0.5;
              mmm=mes[1]*z+0.5;
              break;
/**/      case 9: // изменение дистанции ВАРУ 2 канала ЧМ (ВУ)
              //mes[1]= dsp_param.d_Wary2;
              //mes[1]=mes[1]*z+0.5;
              mmm=mes[1]*z+0.5;
              break;
/**/      case 10: // изменение дистанции ВАРУ 1 канала МОНО (НУ)
              //mes[1] = dsp_param.d_Wary1m;
              //mes[1]=mes[1]*z+0.5;
              mmm=mes[1]*z1+0.5;
              break;
/**/      case 11: // изменение дистанции ВАРУ 2 канала МОНО (ВУ)
              //mes[1] = dsp_param.d_Wary1m;
              //mes[1]=mes[1]*z+0.5;
              mmm=mes[1]*z1+0.5;
              break;
/**/      case 12: // изменение кривой ВАРУ 1 канала ЧМ (НУ)
              //mes[1]= dsp_param.wid1;
              break;
/**/      case 13: // изменение кривой ВАРУ 2 канала ЧМ (ВУ)
              //mes[1]= dsp_param.wid2;
              break;
/**/      case 14: // C
              //mes[1]= dsp_param.wid1m;
              break;
/**/      case 15: // изменение кривой ВАРУ 2 канала МОНО (ВУ)
              //mes[1]= dsp_param.wid2m;
              break;
/**/      case 16: // Порог ограничения ЧМ
              //y[1] = dsp_param.con;
              mmm=y[1];
              break;
/**/      case 17:// Порог ограничения MONO
              //y[1] = dsp_param.con_m;
              mmm=y[1];
              break;
/*--*/      case 18: // Медианная фильтрация
              //mes[1] =dsp_param.median;
              break;
/**/      case 19: // Test номер HM ?
              //mes[1] =dsp_param.nom_hm;
              break;
/**/      case 20: // Test номер MONO ?
              //mes[1] =dsp_param.nom_mono;
              break;
          case 21:  // ожидание перезагрузки
              //      N_XOCT=StrToInt(Dial);
              return 1; // вернем что все хорошо
/*--*/      case 22: // Число обзоров для накопления (=10)
              //mes[1] = dsp_param.nak;
              break;
/**/      case 23:// Усиление в нуле ВАРУ ЧМ НУ
              //y[1] = dsp_param.us0hm1;
              mmm=y[1];
              break;
/**/      case 24:// Усиление в нуле ВАРУ ЧМ ВУ
              //y[1] = dsp_param.us0hm2;
              mmm=y[1];
              break;
/**/      case 25:// Усиление в нуле ВАРУ МОНО НУ
              //y[1] = dsp_param.us0mono1;
              mmm=y[1];
              break;
/**/      case 26:// Усиление в нуле ВАРУ МОНО ВУ
              //y[1] = dsp_param.us0mono2;
              mmm=y[1];
              break;
/**/      case 27: // коэффициент порога для МОНО
              //y[1] = dsp_param.energij;
              mmm=y[1];
              break;
          case 28: // Включение NU
              //mes[1]=dsp_param.NU;
              break;
          case 29: // Включение VU
              //mes[1]=dsp_param.VU;
              break;
          case 30: // Множитель ограничения ЧМ
              //y[1] = dsp_param.sq_con;
              mmm=y[1];
              break;
          case 31: // Множитель ограничения МОНО
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
		//		ms|=dsp_param.MTD<<8;// Вкл/выкл МТД
				break;

          case 34:
                //mes[1]=dsp_param.Nakoplenie;
				//ms &= ~(1<<9);
				//ms|=dsp_param.Nakoplenie<<9;// Вкл/выкл накопление
                break;

			case 35:
				//dsp_param.WKL_HM=;
				//ms &= ~(1<<6);
				//ms|=dsp_param.WKL_HM<<6;// Вкл/выкл ЧМ
                                //mes[1] = dsp_param.WKL_HM ;
				break;
			case 36:
				//dsp_param.WKL_MONO=mes[1];
				//ms &= ~(1<<7);
				//ms|=dsp_param.WKL_MONO<<7;// Вкл/выкл МОНО
                                //mes[1]= dsp_param.WKL_MONO ;
				break;

          case 37:

          // Коэффициент усиления метео
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
			// Авария
				if(mes[1]==1)
				{
					i=~0x100;
				}
				else
					i=0x100;
				DRV_ctrl( g_hDrv, DDDcmd_PIOWRITE, &i );	// Выдать или потушить сигнал "АВАРИЯ"
				break;
                        */
          case 39:
          kmmm=39;
          //mes[1]=39;
          break;
          case 40:
          kmmm=40;
          // Вкл/выкл режим метео
                 //mes[1]=dsp_param.met_o^1;
				//ms&=~(1<<12);
				//ms |= mes[1];
                 //mes[1]=1;
				break;
          case 50:
          kmmm=40;
          // Передача команды выдачи информации в режиме метео
          break;
          case 51:
          kmmm=41;
          // Передача коэффициента усиления для отображения
                k_usil=y[1];
                return 0;
          case 52:
          kmmm=42;
          // Передача включение.выключение сопровождения
                Wkl_Sop=mes[1];
                if (Wkl_Sop) // если фильтр включается, то его инициализация
                {
                /*
                        MyInit_Soprov=(int(__import*)(char *c))GetProcAddress(Dll,"_Init_Soprov");
                        if(MyInit_Soprov==NULL)
                        {
                                MessageBox(NULL,"Нет файлов Init_soprov в библиотеке DLL","Error",MB_OK);
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
          // Запись в файл видео
                sap_file=1;
                //GetPrivateProfileString("Параметры", "Множитель ограничения ЧМ", "1e-4",ss,256, fini );
                GetPrivateProfileString("Запись в файл","Файл","",ss,256,fini);
                Objem=GetPrivateProfileInt("Запись в файл","Объем",200,fini);
                fp=fopen(ss,"wb");
                return 0;
          case 56:
          kmmm=44;
                K_K=0;
                sap_file=0;
                sap_file1=1;
                fclose(fp);
                fp=NULL;
                WritePrivateProfileString("Запись в файл","Запись","0",fini);
                return 0;
          case 57:
          kmmm=45;
                // Фиксация карты помех
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
         //       MessageBox(NULL,"Команда не передана во 2 процессор","Error",MB_OK);
         mes_test[3]=kmmm;
      }
      Sleep(100);
}
time(&tim);// =Now();
mmm=mes[1];
if(kmmm == 40)
{
        if(mes[1]==1)
                fprintf(flog,"\n  %s koef=%f %s","Выдача в ЦАП МЕТЕО",y[1],DateTimeToStr(tim).c_str());
        else
                fprintf(flog,"\n  %s koef=%f %s","Вывод на экран в режиме МЕТЕО",y[1],DateTimeToStr(tim).c_str());
}
else
if(kmmm == 37)
{
    fprintf(flog,"\n  %s dsp_param.MTD=%d koef=%f %s",S[kmmm],dsp_param.MTD,y[1],DateTimeToStr(tim).c_str());
}
else if(kmmm == 38)
{
        if(mes[1]==1)
           strcpy(S[38],"Вкл режим метео канал 0");
        if(mes[1]==2)
           strcpy(S[38],"Вкл режим метео канал 1");
        if(mes[1]==3)
           strcpy(S[38],"Вкл режим метео канал 2");
        if(mes[1]==4)
           strcpy(S[38],"Вкл режим метео канал 3");
        fprintf(flog,"\n  %s mes=%d koef=%f %s",S[kmmm],mes[1],y[2],DateTimeToStr(tim).c_str());
}
else if(kmmm == 39)
{
         if(mes[1]==1)
                fprintf(flog,"\n Передана команда: Авария           %s",DateTimeToStr(tim).c_str());
         if(mes[1]==0)
                fprintf(flog,"\n Передана команда: Авария устранена %s",DateTimeToStr(tim).c_str());
         fflush(flog);
}
else if(kmmm == 6)
         fprintf(flog,"\n Множитель порога обнаружения=%f %s",y[1],DateTimeToStr(tim).c_str());
else if(kmmm == 27)
         fprintf(flog,"\n Множитель порога обнаружения для МОНО=%f %s",y[1],DateTimeToStr(tim).c_str());
else
        fprintf(flog,"\n  %s mes=%f y[2]=%f %s",S[kmmm],mmm,y[2],DateTimeToStr(tim).c_str());
//fprintf(flog,"\n%-60s%s","Перезагрузка из-за ошибок инициализации ",DateTimeToStr(tim).c_str());

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
    //     MessageBox(NULL,"Ошибка при передаче команды в 1 процессор","Error",MB_OK);
    //     return 0;        // Ошибка
    //  }

}


void perevod_ed()
{       
    dsp_param.DALN=dsp_imitat.az1*4096.0/360.0-47;  // азимут 1-й цели в МАИ
    if(dsp_param.DALN<0)
        dsp_param.DALN+=4096;
    //dsp_param.DALN=dsp_param.DALN%4096; // ограничение макс МАИ
    dsp_param.Das=(dsp_imitat.az2-dsp_imitat.az1)*4096.0/360.0; // азимут 2-й цели относительно 1-й в МАИ
    if(dsp_param.Das>=4096)
        dsp_param.Das-=4096;
    if(dsp_param.Das<0)
        dsp_param.Das+=4096;
    //dsp_param.Das=dsp_param.Das%4096; // ограничение макс МАИ
    if( dsp_imitat.daln1>dsp_param.Daln_MONO)
        dsp_param.DMax = dsp_imitat.daln1/96; // дальность 1-й цели в дискретах
    else if( dsp_imitat.daln1<dsp_param.Daln_MONO && dsp_imitat.daln1 >0)
        dsp_param.DMax = dsp_imitat.daln1/48;
    else
    {
        //MessageBox(NULL,"Неверно указана максимальная дальность имитационной отметки","Error",MB_OK);
        dsp_param.DMax=0;
    }
    dsp_param.DMax = dsp_param.DMax%1695; // ограничение макс дальности

    dsp_param.Ddal = (dsp_imitat.daln2 - dsp_imitat.daln1)/96;//*1695.0/dsp_param.Daln_HM; // приращение 2-й цели относительной 1-й в дискретах
    dsp_param.Ddal = dsp_param.Ddal%1695; // ограничение макс дальности
    if(dsp_imitat.min_daln>dsp_param.Daln_MONO)
        dsp_param.md = dsp_imitat.min_daln/96;//*1695.0/dsp_param.Daln_HM; // мин дальность в дискретах
    else
        dsp_param.md = dsp_imitat.min_daln/48;
    dsp_param.md = dsp_param.md%1695;  // ограничение мин дальности
    dsp_param.DELTAD = (dsp_imitat.vr1*1000.0/3600.0)*dsp_param.tobs; // выразим перемещение, которую пройдет цель1 с vr (в метрах) за время обзора

    dsp_param.DELTAD = dsp_param.DELTAD/96;//*1695.0/dsp_param.Daln_HM; // перевод перемещения в дискреты в области ЧМ
    dsp_param.DELTAD = dsp_param.DELTAD%1695; // ограничение макс дальности

    dsp_param.DELTAD2 = (dsp_imitat.vr2*1000.0/3600.0)*dsp_param.tobs; // выразим перемещение, которую пройдет цель1 с vr (в метрах) за время обзора
    dsp_param.DELTAD2 = dsp_param.DELTAD2/96;//*1695.0/dsp_param.Daln_HM; // перевод перемещения в дискреты
    dsp_param.DELTAD2 = dsp_param.DELTAD2%1695; // ограничение макс дальности

}
