// Potokexeex1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <winbase.h>
#include <filesystem>
#include <ctime>
#include <string>
#include <conio.h>
#include <complex>

// Локальные модули и заголовочные файлы
#include "./inc/Bsp_common.h"
#include "./inc/dsp_param.h"
#include "./inc/sundries.h"
#include "./inc/buf.h"
#include "./inc/var.h"
#include "./inc/library_k.h"


#define D12 12
#define MIN(x,y) (((x)<(y))?(x):(y))


using namespace std;

// Переменные
// ***************************
// Режимы работы - 0 - вывод в Осциллограф исходного несжатого сигнала
//                 1 - вывод в Осциллограф сжатого сигнала
//                 4 - вывод в Кико полной обработки
int rejim = 4;
int Wkl_Sop = 0; // включение/выключение сопровождения
// ***************************
volatile PBsp_Status	status = NULL, status2 = NULL;
int kod_rpu = 0;
int komanda0 = 0, komanda;
int kod_vosvrata;
int off = 0;
int chet_time = -1000;
void wwiwod(void);
int start; // запуск с FOrm4
QQQ *uks_QQQ, *uks_QQQ1;
Buf_HM_MONO* buffer = NULL;
QQQ * buffer2 = NULL;
Buf_HM_MONO *uks_Buf_HM = buffer;
RRR rrr;
BBB rbb, rbbM;
int size_buf;
unsigned int Daln_HM, Daln_MONO, sizeHM, sizeMONO;
int size_HM, size_MONO;
int sap_ozenki = 0;
static int kol_povt = 3;
int d_count = 0;
int *Hapka_HM;
TIP_4 bbb;
float *test_buf;
int avt_per=1;
HANDLE hCardExit = CreateEvent(NULL, FALSE, FALSE, "BspExitEvent"), hCardInit=CreateEvent(NULL, FALSE, FALSE, "BspInitEvent");
float max_time;
float PorogL;
int ch;
double T_obs;
int n_obs=0;
int *mes_test;
int exit_err=0;
unsigned int k=0;
double k_usil=1;
// Переменные для записи в файл видео и оценок
int sap_file=0, sap_file1=0, sap_file_oz=0;
unsigned int K_K=0, K_K_oz=0;
unsigned int Objem=200;
double objem_sap_QQQ=sizeof(QQQ) / double(1 << 20);
union
{
	int x;
	short int y[2];
}asd;
complex<float> CV;
double konst_time;


// Указатели на файлы отображаемые в память
HANDLE hFileMapStatus = NULL, hFileMapStatus2 = NULL, hFileMapBuffer = NULL, hFileMapBuffer2 = NULL;
LPVOID lpViewStatus   = NULL, lpViewStatus2   = NULL;
LPVOID lpViewBuffer   = NULL, lpViewBuffer2   = NULL;

// Внешние переменные
extern PARAM_DSP dsp_param, dsp_param_1;
extern int Exit = 0;
extern int Oz_file;

// Указатели на файлы
FILE *fp, *flog, *foz, *fp_oz;
FILE *fpq;
int(*MySoprov_otm)(int *p, int kkkk);
int(*MyInit_Soprov)(char *c);
void(*pFunction)(int, int);

// Имена файлов и рабочих директорий
string ProgrammPath;
string ProgPath;
string Oh;
char fname[256] = ".\\rpu.ini";
char fini[256] = ".\\mtd__n.ini";
char rpu[256];

// Прототипы процедур и функций
void openLogFile(void);
void Sap_File_Oz(void);
void Sap_File_Karta(void);

int main()
	{
		// Переменные
		time_t tim;
		struct tm *timeCurInfo;
	
		static int ttt = 0;
		int qqq1 = 0;
		static int tyu = 0;
		int nom_kom = -1;
		int pr_fifo = 0;
		float x, y;
		int off2 = 0;
		int komanda61 = 0;
		float nn;
		static int kod_oh = 0;
		int nom_obs_old, kol_sond_old, kol_sond;
		static int nom_sond = 0, tek_sond = 0, tek_mai = 0;
		static int pr_per = 0, uytra = 0, uytrn = 0;
		int temp_rez = 0, nom_sap = 0, nom_er;
		int nach = 0;
		QQQ uksQQQ1;
		double konst_time;
		char ss[255];
		static int cper=0;
		int *w;
		int nom_sek=0, nom_sek_old=0;
		int delta_mai=4096 / D12;
		int zwet;
		static int c_oh[10]= { 0 };
		int kod_kk=0;
		string soob;
		int obsorold=0, kolotm=0;
		int n_kom;
		double *dtm;  // TODO не будет работать должным образом, старый формат времени 
		static int kol_otm_obsor=0;
		int Oh_dan=0;
		unsigned int err;

		OpenFileMap();

		// Инициализация мьютекса, защищающего от повторного запуска процесса?
		// выполнение потока
		HANDLE hMutex;
		hMutex = CreateMutex(NULL, TRUE, "Prov_exe");// Создается объект синхронизации
		if (hMutex != NULL)
		{				                                  //Если объект уже создан, то выход                       ,
			if (ERROR_ALREADY_EXISTS == GetLastError())
			{
				return 0;
			}
		}

		// Устанавливаем режим реального времени
		SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

		// Загрузка DLL и получение указателей на функции
		HMODULE soprovLibHnd;
		soprovLibHnd = LoadLibrary("SoprovDLL.dll");
		(FARPROC &)MySoprov_otm = GetProcAddress(soprovLibHnd, "_Soprov_otm");
		(FARPROC &)MyInit_Soprov = GetProcAddress(soprovLibHnd, "_Init_Soprov");
		if (MySoprov_otm == NULL || MyInit_Soprov == NULL)
		{
			MessageBoxMC("Нет функций Soprov_otm или Init_soprov в библиотеке DLL.");
			FreeLibrary(soprovLibHnd);
		}

		// Отправка события инициализации 
		HANDLE hCardInit;
		hCardInit = CreateEvent(NULL, FALSE, FALSE, "BspInitEvent");
		SetEvent(hCardInit);
		CloseHandle(hCardInit);

		// Получение статуса
		if (status != NULL)
			{
			BSP_SET_STATUS(status, cmdWORK);
			}

		// Получение текущей директории
		ProgrammPath = GetCurrentDirectoryMC();
		printf(ProgrammPath.c_str());
		//ProgrammPath = ExtractFilePath(Application->ExeName);
		SetCurrentDirectory(ProgrammPath.c_str());
		Oh = ProgrammPath + "mtd__n.ini";
		strcpy(fini, Oh.c_str());
		Oh = ProgrammPath + "cfg\\kchel\\rpu.ini";
		strcpy(rpu, Oh.c_str());
		kod_rpu = GetPrivateProfileInt("System", "NoConrad", 0, rpu);
	
		// Открываем log-файл
		openLogFile();
		deleteOzFile();
	
	nach_lo:
		nom_sond = 0; tek_sond = 0; tek_mai = 0;
		nom_obs_old = 0; kol_sond_old = 0; kol_sond = 0;
		pr_per = 0; uytra = 0; uytrn = 0;
		komanda = komanda61;
		nn = 0; kod_oh = 0;
		off2 = 0;
		off = 0;
		tyu = 0;
		ttt = 0;
		nom_sap = 0;

		if (status2 != NULL)
			{
	 		BSP_SET_COMMAND(status2, komanda);
	 		BSP_SET_STATUS(status2, komanda);
			}
	
		DWORD rpuAttrs;
		rpuAttrs = GetFileAttributesA((ProgrammPath + string(fname)).c_str());
		if(rpuAttrs != INVALID_FILE_ATTRIBUTES)  // если имеется rpu.ini
			WritePrivateProfileString("Bsp", "Exit", "0", fname);

		time(&tim);
		fprintf(flog, "\n\n*****************************************************************\n");
		fprintf(flog, "\n\n\n%-60s%s", "*****   Начало работы *****", DateTimeToStr(tim).c_str());
		fprintf(flog, "\n\n");
		fflush(flog);

		// если режим  = МТД, то обнуление буфера2
		if (rejim == 4)
			for (int i = 0; i<BUFFER_SIZE; i++)
			{
				buffer2[i] = uksQQQ1;
			}
		// обнуление переменных
		nn = 0;
		start = 0;
		nach = 0;
		off2 = 0;
		chet_time = -1000;
		temp_rez = 0;
		// Инициализируем плату
		//temp_rez = init_plata();
		if (temp_rez != 0)
		{
			int kkom;
			time(&tim);
			fprintf(flog, "\n%-60s%s", "Перезагрузка из-за ошибок инициализации ", DateTimeToStr(tim).c_str());
			MessageBox(NULL, "См. mtd.log. Откройте mtd__n.ini, исправьте и потом нажмите OK ", "Error", MB_OK);
			fflush(flog);
			kkom = 0;
			kkom = BSP_GET_COMMAND(status2);
			BSP_SET_STATUS(status, cmdWORK);
			while (kkom != cmdEx)
			{
				BSP_SET_STATUS(status, cmdWORK);
				kkom = BSP_GET_COMMAND(status2);
				Sleep(1000);
			}
			goto nach_lo;
		}
		// инициализация переменных
		konst_time = 2.0 / dsp_param.f_proz*1e6;
		size_buf = (2 * (sizeHM + sizeMONO) + 8) * sizeof(int);
		chet_time = 0;
		// ************************ ************************* **************************** **************
		// Установить связь с платой и осуществить обмен с ней в бесконечном цикле в отдельном потоке
		// ************************ ************************* **************************** **************

		/* Для работы фильтра сопровождения*/
		if (Wkl_Sop) // если фильтр включается, то его инициализация
		{
			strcpy(ss, ProgrammPath.c_str());
			MyInit_Soprov(ss);
		}
		if (sap_ozenki == 1)
			Sap_File_Oz();
		//if(sap_karta==1)
		//    Sap_File_Karta();
		kol_povt = 3;
		int c;
		int bbb4, bbb5;
		for (;; )  // бесконечный цикл
			{
			Exit = BSP_GET_STATUS(status2);
			komanda = BSP_GET_COMMAND(status2);
		
			// Команда начала новых суток (как я понял)
			if(komanda == 70)
				{
				komanda = 0;
				fclose(flog);
				// Закрыть файл flog и открыть его с новой датой.
				openLogFile();
				} // if(komanda == 70)

			if(Exit == 1 || (_kbhit() && (c = getch(), c == 99)))
				{
					time(&tim);
					if (Exit == 1)
						fprintf(flog, "\nПодана команда Exit=1");
					else
						fprintf(flog, "\nНажата клавиша с");
					// строка ниже сомнительного содержания
					//fprintf(flog, "\nМакс время выполнения=%f мкс", 2e6*max_time / dsp_param.f_proz);
					fprintf(flog, "\n%-60s%s", "Конец работы ", DateTimeToStr(tim).c_str());
					fclose(flog);
					// TODO BRD_cleanup();
					// TODO BRD_close(handle);
					return 0;
					//} if(Exit == 1 . . .
				}

			if (sap_ozenki == 1)
				{
					time(&tim);
					timeCurInfo = localtime(&tim);
					if (timeCurInfo->tm_min == 0 && qqq1 == 0)
						{
							qqq1 = 1;
							fclose(fpq);
							Sap_File_Oz();
						}
					else if (timeCurInfo->tm_min != 0)
						qqq1 = 0;
				}
		
			//////////////////////////////////////////
			// Установка указателей на нужную запись кольцевого буфера
			// По указателю uks_Buf_HM записываются данные во всех режимах, кроме режима 4
			// По указателю uks_qq записываются данные только в режиме 4
			// Разряди шапки передаваемых данных:
			// Hapka_HM[0]: 0..2 - режим работы
			//              3 - вкл/выкл НУ
			//              4 - вкл/выкл ВУ
			//              5 - ЧМ на всей дистанции или только до МОНО
			//              6 - ЧМ вкл/выкл
			//              7 - МОНО вкл/выкл
			//              8 - МТД вкл/выкл
			//              9 - накопление вкл/выкл
			//              10..11 - номер комплекта
			//              12 - метео (0-вкл/1-выкл)
			//              13..15 - режим метео (0-нет вывода,1-1-й канал, 2-2-й канал и т.д.)
			//              16 - управление от компьютера или дистанционное (0-дистанционное, 1- от компьютера)
			//              17 - 0х20 сигнал с регистра (сжатие)
			//              18 - 0х80 сигнал с регистра (сброс)
			if (rejim == 4) // сжатие, МТД
				{
				BSP_SET_STATUS(status, cmdWORK);
				BSP_SET_D_COUNT(status, d_count++);
				off = BSP_GET_COUNT(status);    // Для вывода в кико номер текущей записи
				uks_QQQ = buffer2 + off;        // Установка указателя на текущую запись
				}
			else
				{
				BSP_SET_STATUS(status, cmdWORK);
				BSP_SET_D_COUNT(status, d_count++);
				off2 = BSP_GET_COUNT(status2);  // Номер текущей записи приема
				uks_Buf_HM = buffer + off2;     // Установка указателя на текущую запись
				off = BSP_GET_COUNT(status);    // Для вывода в кико номер текущей записи
				uks_QQQ = buffer2 + off;        // Установка указателя на текущую запись
				}

			// Читаем данные из ФИФО


			// ************************************************************************************
			// Организуем обмен с платой, с проверкой максимального времени ожидания ответа от платы (N_XOCT)
			// ************************************************************************************
			if (rejim == 4) // читаем но не ждем
				{
					pr_fifo = 0;
					//BRD_readFIFO(handle,0,8,&bbb,sizeof(TIP_4),0);
					bbb4 = bbb.Hapka_HM[2];
					bbb5 = bbb.Hapka_HM[5];
					tyu = 0;
					// tyu = BRD_readFIFO(handle, 0, 8, &bbb, sizeof(TIP_4), -1);
					test_buf[39] = 1;
					if (pr_fifo == 1000 ||
						bbb.Hapka_HM[0] <= 0 || bbb.Hapka_HM[2] < 0 || bbb.Hapka_HM[3] < 0 || bbb.Hapka_HM[5]>4096 || tyu != 8224)
						{
						if (cper == 0)
							{
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Данные из платы не верны!", DateTimeToStr(tim).c_str());
							fflush(flog);
							}
						cper++;
						if (cper == 100)
							{
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Данные неверны! Перезагрузка ", DateTimeToStr(tim).c_str());
							fflush(flog);
							}
						else if (cper > 100)
							{
							BSP_SET_DEBUGCOUNT(status2, 1 << 9);
							// BRD_reset(handle, 0);
							// BRD_reset(handle, 1);
							// err = BRD_cleanup();
							goto nach_lo;
							}
					}
					else
						{
							if (cper != 0)
								{
									time(&tim);
									fprintf(flog, "\n%-60s%s", "Программа работает", DateTimeToStr(tim).c_str());
									fflush(flog);
								}
							cper = 0;
							test_buf[39] = 1;
							if (pr_fifo == 0 || uks_Buf_HM->Hapka_HM[0] <= 0 || uks_Buf_HM->Hapka_HM[3] < 0 || uks_Buf_HM->Hapka_HM[5]>4096)
								{
									if (cper == 0)
									{
										time(&tim);
										fprintf(flog, "\n%-60s%s", "Данные неверны! ", DateTimeToStr(tim).c_str());
										fflush(flog);
									}
									cper++;
									if (cper > 1)
										goto nach_lo;

								}
						} 
				}  // if (rejim == 4)
			else //режим=0
				{
					//pr_fifo = BRD_readFIFO(handle, 0, 8, uks_Buf_HM, sizeof(Buf_HM_MONO), -1);
					test_buf[39] = 1;
					if (pr_fifo == 0 || uks_Buf_HM->Hapka_HM[0] <= 0 || uks_Buf_HM->Hapka_HM[3] < 0 || uks_Buf_HM->Hapka_HM[5]>4096)
						{
							if (cper == 0)
								{
									time(&tim);
									fprintf(flog, "\n%-60s%s", "Данные неверны! ", DateTimeToStr(tim).c_str());
									fflush(flog);
								}
							cper++;
							if (cper > 1)
								{
									goto nach_lo;
								}
						}
					else
						cper=0;
				
					if (uks_Buf_HM->Hapka_HM[2] - nom_sap>1)
						{
							uks_Buf_HM->Hapka_HM[1]|=8;
							//cper=cper;
							nom_er=nom_sap;
						}
					nom_sap= uks_Buf_HM->Hapka_HM[2];
				} // режим=0

			// ************************************************************************************
			// Обработка внешних команд
			if (Exit == cmdEx)
				{
					// Перезагрузка
					avt_per=1;
					time(&tim);
					fprintf(flog, "\n%-60s%s", "Перезагрузка ", DateTimeToStr(tim).c_str());
					// fprintf(flog, "\nМакс время выполнения=%f мкс", 2e6*max_time / dsp_param.f_proz);//bbb.HM_Re_Im_1[1688]);
					fflush(flog);
				
					// TO DO BRD_reset(handle, 0);
					// TO DO BRD_reset(handle, 1);
					// TO DO err=BRD_cleanup();
					BSP_SET_STATUS(status, cmdWORK); // Перезагрузка без прекращения работы кико
					goto nach_lo;
				}
			
			if (komanda != 0)
				{
					if (komanda == 61 || komanda == 62)
						komanda61=komanda;
					if (komanda != 70)
					{
						kod_vosvrata=komandos(komanda - 1);
					}
					komanda=0;
					BSP_SET_COMMAND(status2, komanda);
				}

			// Операции с данными
			for (int i=0; i<size_HM; i++)
				{
					w = (int*)&bbb.HM_Re_Im_1[8 + i];
					if (w[0] == -1)
						nom_er=nom_er;
				}
			if (rejim == 4)  // поскольку указатели в режиме 4 отличаются от остальных
				Hapka_HM=(int *)&bbb.Hapka_HM[0];
			else
				Hapka_HM=(int *)&uks_Buf_HM->Hapka_HM[0];
			if ((Hapka_HM[6] & 3) != 3)
				{
					if (nom_obs_old != Hapka_HM[3])
						{
							kol_sond_old=kol_sond;
							kol_sond=0;
							//kol_sond_old=Hapka_HM[2]-kol_sond;
							//kol_sond=Hapka_HM[2];
							nom_obs_old= Hapka_HM[3];
							if (rejim == 1)
								kol_sond_old=(kol_sond_old + 1) >> 1;
						}
					else
						kol_sond++;
				}

			test_buf[23]= kol_sond_old;

			// ************************************************************************************
			// ПРОИЗОШЛО Успешное завершение обмена
			// ************************************************************************************

			// ************************************************************************************
			// Отработка принятого сообщения от платы на предмет ошибок
			// ************************************************************************************
			
			if (rejim == 4)  // поскольку указатели в режиме 4 отличаются от остальных
				{
					Hapka_HM=(int *)&bbb.Hapka_HM[0];
					if ((Hapka_HM[6] & 3) != 3)
					{
						max_time=bbb.HM_Re_Im_1[2046];
						w=(int *)&bbb.HM_Re_Im_1[2037];
						PorogL=bbb.HM_Re_Im_1[2037];
						ch=bbb.Hapka_HM[3];
						///////////////////////////////
						//if(bbb.HM_Re_Im_1[2037]==101)
						//      rejim=rejim;
						/////////////////////////////
						BSP_SET_ERROR_DATA(status2, 2, w[0]);
						w=(int *)&bbb.HM_Re_Im_1[2045];
						BSP_SET_ERROR_DATA(status2, 3, w[0]);
						T_obs=bbb.HM_Re_Im_1[2035] * 2 / dsp_param.f_proz;
						dsp_param.PorogL=bbb.HM_Re_Im_1[2037];
						dsp_param.con=bbb.HM_Re_Im_1[2038];
						dsp_param.con_m=bbb.HM_Re_Im_1[2039];
						nn=bbb.HM_Re_Im_1[2040];
						nom_sek=Hapka_HM[5] / delta_mai;
						komanda0=bbb.HM_Re_Im_1[2033];
						mes_test[3]=komanda0;
						test_buf[44]=bbb.HM_Re_Im_1[2023];//koef
						test_buf[45]=bbb.HM_Re_Im_1[2022];//dsp_param.meteo
					}
			
					if (nom_sek != nom_sek_old)
					{
						nom_sek_old=nom_sek;
					}
				}
			else
				Hapka_HM=(int *)&uks_Buf_HM->Hapka_HM[0];
			
			BSP_SET_ERROR_CODE(status2, Hapka_HM[0]);
			BSP_SET_DEBUGCOUNT(status2, Hapka_HM[1]);
			ttt++;

			if (ttt<40)
				{
					off++;
					if (off == (BUFFER_SIZE))
						off=0;
					BSP_SET_COUNT(status, off);
					off2++;
					if (off2 == (BUFFER_SIZE))
						off2=0;
					BSP_SET_COUNT(status2, off2);
					continue;
				}

			if (exit_err == 0)
				{
					exit_err=20;
					WritePrivateProfileString("Bsp", "work", "1", fname);
				}

			tek_sond=Hapka_HM[2];
			tek_mai= Hapka_HM[5];
			pr_per=nom_kom; // Предыдущая трансляция активного комплекта
			nom_kom=((Hapka_HM[0] & (1 << 10)) >> 10); // Трансляция текущего активного комплекта
			nom_kom ^=1;
			
			if (dsp_param.Nomer_komplekta != (nom_kom + 1)) // Смена трансляции
			{
				zwet=1;
				if (pr_per != nom_kom)
					{
						zwet=1;
						time(&tim);
						fprintf(flog, "\n%-60s%s", "Переход в активный режим", DateTimeToStr(tim).c_str());
						fflush(flog);
					}
				else
					{
						if (uytra == 0)
							{
								time(&tim);
								fprintf(flog, "\n%-60s%s", "Работа в активном режиме", DateTimeToStr(tim).c_str());
								fflush(flog);
								uytra=1;
								uytrn=0;
							}
					}

			}
			else
			{
				zwet=2;
				if (pr_per != nom_kom)
					{
						time(&tim);
						fprintf(flog, "\n%-60s%s", "Переход в неактивный режим", DateTimeToStr(tim).c_str());
						fflush(flog);
						zwet=2;
						uytrn=1; uytra=0;
					}
				else
					{
						if (uytrn == 0)
						{
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Работа в неактивном режиме", DateTimeToStr(tim).c_str());
							fflush(flog);
							uytrn=1;
							uytra=0;
						}
				}
			}

			//******************************************************************************************
			//    Обработка ошибок принятых в сообщении от платы и передача состояний в bsp
			//******************************************************************************************

			komanda0=0;
			//        BSP_SET_ERROR_DATA(status2,1,1);
			//                        komandos(39);
			if (Hapka_HM[1] != 0)
				{
					if ((Hapka_HM[1] & 3) && c_oh[0] == 0)
						{
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Потеря данных при передаче по линку", DateTimeToStr(tim).c_str());
							fprintf(flog, "\nProp1=%f Prop2=%f nT=%f  c_oh=100", bbb.HM_Re_Im_1[1686],
								bbb.HM_Re_Im_1[1687], bbb.HM_Re_Im_1[2024]);
							fflush(flog);
							c_oh[0]++;
							if (c_oh[0] == 100)
								c_oh[0]=0;
						}
					if ((Hapka_HM[1] & 4) && c_oh[1] == 0)
						{
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Периоды повторения в файле не соответсвуют реальным", DateTimeToStr(tim).c_str());
							fprintf(flog, "\nЗначение периода, вызвавшего ошибку=%f", bbb.HM_Re_Im_1[1688]);
							fflush(flog);
							c_oh[1]=1;
							komanda=40;
							BSP_SET_ERROR_DATA(status2, 1, 1);
							if (komanda != 0)
								{
									kod_vosvrata=komandos(komanda - 1);
									komanda=0;
									BSP_SET_COMMAND(status2, komanda);
								}
						}
					if ((Hapka_HM[1] & 16))
						{
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Ошибки при передаче по линку со 2 в 1 процессор", DateTimeToStr(tim).c_str());
							if (rejim == 4)
								fprintf(flog, "\n Код ошибки=%d", bbb.HM_Re_Im_1[1688]);
							else
								fprintf(flog, "\n Код ошибки=%d", uks_Buf_HM->dan.bin.HM_Re_Im_1[1688]);
							mes_test[1]=1;
							komandos(39);
							fflush(flog);
						}

					if ((Hapka_HM[1] & 256))
						{
							time(&tim);
							fprintf(flog, "\n%-60s%s", "NAN при выполнении MTD", DateTimeToStr(tim).c_str());
							fprintf(flog, "\n Индекс=%d", bbb.HM_Re_Im_1[1688]);
							fflush(flog);
							mes_test[1]=1;
							komandos(39);
						}

					if (Oh_dan == 1)
						{
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Ошибка в данных (NAN)", DateTimeToStr(tim).c_str());
							fflush(flog);
							Oh_dan=0;
							mes_test[1]=1;
							komandos(39);
						}
					if ((Hapka_HM[1] & 128))
						{
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Время выполнения сжатия велико", DateTimeToStr(tim).c_str());
							fflush(flog);
							mes_test[1]=1;
							komandos(39);
						}
					if ((Hapka_HM[1] & 32))
						{
							nn=bbb.HM_Re_Im_1[1688];
							if (c_oh[2] == 0)
								{
									time(&tim);
									fprintf(flog, "\n%-60s%s", "Отсутсвие импульсов запуска", DateTimeToStr(tim).c_str());
									fflush(flog);
									BSP_SET_STATUS(status, cmdWORK);
									c_oh[2]++;
									mes_test[1]=1;
									komandos(39);
								}
							else
								BSP_SET_STATUS(status, cmdWORK);
							c_oh[2]++;
						}
					if (/*Hapka_HM[0] &&*/ Hapka_HM[1] & 64)
						{
								nn=bbb.HM_Re_Im_1[1688];
								time(&tim);
								fprintf(flog, "\n%-60s%s", "Данные не 1 канала. Внутренняя перезагрузка", DateTimeToStr(tim).c_str());
								fprintf(flog, "\nНомер канала=%f", bbb.HM_Re_Im_1[1689]);
								fflush(flog);
								c_oh[4]++;

								// TODO  BRD_reset(handle, 0);
								// TODO  BRD_reset(handle, 1);
								// TODO  err=BRD_cleanup();
								goto nach_lo;
						}
					if (Hapka_HM[1] & 32768)
						{
							if (c_oh[3]<3)
							{
								fprintf(flog, "\n%-60s%s", "Нет связи со 2 процессором", DateTimeToStr(tim).c_str());
								mes_test[1]=1;
								komandos(39);
								BSP_SET_STATUS(status, cmdWORK);
								goto nach_lo;
							}
							else
							{
								mes_test[1]=1;
								komandos(39);
								{
									time(&tim);
									fprintf(flog, "\n%-60s%s", "Нет связи со 2 процессором. Перезагрузка ОС", DateTimeToStr(tim).c_str());
									ShtdnRemSystem(NULL, false, true);
								}
							}
							c_oh[3]++;
						}
					if (Hapka_HM[1] & 1 << 10 && c_oh[9] == 0)
						{
							fprintf(flog, "\n%-60s%s", "Переполнение буфера", DateTimeToStr(tim).c_str());
							c_oh[9]=1;

						}
					if (exit_err == 0 && c_oh[0] == 0 && c_oh[1] == 0 && c_oh[2] == 0 && c_oh[3] == 0 && c_oh[4] == 0)
						{
							exit_err=20;
							WritePrivateProfileString("Bsp", "work", "1", fname);
						}
				}
			else
				{
					if (c_oh[1])
						{
							c_oh[1]=0;
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Ошибка периодов ликвидирована", DateTimeToStr(tim).c_str());
							fflush(flog);
						}
					if (c_oh[2])
						{
							c_oh[2]=0;
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Появились импульсы запуска", DateTimeToStr(tim).c_str());
							fflush(flog);
							mes_test[1]=0;
							komandos(39);
						}
					if (c_oh[3])
						{
							c_oh[3]=0;
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Появилась связь со 2 процессором", DateTimeToStr(tim).c_str());
							fflush(flog);
						}
					if (c_oh[4])
						{
							c_oh[4]=0;
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Данные  1 канала", DateTimeToStr(tim).c_str());
							fflush(flog);
						}
				}
				
				kod_oh=BSP_GET_ERROR_CODE(status); //kod_oh=0;
				
				if (kod_oh>1 && kod_rpu == 0)
					{
						if (GetPrivateProfileInt("System", "NoConrad", 0, rpu))
						{
							kod_rpu=1;
						}
						else
						{
							if (kod_kk == 100)
							{
								soob="";
								if (kod_oh & 1) soob += "Не передается СЕВЕР\n";
								if (kod_oh & 2) soob += "Не передаются МАИ\n";
								if (kod_oh & 4) soob += "Не передаются ЗАПУСКИ\n";
								if (kod_oh & 8) soob += "Не передается ЗАПРОСНЫЙ КОД";

								time(&tim);
								sprintf(ss, "Ошибка kikoss. kod=%d.", kod_oh);
								fprintf(flog, "\n%-60s%s", ss, DateTimeToStr(tim).c_str());
								fprintf(flog, "\n%s", soob.c_str());
								fflush(flog);
								mes_test[1]=1;
								komandos(39);
								Hapka_HM[1]|=1 << 30;
								kod_kk=1;
							}
							Hapka_HM[1]|=1 << 30;
							kod_kk++;
						}
					}
				else
					{
						if (kod_kk != 0)
						{
							time(&tim);
							fprintf(flog, "\n%-60s%s", "Ошибка kikoss ликвидирована.", DateTimeToStr(tim).c_str());
							fflush(flog);
							kod_kk=0;
							mes_test[1]=0;
							komandos(39);
							Hapka_HM[1]&=~(1 << 30);
						}

					}
				// ************************************************************************************
				// Конец от отработки принятого сообщения от платы на предмет ошибок
				// ************************************************************************************
				static int utm=0;
				if (rejim == 4) // для режима сжатие+мтд свой вариант
					{
						if (Hapka_HM[6] == 3)  /******* вроде как если запись с оценками  *******/
							{
								kolotm+=Hapka_HM[7];
								if (Hapka_HM[7]>0 && Hapka_HM[3]>dsp_param.nak)
								{
									if (Hapka_HM[1] & 8)
										{
											utm++;
											if (utm<10)
												{
													time(&tim);

													fprintf(flog, "\n%-60s Очень много оценок=%d  %s", "Пропуск оценок",
														Hapka_HM[7], DateTimeToStr(tim).c_str());
													fflush(flog);
												}
	
										}
									else
										utm=0;
									///////////////////////////////////////////////////////
									uks_QQQ->Hapka[2]=3;
									uks_QQQ->Hapka[3]=Hapka_HM[7];
									uks_QQQ->Hapka[0]=Hapka_HM[4];
									uks_QQQ->Hapka[2]&=(~0x300);
									n_kom= (Hapka_HM[0] & 0xc00) >> 10;
								
									uks_QQQ->Hapka[2]|=n_kom << 8;//(~0x300);
									if (uks_QQQ->Hapka[3]>338)
										uks_QQQ->Hapka[3]=338;
									for (int i=0; i<uks_QQQ->Hapka[3]; i++)
										{

											uks_QQQ->Buf[5 * i]=2.0;
											for (int j=0; j<4; j++)
												uks_QQQ->Buf[5 * i + j + 1]=bbb.HM_Re_Im_1[4 * i + j];
											if (uks_QQQ->Buf[5 * i + 2] <= dsp_param.Daln_MONO)
												uks_QQQ->Buf[5 * i]=0;
											if (Oz_file == 1)  // включен режим отладки то вывод инфы
												fprintf(foz, "\n%d  Dal=%.0f As=%.02f Amp=%.0f, Time=%f",
													uks_QQQ->Hapka[0], uks_QQQ->Buf[5 * i + 2], uks_QQQ->Buf[5 * i + 3], uks_QQQ->Buf[5 * i + 1], uks_QQQ->Buf[5 * i + 4]);
										}
								
									if (sap_file == 1) // если включен вывод в файл
										fwrite(uks_QQQ, sizeof(QQQ), 1, fp);
									
									if (sap_ozenki == 1) // если включен вывод в файл оценок, то их запись
										{	
											// TODO dtm записывается в старом формате времени, необходимо преобразование
											dtm=(double*)&uks_QQQ[0];
											time(&tim);
											*dtm = tim; // В шапке передается текущее время (не то которое ожидается)
											temp_rez=5 * uks_QQQ->Hapka[3] + 4;
											fwrite(uks_QQQ, temp_rez, 4, fpq);
										}
									/*  подключение фильтра сопровождения */

									if (Wkl_Sop)
										{ 
											if (MySoprov_otm((int*)uks_QQQ, ch))
												{
													;
												}
											if (Oz_file == 1)  // включен режим отладки то вывод инфы
												for (int i=0; i<uks_QQQ->Hapka[3]; i++)
													fprintf(foz, "\nПосле sop %d  Dal=%.0f As=%.02f Amp=%.0f, V=%f",
														uks_QQQ->Hapka[0], uks_QQQ->Buf[5 * i + 2], uks_QQQ->Buf[5 * i + 3], uks_QQQ->Buf[5 * i + 1], uks_QQQ->Buf[5 * i + 4] * 3.6 / T_obs);
								
										}
								
									kol_otm_obsor+= uks_QQQ->Hapka[3];
								
									// корректировка указателей
									off++;
									if (off == BUFFER_SIZE)
										off=0;
									BSP_SET_COUNT(status, off);
									continue;
								}
							
							}// от Hapka_HM==3  /*********************  конец от обработки оценок  *********************************/
						else // перед нами не оценки, а например видео - результат сжатия +МТД (а что еще может быть в режиме 4)
							{
								if (obsorold != Hapka_HM[3])      // Закончился текущий обзор
									{
										if (Oz_file == 1)
											fprintf(foz, "\n*******************************************  Обзор № %d  ****************************",
												Hapka_HM[3]);
									}

								obsorold=Hapka_HM[3];
								if (nach == 0)
									{
										nach=1;
									}
								else
									{
										k = Hapka_HM[6] & 0xc;
										uks_QQQ->Hapka[0]=Hapka_HM[4 + 0] * konst_time * 10;//pr_fifo;//250;
										uks_QQQ->Hapka[1]=Hapka_HM[4 + 1];
										uks_QQQ->Hapka[2]=Hapka_HM[4 + 2] & 3;
										n_kom= (Hapka_HM[0] & 0xc00) >> 10;
										if (sap_file == 1) k_usil=1;
										uks_QQQ->Hapka[2]|=n_kom << 8;
										uks_QQQ->Hapka[3]=MIN(1685, Hapka_HM[4 + 3]);
										for (int i=0; i<uks_QQQ->Hapka[3]; i++)  /* УСИЛЕНИЕ СИГНАЛА ДЛЯ ОТОБРАЖЕНИЯ НА МОНИТОРЕ В КИКО */
											{
												try
													{
														uks_QQQ->Buf[i]=k_usil*bbb.HM_Re_Im_1[i];
													}
												catch (...)
													{
														Oh_dan=1;
													}
											}
										uks_QQQ->Buf[1]= k_usil;

										/* ******************** ЗАПИСЬ в видео мтд  *************************************/
										/* ********************************************************************************/
										if (sap_file == 1) // если запись в файл - добавил я
										{
											fwrite(uks_QQQ, sizeof(QQQ), 1, fp);
											K_K++;
											err=K_K*objem_sap_QQQ;
											BSP_SET_ERROR_TIME(status2, err);
											if (K_K*objem_sap_QQQ >= Objem)
												{
													K_K=0;
													sap_file=0;
													sap_file1=1;
													fclose(fp);
													fp=NULL;
													WritePrivateProfileString("Запись в файл", "Запись", "0", fini);
												}//sap_file==1
										}

										/* ******************** ЗАВЕРШЕНА ЗАПИСЬ в видео мтд  *************************************/
										/* *********************************************************************************/

										// корректировка указателей
										off++;
										if (off == BUFFER_SIZE)
											off=0;
										BSP_SET_COUNT(status, off);
										////////////////////////////////////

										if (n_obs != Hapka_HM[3])
										{
											n_obs=Hapka_HM[3];
											for (int i=0; i<19; i++)
												test_buf[i]= bbb.HM_Re_Im_1[2029 + i];
											test_buf[4]=kol_otm_obsor;//rew;
											test_buf[16]=kolotm;//rew1;
											kol_otm_obsor=0;
											kolotm=0;
											for (int i=0; i<4; i++)
												test_buf[i + 19]= bbb.HM_Re_Im_1[2025 + i];
										}
										////////////////////////////////////
									} //else rejim==4
							}//else от обработок ne ozenki
						continue;
					}
				// ************************************************************************************
				// Перемещение указателей и запись в файл
				// ************************************************************************************
				// ************************************************************************************
				// для остальных режимов СЫРОЕ видео+сжатие свой вариант
				// ************************************************************************************
				else // если rejim!=4    - запись в файл исходного сигнала, сжатого
					{
						static int kmo=0;
						
						if (rejim == 1)
						{

							int *w1, *w2;
							//FormData.flag_work_PK=2;   // норма
							BSP_SET_STATUS(status, cmdWORK);
							if (Hapka_HM[0] & (1 << 3)) // Включены нижние углы
							{
								if ((uks_Buf_HM->Hapka_HM[6] & 0xf) == 6)//||
								{
									wwiwod();
									off++;
									if (off == (BUFFER_SIZE))
										off=0;
									BSP_SET_COUNT(status, off);
									off2++;
									if (off2 == (BUFFER_SIZE))
										off2=0;
									BSP_SET_COUNT(status2, off2);
									tyu++;
									continue;
								}
								else
								{
									off2++;
									if (off2 == (BUFFER_SIZE))
										off2=0;
									BSP_SET_COUNT(status2, off2);
									continue;
								}
							}
							else
								if (Hapka_HM[0] & (1 << 4))// &&  (uks_Buf_HM->Hapka_HM[6]&0xf)==14) // Включены верхние углы
								{
									if ((uks_Buf_HM->Hapka_HM[6] & 0xf) == 14)
									{
										wwiwod();
										off++;
										if (off == (BUFFER_SIZE))
											off=0;
										BSP_SET_COUNT(status, off);
										off2++;
										if (off2 == (BUFFER_SIZE))
											off2=0;
										BSP_SET_COUNT(status2, off2);
										tyu++;
										continue;
									}
									else
									{
										off2++;
										if (off2 == (BUFFER_SIZE))
											off2=0;
										BSP_SET_COUNT(status2, off2);
										continue;
									}
								}

						} 	//if(rejim==0)
					else   if (rejim == 0) //if((Hapka_HM[0]&1)==0)
							{
								int *w1, *w2;
								BSP_SET_STATUS(status, cmdWORK);
							
								if (sap_file == 1) k_usil=1;
								if (kmo<3)
									{
										uks_QQQ->Hapka[0]=uks_Buf_HM->Hapka_HM[4 + 0] * konst_time;//250;
										uks_QQQ->Hapka[1]=uks_Buf_HM->Hapka_HM[4 + 1];
										uks_QQQ->Hapka[2]=uks_Buf_HM->Hapka_HM[4 + 2] & 3;
										uks_QQQ->Hapka[3]=MIN(1690, uks_Buf_HM->Hapka_HM[4 + 3]);

										rbb.dsp.hm.Hapka[0]=uks_QQQ->Hapka[0];
										rbb.dsp.hm.Hapka[1]=uks_QQQ->Hapka[1];
										rbb.dsp.hm.Hapka[2]=uks_QQQ->Hapka[2];
										rbb.dsp.hm.Hapka[3]=uks_QQQ->Hapka[3];
										if (dsp_param.WKL_HM == 1)
										{
											for (int i=0; i<uks_QQQ->Hapka[3]; i++)
											{
												if (Hapka_HM[0] & (1 << 3))
												{
													asd.x=uks_Buf_HM->dan.bin.HM_Re_Im_1[2 * i];
													CV=complex<float>(asd.y[0], asd.y[1]);
													uks_QQQ->Buf[i]=k_usil*norm(CV);
													rbb.dsp.hm.Buf[i]=asd.x;
												}
												else if (Hapka_HM[0] & (1 << 4))
												{
													asd.x=uks_Buf_HM->dan.bin.HM_Re_Im_1[2 * i + 1];
													CV=complex<float>(asd.y[0], asd.y[1]);
													uks_QQQ->Buf[i]=k_usil*norm(CV);
													rbb.dsp.hm.Buf[i]=asd.x;
												}
											}
											if (sap_file == 1)
											{
												fwrite(&rbb, sizeof(rbb), 1, fp);
												K_K++;
												err=K_K*objem_sap_QQQ;
												BSP_SET_ERROR_TIME(status2, err);
												if (K_K*objem_sap_QQQ >= Objem)
												{
													K_K=0;
													sap_file=0;
													sap_file1=1;
													fclose(fp);
													fp=NULL;
													WritePrivateProfileString("Запись в файл", "Запись", "0", fini);
												}
											}
										}
										if (Hapka_HM[0] & (1 << 7))
										{
											rbbM.dsp.mono[kmo].Hapka[0]=uks_Buf_HM->dan.bin.Hapka_MONO[0] * konst_time;//250; ;
											rbbM.dsp.mono[kmo].Hapka[1]=uks_Buf_HM->dan.bin.Hapka_MONO[1];
											rbbM.dsp.mono[kmo].Hapka[2]=0;
											rbbM.dsp.mono[kmo].Hapka[3]=MIN(500, uks_Buf_HM->dan.bin.Hapka_MONO[3]);// 500;
											rrr.dsp.mono[kmo].Hapka[0]=uks_Buf_HM->Hapka_HM[4 + 0] * konst_time;//250;
											rrr.dsp.mono[kmo].Hapka[1]=uks_Buf_HM->Hapka_HM[4 + 1];
											rrr.dsp.mono[kmo].Hapka[2]=0;
											rrr.dsp.mono[kmo].Hapka[3]=MIN(500, uks_Buf_HM->dan.bin.Hapka_MONO[3]);
											for (int i=0; i<rbbM.dsp.mono[kmo].Hapka[3]; i++)
											{
												if (Hapka_HM[0] & (1 << 3))
												{
													asd.x=uks_Buf_HM->dan.bin.MONO_Re_Im_1[2 * i];
													CV=complex<float>(asd.y[0], asd.y[1]);
													rbbM.dsp.mono[kmo].Buf[i]=asd.x;
													rrr.dsp.mono[kmo].Buf[i]=k_usil*norm(CV);
													rrr.dsp.mono[kmo].Buf[1]=k_usil;
												}
												else if (Hapka_HM[0] & (1 << 4))//if(dsp_param.VU==1)
												{
													asd.x=uks_Buf_HM->dan.bin.MONO_Re_Im_1[2 * i + 1];
													CV=complex<float>(asd.y[0], asd.y[1]);
													rbbM.dsp.mono[kmo].Buf[i]=asd.x;
													rrr.dsp.mono[kmo].Buf[i]=k_usil*norm(CV);
													rrr.dsp.mono[kmo].Buf[1]=k_usil;
												}
											}
										}
										uks_QQQ->Buf[1]=k_usil;
										kmo++;
									} // if(kmo < 3)
								else
									{
										kmo=0;

										w1=(int*)uks_QQQ;
										w2=(int*)&rrr;
										for (int i=0; i<1695; i++)
											{
												w1[i]=w2[i];
												k=k;
											}

										if (sap_file == 1)
											{
												fwrite(&rbbM, sizeof(rbb), 1, fp);
												K_K++;
												err=K_K*objem_sap_QQQ;
												BSP_SET_ERROR_TIME(status2, err);
												if (K_K*objem_sap_QQQ >= Objem)
													{
														K_K=0;
														sap_file=0;
														sap_file1=1;
														fclose(fp);
														fp=NULL;
														WritePrivateProfileString("Запись в файл", "Запись", "0", fini);
													}
											}
									}

							}

							off++;
							if (off == (BUFFER_SIZE))
								off=0;
							BSP_SET_COUNT(status, off);
							off2++;
							if (off2 == (BUFFER_SIZE))
								off2=0;
							BSP_SET_COUNT(status2, off2);
							tyu++;
						}
			} // бесконечный for(;;)
konez:
			//  TODO CloseHandle(hCardExit);
			return 0;
	} 

void openLogFile(void)
{
	//**********************  Создание log-файла ***********************
	// получаем текущее число месяца
	time_t time_m;
	struct tm *timeinfo;
	time(&time_m);
	timeinfo = localtime(&time_m);
	printf("\nDay - %d", timeinfo->tm_mday);

	// проверяем есть ли такой каталог, если нет то создаем
	string cat_log_exist;
	char ctletmp[20];
	sprintf(ctletmp, "%02d", timeinfo->tm_mday);
	cat_log_exist = ProgrammPath + "\\" + std::string(ctletmp);
	DWORD catLogAttrs;
	catLogAttrs = GetFileAttributesA(cat_log_exist.c_str());
	printf(cat_log_exist.c_str());

	if (catLogAttrs == INVALID_FILE_ATTRIBUTES)  // директории не существует
	{
		// создаем директорию
		printf(cat_log_exist.c_str());
		CreateDirectoryA(cat_log_exist.c_str(), NULL);
		//MessageBoxMC("Директории не существует!");
	}
	else
	{
		if (catLogAttrs & FILE_ATTRIBUTE_DIRECTORY)
			//MessageBoxMC("Директория существует!");
			;
	}

	WIN32_FILE_ATTRIBUTE_DATA wfad;
	string file_log_exist;
	DWORD fileLogAttrs;
	file_log_exist = cat_log_exist + "\\mtd.log";
	fileLogAttrs = GetFileAttributesA(file_log_exist.c_str());
	if (fileLogAttrs == INVALID_FILE_ATTRIBUTES)  // файла не существует, создаем
	{
		//MessageBoxMC("Файла mtd.log не существует! Создаем...");
		//CreateFileA(file_log_exist.c_str(), (GENERIC_READ | GENERIC_WRITE), FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		flog = fopen(file_log_exist.c_str(), "a");
		if (flog == NULL)
			//MessageBoxMC("Не удалось открыть файл log.txt.");
			;
	}
	else
	{
		//MessageBoxMC("Файл mtd.log существует! Проверяем дату...");
		if (!GetFileAttributesEx(file_log_exist.c_str(), GetFileExInfoStandard, &wfad))
		{
			//MessageBoxMC("Невозможно считать атрибуты файла mtd.log.");
			DeleteFileA(file_log_exist.c_str());
			flog = fopen(file_log_exist.c_str(), "a");
			if (flog == NULL)
				//MessageBoxMC("Не удалось открыть файл log.txt.");
				;
		}
		else
		{
			//MessageBoxMC("Атрибуты файла mtd.log считаны.");
			FILETIME ftLocal;
			SYSTEMTIME st;
			FileTimeToLocalFileTime(&(wfad.ftLastWriteTime), &ftLocal);
			FileTimeToSystemTime(&ftLocal, &st);
			//printf("\ntm_mday = %d, st.wDay = %d, tm_mon = %d, st.wMonth = %d\n", timeinfo->tm_mday, st.wDay, timeinfo->tm_mon, st.wMonth);
			if ((timeinfo->tm_mday != st.wDay) && (timeinfo->tm_mon != st.wMonth))
				DeleteFileA(file_log_exist.c_str());
			flog = fopen(file_log_exist.c_str(), "a");
			if (flog == NULL)
				MessageBoxMC("Не удалось открыть файл log.txt.");
		}
	}
}

void deleteOzFile(void)
  {
	time_t time_m;
	struct tm *timeinfo;
	time(&time_m);
	timeinfo = localtime(&time_m);
	string file_oz_exist;
	DWORD fileOzAttrs;
	string cat_log_exist;
	WIN32_FILE_ATTRIBUTE_DATA wfad;
	char ctletmp[20];
	sprintf(ctletmp, "%02d", timeinfo->tm_mday);
	cat_log_exist = ProgrammPath + "\\" + std::string(ctletmp);
	sprintf(ctletmp, "\\ozenki_%02d.oz", timeinfo->tm_hour);
	file_oz_exist = cat_log_exist + string(ctletmp);
	fileOzAttrs = GetFileAttributesA(file_oz_exist.c_str());
	if(fileOzAttrs != INVALID_FILE_ATTRIBUTES)
	  {
		if (!GetFileAttributesEx(file_oz_exist.c_str(), GetFileExInfoStandard, &wfad))
		{
			MessageBoxMC("Невозможно считать атрибуты файла ozenki_.oz.");
			DeleteFileA(file_oz_exist.c_str());
		}
		else
		{
			MessageBoxMC("Атрибуты файла ozenki_.oz считаны.");
			FILETIME ftLocal;
			SYSTEMTIME st;
			FileTimeToLocalFileTime(&(wfad.ftLastWriteTime), &ftLocal);
			FileTimeToSystemTime(&ftLocal, &st);
			//printf("\ntm_mday = %d, st.wDay = %d, tm_mon = %d, st.wMonth = %d\n", timeinfo->tm_mday, st.wDay, timeinfo->tm_mon, st.wMonth);
			if ((timeinfo->tm_mday != st.wDay) && (timeinfo->tm_mon != st.wMonth))
				DeleteFileA(file_oz_exist.c_str());
		}
	  }
	else 
	  MessageBoxMC("Файла ozenki_.oz не существует.");

}

void Sap_File_Oz(void)
  {
	time_t tim;
	struct tm *timeinfo;
	string filepath;
	char ss[255];
	// Запись в файл оценок
	time(&tim);
	timeinfo = localtime(&tim);
	sprintf(ss, "\\%02d\\ozenki_%02d.oz", timeinfo->tm_mday, timeinfo->tm_hour);
	filepath = ProgrammPath + string(ss);
	fpq = fopen(filepath.c_str(), "ab");
  }

void Sap_File_Karta(void)
  {
	time_t tim;
	struct tm *timeinfo;
	string filepath;
	char ss[256];
	// Запись в файл оценок
	time(&tim);
	timeinfo = localtime(&tim);
	sprintf(ss, "%s%02d\\karta_%02d.txt", timeinfo->tm_mday, timeinfo->tm_hour);
	filepath = ProgrammPath + string(ss);
	fpq = fopen(filepath.c_str(), "a");
  }

DWORD WINAPI
ThreadCardExit(void)
{
	DWORD r;

	hCardExit = CreateEvent(NULL, FALSE, FALSE, "BspExitEvent");

	for (;; ) {
		r = WaitForSingleObject(hCardExit, INFINITE);
		if (WAIT_OBJECT_0 == r) {
			Exit = 1;
			WritePrivateProfileString("Bsp", "Exit", "1", fname);
			break;
		}
	}

	CloseHandle(hCardExit);
	hCardExit = NULL;
	return 0;
}

void wwiwod(void)
{
	static int km=0;
	complex<float> CV;
	int *w1, *w2;
	int err;
	if (sap_file == 1) k_usil=1;
	if (km<3)
		{
			uks_QQQ->Hapka[0]=uks_Buf_HM->Hapka_HM[4 + 0] * konst_time;//250;
			uks_QQQ->Hapka[1]=uks_Buf_HM->Hapka_HM[4 + 1];
			uks_QQQ->Hapka[2]=uks_Buf_HM->Hapka_HM[4 + 2] & 3;
			uks_QQQ->Hapka[3]=MIN(1690, uks_Buf_HM->Hapka_HM[4 + 3]);

			for (int i=0; i<uks_QQQ->Hapka[3]; i++)
			{
				CV=complex<float>(uks_Buf_HM->dan.dsp.HM_Re_Im_1[2 * i], uks_Buf_HM->dan.dsp.HM_Re_Im_1[2 * i + 1]);
				uks_QQQ->Buf[i]=k_usil*norm(CV);
			}
			uks_QQQ->Buf[1]=k_usil;

			if (sap_file == 1) // если запись в файл - добавил я
			{
				fwrite(uks_QQQ, sizeof(QQQ), 1, fp);
				K_K++;
				err=K_K*objem_sap_QQQ;
				BSP_SET_ERROR_TIME(status2, err);
				if (K_K*objem_sap_QQQ >= Objem)
				{
					K_K=0;
					sap_file=0;
					sap_file1=1;
					fclose(fp);
					fp=NULL;
					WritePrivateProfileString("Запись в файл", "Запись", "0", fini);
				}//sap_file==1
			}
			rrr.dsp.mono[km].Hapka[0]=uks_Buf_HM->Hapka_HM[4 + 0] * konst_time;//250;
			rrr.dsp.mono[km].Hapka[1]=uks_Buf_HM->Hapka_HM[4 + 1];
			rrr.dsp.mono[km].Hapka[2]=0;
			rrr.dsp.mono[km].Hapka[3]=MIN(500, uks_Buf_HM->dan.bin.Hapka_MONO[3]);

			for (int i=0; i<rrr.dsp.mono[km].Hapka[3]; i++)
				{
					CV= complex<float>(uks_Buf_HM->dan.dsp.MONO_Re_Im_1[2 * i], uks_Buf_HM->dan.dsp.MONO_Re_Im_1[2 * i + 1]);
					rrr.dsp.mono[km].Buf[i]= k_usil*norm(CV);
				}
			rrr.dsp.mono[0].Buf[1]= k_usil;
			km++;
		}
	else
		{
			w1=(int*)uks_QQQ;
			w2=(int*)&rrr;
			for (int i=0; i<1695; i++)
				{
					w1[i]=w2[i];
					k=k;
				}
			km=0;
			if (sap_file == 1) // если запись в файл - добавил я
				{
					fwrite(uks_QQQ, sizeof(QQQ), 1, fp);
					K_K++;
					err=K_K*objem_sap_QQQ;
					BSP_SET_ERROR_TIME(status2, err);
					if (K_K*objem_sap_QQQ >= Objem)
						{
							K_K=0;
							sap_file=0;
							sap_file1=1;
							fclose(fp);
							fp=NULL;
							WritePrivateProfileString("Запись в файл", "Запись", "0", fini);
						}//sap_file==1
				}
		}
}

int ShtdnRemSystem(char *name, bool force_opt,
	bool reboot_opt)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tkp;

	// char *msg="";
	// DWORD time=0;

	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
		&hToken);

	if (!LookupPrivilegeValue(name, SE_SHUTDOWN_NAME,
		&tkp.Privileges[0].Luid))
	{
		MessageBoxMC("Пипец в LookupPrivilegeValue");
		return 1;
	}

	tkp.PrivilegeCount =1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0);

	if (!LookupPrivilegeValue(name,
		SE_REMOTE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid))
	{
		MessageBoxMC("Пипец с SE_REMOTE_SHUTDOWN_NAME");
		return 2;
	}
	tkp.PrivilegeCount =1;
	tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	AdjustTokenPrivileges(hToken, FALSE, &tkp,
		0, (PTOKEN_PRIVILEGES)NULL, 0);

	if (InitiateSystemShutdown(name, //если NULL - то выключение локальной машины
		NULL/*или msg*/, 0/*или time*/,
		force_opt, reboot_opt))
		MessageBoxMC("Система погашена...");
	else
	{
		MessageBoxMC("Облом...");
		return 3;
	}

	return 0;
}