#include "CInfoMgr.h"
IMPLEMENT_SINGLETON(CInfoMgr)

CInfoMgr::CInfoMgr()
	: m_fScaleFactor(1.f)
	, m_iDpi(96)
	, m_iWINCX(WINCX)
	, m_iWINCY(WINCY)
{
}

CInfoMgr::~CInfoMgr()
{
	Free();
}

void CInfoMgr::Free()
{

}