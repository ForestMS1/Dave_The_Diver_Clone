#include "CLightMgr.h"

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr()
{
}

CLightMgr::~CLightMgr()
{
    Free();
}

HRESULT CLightMgr::Ready_Light( const D3DLIGHT9* pLightInfo, const _uint& iIndex)
{
    CLight* pLight = CLight::Create( pLightInfo, iIndex);
    if (nullptr == pLight)
        return E_FAIL;

    m_LightList.push_back(pLight);

    return S_OK;
}

void CLightMgr::Free()
{
    for_each(m_LightList.begin(), m_LightList.end(), CDeleteObj());
    m_LightList.clear();
}
