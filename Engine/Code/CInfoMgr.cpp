#include "CInfoMgr.h"
IMPLEMENT_SINGLETON(CInfoMgr)

CInfoMgr::CInfoMgr()
	: m_fScaleFactor(1.f)
	, m_iDpi(96)
	, m_iWINCX(WINCX)
	, m_iWINCY(WINCY)
	, m_hWnd(nullptr)
	, m_hInst(nullptr)
{
}

CInfoMgr::~CInfoMgr()
{
	Free();
}

void CInfoMgr::Free()
{

}