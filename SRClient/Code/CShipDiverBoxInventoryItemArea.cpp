#include "pch.h"
#include "CShipDiverBoxInventoryItemArea.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CColliderMgr.h"
#include "CManagement.h"
#include "CHelper.h"
#include "CDInputMgr.h"
#include "CShipPhone.h"

CShipDiverBoxInventoryItemArea::CShipDiverBoxInventoryItemArea(_uint iAppIdx, _float fPosX, _float fPosY)
    : CGameObject()
    , m_iIdx(iAppIdx)
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CShipDiverBoxInventoryItemArea::~CShipDiverBoxInventoryItemArea()
{
}

void CShipDiverBoxInventoryItemArea::Update_ImGui()
{
    CGameObject::Update_ImGui();
    ImGui::DragFloat("OffsetPosX", &m_fPosX, 0.1f);
    ImGui::DragFloat("OffsetPosY", &m_fPosY, 0.1f);


    ImGui::DragFloat("OffsetScaleX", &m_fScaleX, 0.1f);
    ImGui::DragFloat("OffseScaletY", &m_fScaleY, 0.1f);
}


HRESULT		CShipDiverBoxInventoryItemArea::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    float refX = 0.92f / 2.f;
    float refY = 0.88f / 2.f;
    _vec3 vExtents = { refX, refY, 0.01f };

    m_fScaleX = 1.f;
    m_fScaleY = 1.f;
    _vec3 vScale = { m_fScaleX , m_fScaleY, 1.f };
    m_pTransformCom->Set_Scale(&vScale);
    _vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    //m_pTransformCom->Update_Component(0.f);

    _vec3 vOri = { 0.f, 0.f, 0.f };

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_DiverBoxItemArea", this);

    return S_OK;
}

_int		CShipDiverBoxInventoryItemArea::Update_GameObject(const _float& fTimeDelta)
{

    _vec3 vPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

    float fOffsetX = m_fPosX;
    float fOffsetY = m_fPosY;
    vPos.x += fOffsetX;
    vPos.y += fOffsetY;

    m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);

    _vec3 vScale = { m_fScaleX , m_fScaleY, 1.f };
    m_pTransformCom->Set_Scale(&vScale);


    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_DiverBoxItemArea", m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    //CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);


   

    // 충돌체 그룹에 넣어줘야한다.

    return iExit;
}

void		CShipDiverBoxInventoryItemArea::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    
}

void		CShipDiverBoxInventoryItemArea::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    // 2. 투명도 설정 (0~255) - 예: 128은 약 50% 투명도
    float alpha = 0.5f;
    DWORD alphaValue = (DWORD)(alpha * 255);
    pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, D3DCOLOR_ARGB(alphaValue, 255, 255, 255));

    // 3. 텍스쳐의 RGB는 그대로 쓰고, 알파는 TFACTOR에서 가져오도록 설정
    //pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
    //pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

    //pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
    //pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

    //pGraphicDev->SetTransform(D3DTS_WORLD, &m_matNewWorld);




    //m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT			CShipDiverBoxInventoryItemArea::Ready_Component()
{
    // 버퍼
    //if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
    //    return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}
CShipDiverBoxInventoryItemArea* CShipDiverBoxInventoryItemArea::Create(_uint iAppIdx, _float fPosX, _float fPosY)
{
    CShipDiverBoxInventoryItemArea* pShipPhoneApp = new CShipDiverBoxInventoryItemArea{ iAppIdx, fPosX, fPosY };

    if (FAILED(pShipPhoneApp->Ready_GameObject()))
    {
        Safe_Release(pShipPhoneApp);
        MSG_BOX("pShipPhoneApp Create Failed");
        return nullptr;
    }

    return pShipPhoneApp;
}

void CShipDiverBoxInventoryItemArea::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}
