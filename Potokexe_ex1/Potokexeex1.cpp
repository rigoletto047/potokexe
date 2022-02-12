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

// Локальные модули и заголовочные файлы
#include "./inc/Bsp_common.h"
#include "./inc/dsp_param.h"
#include "./inc/sundries.h"
#include "./inc/buf.h"
#include "./inc/var.h"

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
int off = 0;
int chet_time = -1000;
void wwiwod(void);
int start; // запуск с FOrm4
QQQ *uks_QQQ, *uks_QQQ1;
Buf_HM_MONO* buffer = NULL;
QQQ * buffer2 = NULL;
Buf_HM_MONO *uks_Buf_HM = buffer;
int size_buf;
unsigned int Daln_HM, Daln_MONO, sizeHM, sizeMONO;
int sap_ozenki = 0;
static int kol_povt = 3;
int d_count = 0;
TIP_4 bbb;
float *test_buf;

// Указатели на файлы отображаемые в память
HANDLE hFileMapStatus = NULL, hFileMapStatus2 = NULL, hFileMapBuffer = NULL, hFileMapBuffer2 = NULL;
LPVOID lpViewStatus   = NULL, lpViewStatus2   = NULL;
LPVOID lpViewBuffer   = NULL, lpViewBuffer2   = NULL;

// Внешние переменные
extern PARAM_DSP dsp_param, dsp_param_1;
extern int Exit = 0;

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

		// !!! Безсмысленно, т.к. будет доступен только после открытия файла в памяти 
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

			} // бесконечный for(;;)
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