#include "CGameMemMgr.h"
IMPLEMENT_SINGLETON(CGameMemMgr)


CGameMemMgr::CGameMemMgr()
{
}

CGameMemMgr::~CGameMemMgr()
{
    Free();
}

void CGameMemMgr::Free()
{

}
HRESULT CGameMemMgr::Ready()
{

    return S_OK;
}