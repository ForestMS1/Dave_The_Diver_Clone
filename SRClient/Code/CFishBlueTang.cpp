#include "pch.h"
#include "CFishBlueTang.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetSpine.h"

CFishBlueTang::CFishBlueTang(float fPosX, float fPosY)
    : CFishGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}


CFishBlueTang::~CFishBlueTang()
{
}

HRESULT CFishBlueTang::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 0.1, 0.1f, 0.1f };

    m_pTransformCom->Set_Scale(&vScale);

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);

    m_fViewZ = 0.5f;

    return S_OK;
}

_int CFishBlueTang::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CFishGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    if (ImGui::Button("ASDF"))
    {
        m_pSpineCom->Set_AniState(L"die");
    }
    return iExit;
}

void CFishBlueTang::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CFishGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CFishBlueTang::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());


    m_pSpineCom->Render(m_pDynamicBuffer);

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CFishBlueTang::Ready_Component()
{
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    // 버퍼
    if (FAILED((AddComponent<Engine::CDynamicBuffer, ID_STATIC>(L"Proto_DynamicBuffer_Spine", L"Com_DynamicBuffer", &m_pDynamicBuffer))))
        return E_FAIL;

    // 스파인
    if (FAILED((AddComponent<Engine::CSpine, ID_DYNAMIC>(L"Proto_Spine", L"Com_Spine", &m_pSpineCom))))
        return E_FAIL;

    if (auto pAssSpine = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetSpine>(L"Spine_Fish_BlueTang"))
    {
        m_pDynamicBuffer->Set_VertexCnt(pAssSpine->Get_NumVertex());
        m_pDynamicBuffer->Set_TriCnt(pAssSpine->Get_NumTri());
        m_pDynamicBuffer->Ready_Buffer();
        
        m_pSpineCom->Ready_AfterCreate(pAssSpine->Get_SkeletonData(), pAssSpine->Get_AnimationStateData(), L"Spine_Fish_BlueTang");
        m_pSpineCom->Index_Buffer_Lock(m_pDynamicBuffer->Get_IndexBuffer());

        m_pSpineCom->Set_AniState(L"swim");
    };



    return S_OK;
}


CFishBlueTang* CFishBlueTang::Create(float fPosX, float fPosY)
{
    CFishBlueTang* pBlueTang = new CFishBlueTang{fPosX, fPosY};

    if (FAILED(pBlueTang->Ready_GameObject()))
    {
        Safe_Release(pBlueTang);
        MSG_BOX("pBlueTang Create Failed");
        return nullptr;
    }

    return pBlueTang;
}

void CFishBlueTang::Free()
{
    CGameObject::Free();
}
