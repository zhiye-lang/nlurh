// ============================================================================
// File: sz_api_ext.cpp 
// Project: Zhiye Platform Basic SDK Encapsulation
// Description: Used to simplify the use of platform interfaces
// 
// Copyright (c) 2024 Changsha Sizhi Information Technology Co., Ltd.
// 
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this project.
// - Project Homepage: https://y-lang.tech
// ============================================================================
#include "ThirdBasics.h"

//===Element operation interface encapsulation===============
void sz_ensure_unit(ExpUnit*& sz_unit) {
	if(sz_unit)
		sz_clean_unit_data(sz_unit);
	else
		sz_unit = sz_create_unit();
}

void sz_set_unit_bool(ExpUnit*& sz_unit, int result) {
	sz_ensure_unit(sz_unit);
	sz_unit->type = TYPE_NUM;
	sz_unit->tval = NUM_BOOL;
	sz_unit->Result = result;
}

void sz_set_unit_int(ExpUnit*& sz_unit, intptr_t val) {
	sz_ensure_unit(sz_unit);
	sz_unit->type = TYPE_NUM;
	sz_unit->tval = NUM_INT;
	sz_unit->iVal = val;
}

void sz_set_unit_num(ExpUnit*& sz_unit,number_t num) {
	sz_ensure_unit(sz_unit);
	sz_unit->type = TYPE_NUM;
	sz_unit->tval = NUM_NUM;
	sz_unit->iNum = num;
}

void sz_set_unit_ptr(ExpUnit*& sz_unit, void* p) {
	sz_ensure_unit(sz_unit);
	sz_unit->type = TYPE_NUM;
	sz_unit->tval = NUM_POI;
	sz_unit->uPit = p;
}

DataBufStr * sz_get_dbs(const void *buf, size_t len) {
	DataBufStr *pData = (DataBufStr *)sz_malloc(sizeof(DataBufStr) + len + 1);
	if (buf) memcpy(pData->pBuf, buf, len);
	pData->pBuf[len] = 0;
	pData->Len = len;
	return pData;
}

DataBufStr* sz_realloc_dbs(DataBufStr* dbs, size_t len) {
	DataBufStr *pData = (DataBufStr *)sz_realloc(dbs,sizeof(DataBufStr) + len + 1);
	pData->pBuf[len] = 0;
	pData->Len = len;
	return pData;
}

void sz_free_dbs(DataBufStr *pBfS) {
	sz_free(pBfS);
}
//--The caller should NOT free the `dbs` memory after this call-----------------
void sz_take_unit_dbs(ExpUnit* &sz_unit, int tval, DataBufStr *dbs) {
	sz_ensure_unit(sz_unit);
	if (dbs) {
		sz_unit->type = TYPE_DATA;
		sz_unit->tval = tval;
		sz_unit->pBfS = dbs;
	}
	else { //Returns FALSE when dbs is NULL
		sz_unit->type = TYPE_NUM;
		sz_unit->tval = NUM_BOOL;
		sz_unit->Result = FALSE;
	}
}

