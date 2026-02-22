#include "pch.h"
#include "CTestFish.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"

CTestFish::CTestFish()
    : CGameObject()
{
}

CTestFish::CTestFish(const CTestFish& rhs)
    : CGameObject(rhs)
{
}

CTestFish::~CTestFish()
{
}

HRESULT CTestFish::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };

    _vec3 vPos = { 00.0f, 0.0f, 0.0f };

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Boat", this);
    m_pTransformCom->Set_Pos(-3.f, -3.f, 0.f);
    return S_OK;
}

_int CTestFish::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    // 충돌체 그룹에 넣어줘야한다.
    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Ship", m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());

    return iExit;
}

void CTestFish::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTestFish::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTestFish::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_TestFishTexture", L"Com_Texture", &m_pTextureCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CTestFish* CTestFish::Create()
{
    CTestFish* pTestFish = new CTestFish;

    if (FAILED(pTestFish->Ready_GameObject()))
    {
        Safe_Release(pTestFish);
        MSG_BOX("pTestFish Create Failed");
        return nullptr;
    }

    return pTestFish;
}

void CTestFish::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}
