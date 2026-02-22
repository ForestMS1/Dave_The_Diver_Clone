#include "pch.h"
#include "CShipTransformCam.h"
#include "CGraphicDev.h"

CShipTransformCam::CShipTransformCam()
    : CGameObject()
    , m_vAt(_vec3(0.f, 0.f, 0.f))
    , m_vUp(_vec3(0.f, 1.f, 0.f))
{
}

CShipTransformCam::~CShipTransformCam()
{
}

void CShipTransformCam::Update_ImGui()
{
    CGameObject::Update_ImGui();
    ImGui::DragFloat3("At", &m_vAt.x, 0.1f);
}


_int		CShipTransformCam::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    
    _vec3 vEye;
    m_pTransformCom->Get_Info(INFO_POS, &vEye);

    _matrix	matView;

    D3DXMatrixLookAtLH(&matView, &vEye, &m_vAt, &m_vUp);
    CGraphicDev::GetInstance()->Get_GraphicDev()->SetTransform(D3DTS_VIEW, &matView);

    return iExit;
}

void		CShipTransformCam::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipTransformCam::Render_GameObject()
{

}


HRESULT		CShipTransformCam::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_pTransformCom->Set_Pos(0.f, 0.f, -10.f);
    return S_OK;
}

HRESULT			CShipTransformCam::Ready_Component()
{
    // Æ®·£½ºÆû
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipTransformCam* CShipTransformCam::Create()
{
    CShipTransformCam* pCam = new CShipTransformCam;

    if (FAILED(pCam->Ready_GameObject()))
    {
        Safe_Release(pCam);
        MSG_BOX("pCam Create Failed");
        return nullptr;
    }

    return pCam;
}

void CShipTransformCam::Free()
{
    CGameObject::Free();
}