#pragma once

#include "CBase.h"
#include "Engine_Define.h"

class CDSTestLoading : public CBase
{
public:
	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

public:
	explicit CDSTestLoading();
	virtual ~CDSTestLoading();

public:
	std::wstring_view	Get_String() { return m_sLoading; }
	LOADINGID		Get_LoadingID() { return m_eLoadingID; }
	_bool			Get_Finish() { return m_bFinish; }

	CRITICAL_SECTION* Get_Crt() { return &m_Crt; }

public:
	HRESULT		Ready_Loading(LOADINGID eID);
	_uint		Loading_Stage();

public:
	static unsigned int CALLBACK Thread_Main(void* pArg);


private:
	std::wstring				m_sLoading;

	HANDLE				m_hThread;
	LOADINGID			m_eLoadingID;

	CRITICAL_SECTION	m_Crt;
	_bool				m_bFinish;


public:
	static CDSTestLoading* Create(LOADINGID eID);

private:
	virtual void	Free();

};



