#include "CTestGlb.h"
#include "CGlbTex.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CAssetMgr.h"
#include "CAssetGlb.h"

CTestGlb::CTestGlb()
    : CGameObject()
{
}

CTestGlb::CTestGlb(const wstring_view tex) : CGameObject(), m_wsName(tex)
{
}

CTestGlb::CTestGlb(const CTestGlb& rhs)
    : CGameObject(rhs)
{
}

CTestGlb::~CTestGlb()
{
}

HRESULT CTestGlb::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };

    _vec3 vPos = { 0.0f, 0.0f, 0.f };

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_GLB", this);

    return S_OK;
}

_int CTestGlb::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    // 충돌체 그룹에 넣어줘야한다.
    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_GLB", m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());

    return iExit;
}

void CTestGlb::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTestGlb::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
  //  pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    for (int i = 0; i < CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(m_wsName)->Get_MeshCnt(); ++i) {
        pGraphicDev->SetTexture(0, (*CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(m_wsName)->Get_Texture())[i]);
        
        _uint first = (*CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(m_wsName)->Get_vecTexVtxTriCnt())[i].first;
        _uint second = (*CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(m_wsName)->Get_vecTexVtxTriCnt())[i].second;
        m_pBufferCom->Render_Buffer(first, second);

    }

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);
   // pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTestGlb::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CGlbTex, ID_STATIC>(m_wsName, L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    //if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_PlayerTexture", L"Com_Texture", &m_pTextureCom))))
    //    return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CTestGlb* CTestGlb::Create()
{
    CTestGlb* pTestGlb = new CTestGlb;

    if (FAILED(pTestGlb->Ready_GameObject()))
    {
        Safe_Release(pTestGlb);
        MSG_BOX("CTestGlb Create Failed");
        return nullptr;
    }

    return pTestGlb;
}

CTestGlb* CTestGlb::Create(const wstring_view tex)
{
    CTestGlb* pTestGlb = new CTestGlb(tex);

    if (FAILED(pTestGlb->Ready_GameObject()))
    {
        Safe_Release(pTestGlb);
        MSG_BOX("CTestGlb Create Failed");
        return nullptr;
    }

    return pTestGlb;
}

void CTestGlb::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}
