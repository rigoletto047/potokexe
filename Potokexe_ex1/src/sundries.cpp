#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winbase.h>
#include <string>
#include <time.h>


#ifdef WIN32
#define _CRT_SECURE_NO_WARNINGS
void MessageBoxMC(char * message) 
{
	size_t uSize = strlen(message) + 1;
	wchar_t* wcsBuffer = new wchar_t[uSize];
	mbstowcs(wcsBuffer, message, uSize);
	MultiByteToWideChar(CP_UTF8, 0, message, -1, wcsBuffer, uSize << 1);
	MessageBoxW(NULL, wcsBuffer, L"Error", MB_OK);
}

std::string GetCurrentDirectoryMC(void) 
{
	char PP[255];
	GetCurrentDirectoryA(255, PP);
	return std::string(PP);
	//char tmp[255];
	//GetModuleFileName(NULL, tmp, MAX_PATH);
	// получение каталога без имени файла
	//std::wstring::size_type pos = std::wstring(tmp).find_last_of(L"\\/");
	//std::wstring(tmp).substr(0, pos);
	//ProgrammPath = string(tmp);

}

std::string DateTimeToStr(time_t tc) 
  {
	struct tm *timeInfo;
	char t_str[30];
	timeInfo = localtime(&tc);
	sprintf(t_str, "%02d.%02d.%04d %02d:%02d:%02d", timeInfo->tm_mday,
													timeInfo->tm_mon,
													(timeInfo->tm_year + 1900),
													timeInfo->tm_hour,
													timeInfo->tm_min,
													timeInfo->tm_sec
	);
	return std::string(t_str);
}

void time_log_write(FILE* file, char* message) 
{
	time_t time_stamp;
	struct tm;
	if (file != NULL)
	  {

	  }
	else
	  {
		MessageBoxMC("");
	  }
}
#endif
