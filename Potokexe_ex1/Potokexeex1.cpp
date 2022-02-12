// Potokexeex1.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"

#define _CRT_SECURE_NO_WARNINGS


#include <windows.h>
#include <winbase.h>
#include <filesystem>
#include <ctime>
#include <string>

// Локальные модули и заголовочные файлы
#include "./inc/Bsp_common.h"
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
volatile PBsp_Status	status = NULL, status2 = NULL;
int kod_rpu = 0;
int komanda0 = 0, komanda;
int off = 0;

// Указатели на файлы
FILE *fp, *flog, *foz, *fp_oz;
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
	//if (status != NULL)
	//{
	//	BSP_SET_STATUS(status, cmdWORK);
	//}

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

	// !!! Безсмысленно, т.к. будет доступен только после открытия файла в памяти
	// if (status2 != NULL)
	//   {
	// 	BSP_SET_COMMAND(status2, komanda);
	// 	BSP_SET_STATUS(status2, komanda);
	//   }
	
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
	temp_rez = 0;//init_plata();
	temp_rez = init_plata();


	return 0;
}


void openLogFile(void)
  {
	//**********************  Создание log-файла ***********************
	// получаем текущее число месяца
	time_t time_m;
	struct tm *timeinfo;
	time(&time_m);
	timeinfo  = localtime(&time_m);
	printf("\nDay - %d", timeinfo->tm_mday);
	
	// проверяем есть ли такой каталог, если нет то создаем
	string cat_log_exist;
	char ctletmp[20];
	sprintf(ctletmp, "%02d", timeinfo->tm_mday);
	cat_log_exist = ProgrammPath + "\\" + std::string(ctletmp);
	DWORD catLogAttrs;
	catLogAttrs = GetFileAttributesA(cat_log_exist.c_str());
	printf(cat_log_exist.c_str());
	
	if(catLogAttrs == INVALID_FILE_ATTRIBUTES)  // директории не существует
	  {
		// создаем директорию
		printf(cat_log_exist.c_str());
		CreateDirectoryA(cat_log_exist.c_str(), NULL);
		MessageBoxMC("Директории не существует!");
	  }
	else
	  {
		if (catLogAttrs & FILE_ATTRIBUTE_DIRECTORY)
			MessageBoxMC("Директория существует!");
	  }

	WIN32_FILE_ATTRIBUTE_DATA wfad;
	string file_log_exist;
	DWORD fileLogAttrs;
	file_log_exist = cat_log_exist + "\\mtd.log";
	fileLogAttrs = GetFileAttributesA(file_log_exist.c_str());
	if (fileLogAttrs == INVALID_FILE_ATTRIBUTES)  // файла не существует, создаем
	{
		MessageBoxMC("Файла mtd.log не существует! Создаем...");
		//CreateFileA(file_log_exist.c_str(), (GENERIC_READ | GENERIC_WRITE), FILE_SHARE_WRITE, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
		flog = fopen(file_log_exist.c_str(), "a");
		if (flog == NULL)
			MessageBoxMC("Не удалось открыть файл log.txt.");
	}
	else
	{
		MessageBoxMC("Файл mtd.log существует! Проверяем дату...");
		if (!GetFileAttributesEx(file_log_exist.c_str(), GetFileExInfoStandard, &wfad))
		{
			MessageBoxMC("Невозможно считать атрибуты файла mtd.log.");
			DeleteFileA(file_log_exist.c_str());
			flog = fopen(file_log_exist.c_str(), "a");
			if (flog == NULL)
				MessageBoxMC("Не удалось открыть файл log.txt.");
		}
		else
		{
			MessageBoxMC("Атрибуты файла mtd.log считаны.");
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

	string file_oz_exist;
	DWORD fileOzAttrs;
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

