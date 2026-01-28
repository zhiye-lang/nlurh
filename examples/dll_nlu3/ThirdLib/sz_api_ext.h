// ============================================================================
// File: sz_api_ext.h 
// Project: Zhiye Platform Basic SDK Encapsulation
// Description: Used to simplify the use of platform interfaces
// Attention: The reference must be placed after #include "ThirdBasics.h"
// 
// Copyright (c) 2024 Changsha Sizhi Information Technology Co., Ltd.
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this project.
// - Project Homepage: https://y-lang.tech
// ============================================================================

#ifndef _SZ_API_EXT_
#define _SZ_API_EXT_

extern void sz_ensure_unit(ExpUnit*& sz_unit);
extern void sz_set_unit_bool(ExpUnit*& sz_unit, int result);
extern void sz_set_unit_int(ExpUnit*& sz_unit, intptr_t val);
extern void sz_set_unit_num(ExpUnit*& sz_unit,number_t num);
extern void sz_set_unit_ptr(ExpUnit*& sz_unit, void* p);
extern DataBufStr* sz_get_dbs(const void *buf, size_t len);
extern DataBufStr* sz_realloc_dbs(DataBufStr* dbs, size_t len);
extern void sz_free_dbs(DataBufStr *pBfS);
//--The caller should NOT free the `dbs` memory after this call-----------------
extern void sz_take_unit_dbs(ExpUnit* &sz_unit, int tval, DataBufStr *dbs);
//----------General Assignment------------
#define SZ_GET_INT_VALUE(u) (u)->tval==NUM_NUM ?(u)->iNum:(u)->iVal

extern int sz_load_func_tab(ParUnit* &pStackPar,const SzFuncTable *pTbl);

#endif //_SZ_API_EXT_