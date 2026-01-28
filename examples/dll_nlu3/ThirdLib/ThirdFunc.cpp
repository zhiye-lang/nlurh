// ============================================================================
// File: ThirdFunc.cpp 
// Project: Third-party interface development example
// Description: demo
// 
// Copyright (c) 2024 Changsha Sizhi Information Technology Co., Ltd.
// This source code is licensed under the MIT license found in the
// LICENSE file in the root directory of this project.
// - Project Homepage: https://y-lang.tech
// ========================================================================
#include "ThirdBasics.h"
#include "sz_api_ext.h"

//=======dll init=======================
void sz_lib_attach(void* p) { //LOG not yet initialized here, cannot use LOG output
	printf("Thirdlib Init...\n"); 
}

//=======dll deinit===========================
void sz_lib_detach(void *p) {
	printf("...Thirdlib DeInit");
}

//=============Function Interface Area============================
int exp_func1(EnvP evn, ExpUnit*& in_out)
{
	//Input parameters are linked sequentially; the next parameter must have the previous one present.
	ExpUnit *pOne = sz_parsing_unit(in_out);
	if(pOne != NULL) {
		LG("pOne->type=%d",pOne->type);
		ExpUnit *pTwo = sz_parsing_unit(in_out->pNext);
		if(pTwo != NULL) {
			LG("pTwo->type=%d",pTwo->type);
			ExpUnit *pThree = sz_parsing_unit(in_out->pNext->pNext);
			if(pThree != NULL) {
				LG("pThree->type=%d",pThree->type);
				//..........
			}
		}
	}
	//---------Execution Module--------------------
	//...
	//--------Fill Result Return------------------------
	sz_set_unit_bool(in_out,true); //First return parameter
	sz_set_unit_int(in_out->pNext,100); //Second return parameter
	sz_set_unit_num(in_out->pNext->pNext,200); //Third return parameter
	sz_take_unit_dbs(in_out->pNext->pNext->pNext, DATA_STR, sz_get_dbs("test OK",7)); //Fourth return parameter
	return 4;   //Return 4 parameters
}

int exp_func2(EnvP evn, ExpUnit*& in_out) //(a,b,c) -> (a+b)*c ->
{
	ExpUnit* pA = sz_parsing_unit(in_out);
	if (pA == NULL || pA->type != TYPE_NUM) return -5; //First parameter must be numeric, return -5 input parameters do not satisfy function execution.
	ExpUnit* pB = sz_parsing_unit(in_out->pNext);
	if (pB == NULL || pB->type != TYPE_NUM) return -5; //Second parameter must be numeric, return -5 input parameters do not satisfy function execution.
	ExpUnit* pC = sz_parsing_unit(in_out->pNext->pNext);
	if (pC == NULL || pC->type != TYPE_NUM) return -5; //Third parameter must be numeric, return -5 input parameters do not satisfy function execution.
	intptr_t a, b, c;
	a = SZ_GET_INT_VALUE(pA); //Regardless of whether pA is float or integer, convert to integer and assign to a.
	b = SZ_GET_INT_VALUE(pB); //Same as above
	c = SZ_GET_INT_VALUE(pC); //Same as above
	//--------Fill Result Return------------------------
	sz_set_unit_int(in_out, (a + b) * c); //First return parameter
	return 1;	//Return 1 parameter
}

int exp_func3(EnvP evn, ExpUnit*& in_out)
{
	ExpUnit *pPar = sz_parsing_unit(in_out);
	if (pPar == NULL || pPar->type != TYPE_DATA) return -5; //First parameter must be data type, return -5 input parameters do not satisfy function execution.
	//---------Execution Module--------------------
	LG("FunTest2[%d]:%s",pPar->pBfS->Len,pPar->pBfS->pStr);
	//--------Display All Attributes---------------
	DataBufStr *dbs = sz_get_dbs(NULL,pPar->pBfS->Len+4); //Allocate Len+4 space
	memcpy(dbs->pStr,pPar->pBfS->pStr, pPar->pBfS->Len);
	strcpy(dbs->pStr+pPar->pBfS->Len,"+dgz");
	//--------Fill Result Return------------------------
	sz_take_unit_dbs(in_out, DATA_STR, dbs); //First return parameter, dbs is already embedded in in_out, no need to free
	return 1;   //Return 1 parameter
}

int exp_square(EnvP env, ExpUnit* &in_out)
{
	ExpUnit *pPar = sz_parsing_unit(in_out);
	//---------Execution Module--------------------
	if (pPar->type == TYPE_NUM) {
		if(pPar->tval == NUM_NUM)
			in_out->iNum = pPar->iNum * pPar->iNum;
		else
			in_out->iVal = pPar->iVal * pPar->iVal;
	}
	else return 0;
	in_out->space = SPACE_NULL; //Eliminate pointer type
	in_out->type = pPar->type;
	return 1;
}

int exp_read_file(EnvP evn, ExpUnit*& in_out) //(pathname) -> data
{
	ExpUnit *PathName = sz_parsing_unit(in_out);
	if (PathName == NULL || PathName->type != TYPE_DATA) return -5; //First parameter must be data type
	//---------Execution Module--------------------
	LG("sz_read_file[%d]:%s",PathName->pBfS->Len,PathName->pBfS->pStr);
	DataBufStr *dbs = sz_read_file(PathName->pBfS->pStr);
	//--------Fill Result Return------------------------
	sz_take_unit_dbs(in_out, DATA_BUF, dbs); //First return parameter, dbs is already embedded in in_out, no need to free (when dbs is null, it is internally converted to FALSE)
	return 1;	//Return 1 parameter
}

//=====================Configuration==================================================
//=========Function Correspondence Table (Name, Function, Usage Rules, Terminated by NULL)=================
static const SzFuncTable funs_normal_tbl =
{
	FUNC_TYPE_I,	1,
	(1 << 16) | (1 << 8) | (10), //ver:1.1.10
	"test", //ArrayName is "test",Add "test." when accessing 
	{
		"fun1",		exp_func1,
		"fun2",		exp_func2,
		"fun3",		exp_func3,
		"square",	exp_square,
		"readfile",	exp_read_file,
		NULL,		NULL,	//Ending row
	}
};
//Note: The call method is: test.fun1(param1, param2, param3, ...). When ArrayName is NULL, it means no table name is needed, just call the function name directly.

/*
Developer Information (JSON format {id:var,id2:var2,....}) Description:
ID: Registered developer ID (basis for copyright, contribution, revenue entitlement). Optional if none.
Company: Personal developers do not need to fill this.
Author: Developer's name.
Phone: Developer's phone number (can also serve as basis for contribution copyright when no developer ID exists).
*/
const char sDeveloperInfo[] = \
"{\"ID\":\"00000001-0001-0001\","\
"\"Company\":\"Changsha Sizhi\","        \
"\"Phone\":\"0731****8888\","     \
"\"Author\":\"ZhiYe\","             \
"\"Mobile\":\"138****8888\"}";

//const char sDeveloperCert[]

//==============Export Total Interface: gTotalInterSet==================
extern const SzImportSystem gTotalInterSet =
{
	sDeveloperInfo,
	NULL, //Developer certificate
	{
		&funs_normal_tbl,
		NULL,	//Ending row
	}
};




