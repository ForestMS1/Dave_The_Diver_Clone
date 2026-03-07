#include "CLight.h"
#include "CAllLight.h"
#include "CGraphicDev.h"
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


    // 해저용 전체광
    tLight.Type = D3DLIGHT_DIRECTIONAL;

    // 위에서 아래로 비추는 느낌
    tLight.Direction = D3DXVECTOR3(0.15f, -1.0f, 0.2f);

    // 메인 색
    tLight.Diffuse = D3DXCOLOR(0.20f, 0.45f, 0.50f, 1.f);

    // 반짝임은 약하게
    tLight.Specular = D3DXCOLOR(0.08f, 0.15f, 0.18f, 1.f);

    // 라이트 자체 Ambient는 크게 의미 없지만 같이 세팅
    tLight.Ambient = D3DXCOLOR(0.03f, 0.08f, 0.10f, 1.f);
    

    pGraphicDev->SetLight(iIndex, &tLight);
    pGraphicDev->LightEnable(iIndex, TRUE);

    pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);

    // 장면 전체 환경광
    pGraphicDev->SetRenderState(D3DRS_AMBIENT, D3DCOLOR_XRGB(8, 20, 24));

    // 필요하면 스페큘러 켜기
    pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);

    return S_OK;
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
