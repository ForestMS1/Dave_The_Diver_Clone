#include "CLight.h"
#include "CAllLight.h"
#include "CGraphicDev.h"
#include "CGameMemMgr.h"
CAllLight::CAllLight()
{
}

CAllLight::~CAllLight()
{
}

HRESULT CAllLight::Ready_Light( const _uint& iIndex)
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    if (nullptr == pGraphicDev)
        return E_FAIL;

    D3DLIGHT9 tLight{};
    ZeroMemory(&tLight, sizeof(D3DLIGHT9));

    tLight.Type = D3DLIGHT_DIRECTIONAL;

    D3DXVECTOR3 vDir(0.15f, -1.f, 0.2f);
    D3DXVec3Normalize(&vDir, &vDir);
    tLight.Direction = vDir;
    m_iIndex = iIndex;
  
    float dark = CGameMemMgr::GetInstance()->Get_Dark();

    tLight.Diffuse = D3DXCOLOR(0.20f * dark, 0.45f * dark, 0.50f * dark, 1.f);
    tLight.Ambient = D3DXCOLOR(0.03f * dark, 0.08f * dark, 0.10f * dark, 1.f);
    tLight.Specular = D3DXCOLOR(0.08f * dark, 0.15f * dark, 0.18f * dark, 1.f);

    pGraphicDev->SetLight(iIndex, &tLight);
    pGraphicDev->LightEnable(iIndex, TRUE);
    pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);


    int r = int(8 * dark);
    int g = int(20 * dark);
    int b = int(24 * dark);

    pGraphicDev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(r, g, b));
    pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

    return S_OK;

    


}

void CAllLight::Off_Light()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();



    pGraphicDev->LightEnable(m_iIndex, FALSE);

    pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);



    pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
}

CAllLight* CAllLight::Create(const _uint& iIndex)
{
    CAllLight* pLight = new CAllLight();

    if (FAILED(pLight->Ready_Light(iIndex)))
    {
        Safe_Release(pLight);
        MSG_BOX("pLight Create Failed");
        return nullptr;
    }
    return pLight;
}

void CAllLight::Free()
{
}
