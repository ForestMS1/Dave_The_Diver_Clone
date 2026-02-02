#pragma once

#include "CBase.h"
#include "CTimer.h"

BEGIN(Engine)

class ENGINE_DLL CTimerMgr : public CBase
{
	DECLARE_SINGLETON(CTimerMgr)

private:
	explicit CTimerMgr();
	virtual ~CTimerMgr();

public:
	_float			Get_TimeDelta(std::wstring_view svTimerTag);
	void			Set_TimeDelta(std::wstring_view svTimerTag);

public:
	HRESULT			Ready_Timer(std::wstring_view svTimerTag);

private:
	CTimer* Find_Timer(std::wstring_view svTimerTag);

private:
	map<const std::wstring, CTimer*>		m_mapTimer;

private:
	virtual void		Free();
};

END