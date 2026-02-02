#include "CTimerMgr.h"

IMPLEMENT_SINGLETON(CTimerMgr)

CTimerMgr::CTimerMgr()
{
}

CTimerMgr::~CTimerMgr()
{
	Free();
}

_float CTimerMgr::Get_TimeDelta(std::wstring_view svTimerTag)
{
	CTimer* pTimer = Find_Timer(svTimerTag);
	if (nullptr == pTimer)
		return 0.f;

	return pTimer->Get_TimeDelta();
}

void CTimerMgr::Set_TimeDelta(std::wstring_view svTimerTag)
{
	CTimer* pTimer = Find_Timer(svTimerTag);
	if (nullptr == pTimer)
		return;

	pTimer->Update_Timer();
}

HRESULT CTimerMgr::Ready_Timer(std::wstring_view svTimerTag)
{
	CTimer* pTimer = Find_Timer(svTimerTag);

	if (nullptr != pTimer)
		return E_FAIL;

	pTimer = CTimer::Create();
	if (nullptr == pTimer)
		return E_FAIL;

	m_mapTimer.insert({ std::wstring(svTimerTag), pTimer });

	return S_OK;
}

CTimer* CTimerMgr::Find_Timer(std::wstring_view svTimerTag)
{
	auto		iter = find_if(m_mapTimer.begin(), m_mapTimer.end(),
		CTag_FinderSV(svTimerTag));

	if (iter == m_mapTimer.end())
		return nullptr;

	return iter->second;
}

void CTimerMgr::Free()
{
	for_each(m_mapTimer.begin(), m_mapTimer.end(), CDeleteMap());
	m_mapTimer.clear();
}
