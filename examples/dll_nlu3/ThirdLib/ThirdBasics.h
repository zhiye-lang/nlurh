// ============================================================================
// File: ThirdBasics.h 
// Project: Basic definition of third-party development interface for ZhiYe
// Description: Define the fundamental structure and interfaces
// 
// Copyright (c) 2024 Changsha Sizhi Information Technology Co., Ltd.
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this project.
// - Project Homepage: https://y-lang.tech
// ========================================================================
//#pragma once
#ifndef _THIRD_BASICS_
#define _THIRD_BASICS_

#include <stdio.h>
#include <stdint.h>
#include <windows.h>

//==============Basic Type Definitions=<stdint.h>===========================
typedef uint8_t      u8;
typedef uint16_t     u16;
typedef uint32_t     u32;
//typedef uint64_t     u64;
//===================Content Usable Only in C++===========================================
#ifdef __cplusplus
typedef u16	cunit;	// Character unit definition, currently 16-bit width

//------Boolean Ternary Definition---Adds REVERSE (ANTI)-
#define ANTI  (-1)
/*
typedef enum
{
	FALSE	= 0,
	TRUE	= 1,
	ANTI	=-1,
}BOOL;
*/
//=======Data Space===space============
typedef enum
{
	SPACE_NULL=0,	// None
	SPACE_NAME=1,	// Name
	SPACE_POIT=2,	// Pointer
	SPACE_SUB=0x08,	// Subset
}SPACE_VAL;

//=======Parameter Type=========================
typedef enum
{
	TYPE_NULL,	//0 None
	TYPE_NUM,	//1 Number
	TYPE_DATA,	//2 Data

	TYPE_FUNC,	//3 C Function
	TYPE_SCR,	//4 Script

	TYPE_SEPA,	//5 Separator
}TYPE_VAL;
//--------Type Values----------------------------
typedef enum
{
	NUM_NULL,
	NUM_BOOL,	// Boolean
	NUM_NUM,	// Floating Point
	NUM_INT,	// Integer (signed decimal)
	NUM_POI,	// Address (unsigned hexadecimal)
}TVAL_NUM;

typedef enum
{
	DATA_NULL,
	DATA_BUF,	// Contiguous Data
	DATA_STR,	// String
}TVAL_DATA;

//=================Structure Declarations================================
typedef struct _ExpUnit ExpUnit;
typedef struct _ParUnit ParUnit;
//=================Function Definitions================================
#define FUNC_TYPE_I				1
typedef struct _DefEnv DefEnv,*EnvP;
typedef int(*sz_func_t)(DefEnv *, ExpUnit*&); //Function definition (environment parameter, input/output parameter) Returns input parameter count
typedef int(*sz_funp_t)(DefEnv &, ExpUnit*, ExpUnit*, ExpUnit*); //Enhanced function definition (env, before, after, itself)
//==========Contiguous Characters=====================
#ifdef _WIN32 //Windows platform compilation control
#pragma warning(disable : 4200)
#endif
typedef struct
{
	size_t Len;			// Parameter length
	union
	{
		u8		pBuf[]; //Buffer characters (tval==DATA_BUF)
		char	pStr[];	//String (tval==DATA_STR)
	};
}DataBufStr;

#if defined(_WIN64) || defined(__LP64__)  //64-bit definition
typedef double  number_t; //64-bit system, double precision definition
//======64-bit System Data Base Source (including data and logical relationships)===============
struct _ExpUnit
{
	u8		space;		// Space type, {@link SPACE_VAL}
	u8		type;		// Parameter type, {@link TYPE_VAL}
	u8		funt;		// Function, {@link DATA_FUNT}
	u8		tval : 4;	// Type value, {@link DATA_DEFINE_TYPE}
	u8		fval : 4;	// Classification

	float	multiple;	// Multiplier, effective for SPACE_POIT, SPACE_SUB.
	union
	{
		number_t	iNum;	// Floating-point value (type==TYPE_NUM && tval==NUM_NUM)
		intptr_t	iVal;	// Signed numeric value (type==TYPE_NUM && tval==NUM_INT)
		intptr_t	Result;	// Return result 1 and 0 (type==TYPE_NUM && tval==NUM_BOOL)
		void*		uPit;	// Unsigned numeric value (type==TYPE_NUM && fval == NUM_POI)
		DataBufStr*	pBfS;	// Buffer/String (type==TYPE_DATA)
		ExpUnit*	pUNIT;	// Sub-element or pointer (type==SPACE_POIT)
		cunit*	pFs;		// Internal execution segment: start address
		int		iVaA[2];
	};
	union
	{
		cunit*		pFe;    // Internal execution segment: end address
		intptr_t	offset; // Offset
		int		iVaB[2];
	};
	ExpUnit* pAttr; // Attribute, can also represent relationship
	//------------------------------------------------------------
	ExpUnit* pNext;
};
#else
//======32-bit System Data Base Source (including data and logical relationships)===============
typedef float     number_t;   //32-bit system, single precision definition
struct _ExpUnit
{
	u8		space;		// Space type, {@link SPACE_VAL}
	u8		type;		// Parameter type, {@link TYPE_VAL}
	u8		funt;		// Function, {@link DATA_FUNT}
	u8		tval : 4;	// Type value, {@link DATA_DEFINE_TYPE}
	u8		fval : 4;	// Classification
	union
	{
		number_t	iNum;	// Floating-point value (type==TYPE_NUM && tval==NUM_NUM)
		intptr_t	iVal;	// Signed numeric value (type==TYPE_NUM && tval==NUM_INT)
		intptr_t	Result;	// Return result 1 and 0 (type==TYPE_NUM && tval==NUM_BOOL)
		void*		uPit;	// Unsigned numeric value (type==TYPE_NUM && fval == NUM_POI)
		DataBufStr*	pBfS;	// Buffer/String (type==TYPE_DATA)
		ExpUnit*	pUNIT;	// Sub-element or pointer (type==SPACE_POIT)
		cunit*		pFs;	// Internal execution segment: start address
	};
	union
	{
		float		multiple;	// Multiplier, effective for SPACE_POIT, SPACE_SUB.
		cunit*		pFe;		// Internal execution segment: end address
		int			offset;		// Offset
	};
	ExpUnit*	pAttr;	// Attribute, can also represent relationship
	//------------------------------------------------------------
	ExpUnit*	pNext;
};
#endif
//====================Basic Functions (Core Version >= "3.0.0")=================================================
extern const char* sz_get_build_time();
extern void* sz_malloc(size_t len);
extern void* sz_realloc(void* p, size_t len);
extern void sz_free(void* p);
extern ExpUnit* sz_create_unit(void);
extern void sz_clean_unit_data(ExpUnit* pUnit);
extern void sz_free_unit_all(ExpUnit* pStar);
extern void sz_free_curr_and_next(ExpUnit* pStar, int Num);
extern ExpUnit* sz_free_unit(ExpUnit* pStar, int Num);
extern ExpUnit* sz_set_unit_lose(ExpUnit* pStar, int Num);
extern void sz_free_next(ExpUnit* pStar, int Num);
extern void sz_add_next(ExpUnit* pStar, ExpUnit* pAdd);
extern ExpUnit* sz_parsing_unit(ExpUnit* pUNIT);
extern intptr_t sz_get_unit_result(ExpUnit* pUNIT);
extern void sz_put_unit(char* pTitle, u8 NoSpace, ExpUnit* pUnit);
ExpUnit* sz_str_run(EnvP pEnv,const char *pStrFun,ExpUnit* par,int num)
extern DataBufStr* sz_read_file(const char* pathName);
extern intptr_t sz_save_file(const char* pathName, DataBufStr* pBfS);
extern int sz_input(const char* title, char* pOut, int oSize);
//------------Tcp---------------------------------------------------
extern int sz_tcp_connect(const char* host, const char* port, int proto);
extern int sz_tcp_send(int fd, const unsigned char* buf, size_t len);
extern int sz_tcp_recv(int fd, unsigned char* buf, size_t len);
extern int sz_tcp_recvto(int fd, unsigned char* buf, size_t len, u32 timeout);
extern int sz_tcp_bind(const char* bind_ip, const char* port, int proto);
extern int sz_tcp_accept(int* bind_fd, int* client_fd, void* client_ip, size_t buf_size, size_t* ip_len);
extern void sz_tcp_free(int fd);
//-----------------------NET---（proto: 0 for TCP, 1 for UDP, |0x10 for SSL）-------------------------------------------
typedef struct _NetCtx NetCtx;
extern NetCtx* sz_net_connect(const char* pHost, const char* pPort, int proto);
extern int sz_net_send(NetCtx* pNet, char* pIn, int iLen);
extern int sz_net_recv(NetCtx* pNet, char* pOut, int oSize, int timeOutMs);
extern int sz_net_disconnect(NetCtx* pNet);
//==========================Type I Function Corresponding Structure================================================
typedef struct
{
	const char*		pFunName;	// Function name, external name
	const sz_func_t	pFunExp;	// Processing function
}SzFuncEntryI;

typedef struct
{
	u16 type,Priority;			// Type + Priority
	u32 ver;					// Interface version
	const char* ArrayName;		// Group name (when present, interface access is: ArrayName.pFunName)
	const SzFuncEntryI ArrayI[];	// Terminated by NULL
}SzFuncTable;
//==========================External Total Structure Definition===============================
typedef struct
{
	const char* Info;			// Developer information
	const char* certificate;	// Developer authorization certificate (NULL if unregistered/no certificate)
	const SzFuncTable* gAll[];	// Terminated by NULL
}SzImportSystem;
#endif
//======================Log Output=================================================
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
	LOG_DEBUG,	// Show debug information
	LOG_INFO,	// Show informational message
	LOG_WARN,	// Show warning message
	LOG_ERROR,	// Show error message
} LOG_T;
typedef void (*sz_log_func_t)(LOG_T level, const char* fmt, ...);
typedef void (*sz_log_hex_func_t)(LOG_T level, const char* msg, void* buf, int len);
extern sz_log_func_t sz_OsLog;
extern sz_log_hex_func_t sz_OsLogHex;
#ifdef __cplusplus
}
#endif
//--------sz_OsLog and sz_OsLogHex output includes newline at the end-----------------------------------
/*
#define SZ_LOG(t,...) \
    do { \
        if (sz_OsLog) sz_OsLog(t, ##__VA_ARGS__); \
    } while(0)
#define SZ_LOG_HEX(t,msg,pBuf,Len) \
    do { \
        if (sz_OsLogHex) sz_OsLogHex(t,msg,pBuf,Len); \
    } while(0)
*/
#define SZ_LOG(t,...)					if(sz_OsLog) sz_OsLog(t, __VA_ARGS__)
#define SZ_LOG_HEX(t,msg,pBuf,Len)		if(sz_OsLogHex) sz_OsLogHex(t,msg,pBuf,Len)  
//#define SZ_LOG(t,...)					sz_OsLog(t, ##__VA_ARGS__)
//#define SZ_LOG_HEX(t,msg,pBuf,Len)		sz_OsLogHex(t,msg,pBuf,Len)
#define LG(...)							SZ_LOG(LOG_INFO, __VA_ARGS__)
#define LG_HEX(msg,pBuf,Len)			SZ_LOG_HEX(LOG_INFO,msg,pBuf,Len)

#endif
