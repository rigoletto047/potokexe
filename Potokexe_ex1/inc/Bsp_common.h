#ifndef __BSP_COMMON__
#define __BSP_COMMON_
#include <windows.h>


typedef struct _Bsp_Status
{
	DWORD	Command;
	DWORD	Status;
	int		Count;
	int		DebugCount;

	DWORD	ErrorTime;
	int		ErrorCode;
	int		ErrorData[4];
        //int mes[4];
} Bsp_Status, *PBsp_Status;

#define IS_PRESENT								0x01

#define SET_BIT(a,b,c)						((a) |=  ((b) << (((c) << 3))))
#define GET_BIT(a,b,c)						((a) &   ((b) << (((c) << 3))))
#define CLR_BIT(a,b,c)						((a) &= ~((b) << (((c) << 3))))

#define BSP_RESET(a)						memset((a),0,sizeof(Bsp_Status))

#define BSP_SET_COUNT(a,b)					((a)->Count=(b))
#define BSP_GET_COUNT(a)					((a)->Count)
#define BSP_SET_D_COUNT(a,b)				((a)->DebugCount=(b))
#define BSP_GET_D_COUNT(a)					((a)->DebugCount)

#define cmdNULL										0
#define cmdEXIT										1
#define cmdWORK										2
#define cmdSTART									3
#define cmdSTARTING									4
#define cmdERR										5
#define cmdPERES									6
#define cmdKARTA									7
#define cmdFILE										8
#define cmdBIN										9
#define cmdAZP										10
#define cmdEx										-1

#define BSP_SET_COMMAND(a,b)		((a)->Command = (b))
#define BSP_GET_COMMAND(a)			((a)->Command)
#define BSP_COMMAND_EXIT(a)			((a)->Command == cmdEXIT)

#define BSP_SET_STATUS(a,b)			((a)->Status = (b))
#define BSP_GET_STATUS(a)			((a)->Status)

#define BUFFER_SIZE					8192
#define	BUFFER_SIZE_MASK			(BUFFER_SIZE-1)

#define BSP_SET_ERROR_TIME(a,b)		((a)->ErrorTime=(b))
#define BSP_GET_ERROR_TIME(a)		((a)->ErrorTime)

#define BSP_SET_ERROR_CODE(a,b)		((a)->ErrorCode=(b))
#define BSP_GET_ERROR_CODE(a)		((a)->ErrorCode)

#define BSP_SET_ERROR_DATA(a,i,b)	((a)->ErrorData[(i)]=(b))
#define BSP_GET_ERROR_DATA(a,i)		((a)->ErrorData[(i)])

#define BSP_SET_DEBUGCOUNT(a,b)		((a)->DebugCount=(b))
#define BSP_GET_DEBUGCOUNT(a)		((a)->DebugCount)
#endif
