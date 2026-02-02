#pragma once

#include "CBase.h"
#include "Engine_Define.h"

class CLoading : public CBase
{
public:
	enum LOADINGID { LOADING_STAGE, LOADING_BOSS, LOADING_END };

public:
	explicit CLoading(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLoading();

public:
	std::wstring_view	Get_String()	{ return m_sLoading;}
	LOADINGID		Get_LoadingID() { return m_eLoadingID; }
	_bool			Get_Finish()	{ return m_bFinish; }

	CRITICAL_SECTION*		Get_Crt() { return &m_Crt; }
 
public:
	HRESULT		Ready_Loading(LOADINGID eID);
	_uint		Loading_Stage();

public:
	static unsigned int CALLBACK Thread_Main(void* pArg);


private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	std::wstring				m_sLoading;

	HANDLE				m_hThread;
	LOADINGID			m_eLoadingID;

	CRITICAL_SECTION	m_Crt;
	_bool				m_bFinish;


public:
	static CLoading* Create(LPDIRECT3DDEVICE9 pGraphicDev, LOADINGID eID);

private:
	virtual void	Free();

};



