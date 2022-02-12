#include <stdio.h>
#include <windows.h>
#include <winbase.h>

#ifdef WIN32

void MessageBoxMC(char * message);

std::string GetCurrentDirectoryMC(void);
std::string DateTimeToStr(time_t tc);
void time_log_write(FILE* file, char* message);

#endif