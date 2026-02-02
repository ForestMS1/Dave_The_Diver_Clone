#include "CFrameMgr.h"

IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr()
{
}

CFrameMgr::~CFrameMgr()
{
    Free();
}

HRESULT CFrameMgr::Ready_Frame(std::wstring_view svFrameTag,
                                const _float& fCallLimit)
{
    CFrame* pFrame = Find_Frame(svFrameTag);

    if (nullptr != pFrame)
        return E_FAIL;

    pFrame = CFrame::Create(fCallLimit);
    if (nullptr == pFrame)
        return E_FAIL;

    m_mapFrame.insert({std::wstring(svFrameTag), pFrame});

    return S_OK;
}

_bool CFrameMgr::IsPermit_Call(std::wstring_view svFrameTag,
                                const _float& fTimeDelta)
{
    CFrame* pFrame = Find_Frame(svFrameTag);

    if (nullptr == pFrame)
        return false;

    return pFrame->IsPermit_Call(fTimeDelta);
}

CFrame* CFrameMgr::Find_Frame(std::wstring_view svFrameTag)
{
    auto		iter = find_if(m_mapFrame.begin(), m_mapFrame.end(), CTag_FinderSV(svFrameTag));

    if (iter == m_mapFrame.end())
        return nullptr;

    return iter->second;
}

void CFrameMgr::Free()
{
    for_each(m_mapFrame.begin(), m_mapFrame.end(), CDeleteMap());
    m_mapFrame.clear();
}
