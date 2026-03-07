#include "CLight.h"
#include "CAllLight.h"
#include "CGraphicDev.h"
#include "CGameMemMgr.h"
#include "CSeaPointLight.h"

CSeaPointLight::CSeaPointLight()
{
}

CSeaPointLight::~CSeaPointLight()
{
}

HRESULT CSeaPointLight::Ready_Light(const _uint& iIndex)
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    if (nullptr == pGraphicDev)
        return E_FAIL;

    m_iIndex = iIndex;


    D3DLIGHT9 tLight{};
    ZeroMemory(&tLight, sizeof(D3DLIGHT9));

    tLight.Type = D3DLIGHT_POINT;
    tLight.Position = D3DXVECTOR3(p_Pos.x, p_Pos.y, p_Pos.z);

    tLight.Diffuse = D3DXCOLOR(0.30f, 0.85f, 0.78f, 1.f);
    tLight.Ambient = D3DXCOLOR(0.04f, 0.12f, 0.10f, 1.f);
    tLight.Specular = D3DXCOLOR(0.16f, 0.40f, 0.35f, 1.f);

    tLight.Range = 10.f;
    tLight.Attenuation0 = 0.0f;
    tLight.Attenuation1 = 0.18f;
    tLight.Attenuation2 = 0.03f;

    pGraphicDev->SetLight(iIndex, &tLight);
    pGraphicDev->LightEnable(iIndex, TRUE);

    return S_OK;
}

void CSeaPointLight::Off_Light()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();



    pGraphicDev->LightEnable(m_iIndex, FALSE);

    pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);



    pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
}

CSeaPointLight* CSeaPointLight::Create(const _uint& iIndex, _vec3 _pos)
{
    CSeaPointLight* pSeaPointLight = new CSeaPointLight();
    pSeaPointLight->Set_Pos(_pos);
    if (FAILED(pSeaPointLight->Ready_Light(iIndex)))
    {
        Safe_Release(pSeaPointLight);
        MSG_BOX("pSeaPointLight Create Failed");
        return nullptr;
    }
    return pSeaPointLight;
}

void CSeaPointLight::Free()
{
}
