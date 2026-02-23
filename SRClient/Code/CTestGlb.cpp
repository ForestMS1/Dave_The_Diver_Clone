#include "CTestGlb.h"
#include "CGlbTex.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CAssetMgr.h"
#include "CAssetGlb.h"
#include "CManagement.h"


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


    //-------------AABB Collider With AABB_Terrian----------------

    vector<MeshBound>* meshBound;
    meshBound = (CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian1_Collision")->Get_vec_meshBounds());
   
    for (int i = 0; i < meshBound->size(); ++i) {
        float aspect = (*meshBound)[i].size.x / (*meshBound)[i].size.y;
        float scale = 3.5f;
        _vec3 vExtents = { aspect* scale,1.f* scale,0.f };
        _vec3 vPos = { (*meshBound)[i].center};
        m_pAABB.emplace_back(CAABB::Create(&vPos, &vExtents, L"AABB_Terrian1", this));
    }
    




    return S_OK;
}

_int CTestGlb::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    // 충돌체 그룹에 넣어줘야한다.
    for (auto i : m_pAABB) {
        CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_GLB", i);
        //i->Transform(m_pTransformCom->Get_World());
    }


    return iExit;
}

void CTestGlb::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTestGlb::Render_GameObject()
{
    CColliderMgr::GetInstance()->Set_Render(true);
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();




    if (FAILED(Ready_Material()))
        return;


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

    pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


   
}

HRESULT CTestGlb::Ready_Material()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    D3DMATERIAL9			tMtrl;
    ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));

    tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tMtrl.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);

    tMtrl.Emissive = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.f);
    tMtrl.Power = 0.f;

    pGraphicDev->SetMaterial(&tMtrl);


    D3DLIGHT9 light; 
    ZeroMemory(&light, sizeof(D3DLIGHT9));
    CTransform* pDaveTransform = static_cast<CTransform*>( CManagement::GetInstance()->Get_FirstObjectComponent(ID_DYNAMIC, L"0_GameLogic_Layer", L"DiveDave", L"Com_Transform"));
    light.Type = D3DLIGHT_POINT; 
    light.Diffuse.r = 1.0f;
    light.Diffuse.g = 1.0f;
    light.Diffuse.b = 1.0f;
    light.Attenuation0 = 0.0f;
    light.Attenuation1 = 0.1f;
    light.Attenuation2 = 0.0f;
    light.Range = 10.f; 
    _vec3 Pos;
    pDaveTransform->Get_Info(INFO_POS, &Pos);
    light.Position = Pos;
    pGraphicDev->SetLight(1, &light); 
    pGraphicDev->LightEnable(1, TRUE);
    pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    pGraphicDev->SetRenderState(D3DRS_AMBIENT, 0x00202020);



    return S_OK;
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
    // 충돌체 그룹에 넣어줘야한다.
    for (auto i : m_pAABB) {
        Safe_Release(i);
    }
   
}
