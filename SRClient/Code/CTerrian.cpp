#include "CTerrian.h"
#include "CGlbTex.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CAssetMgr.h"
#include "CAssetGlb.h"
#include "CManagement.h"
#include "CDiveDave.h"


CTerrian::CTerrian()
    : CGameObject()
{
}

CTerrian::CTerrian(const wstring_view tex) : CGameObject(), m_wsName(tex), m_CollisionName(L"")
{
}

CTerrian::CTerrian(const wstring_view tex, const wstring_view Name) : CGameObject(), m_wsName(tex), m_CollisionName(Name)
{
}

CTerrian::CTerrian(const CTerrian& rhs)
    : CGameObject(rhs)
{
}

CTerrian::~CTerrian()
{
}

HRESULT CTerrian::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    //-------------AABB Collider With AABB_Terrian----------------
    m_pfrustomAABB = nullptr;

  
    if (m_CollisionName != L"") {
        vector<MeshBound>* meshBound;
        meshBound = nullptr;
        //meshBound = (CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(L"Terrian1_Collision")->Get_vec_meshBounds());
        meshBound = (CAssetMgr::GetInstance()->Get_AssetFirst<CAssetGlb>(m_CollisionName)->Get_vec_meshBounds());

        if (meshBound != nullptr) {
            for (int i = 0; i < meshBound->size() - 1; ++i) {

                _vec3 scale = (*meshBound)[i].scale;
                _vec3 vExtents = { 1.f * scale.x,1.f * scale.y,0.f };
                _vec3 vPos = { (*meshBound)[i].center };
                m_pAABB.emplace_back(CAABB::Create(&vPos, &vExtents, m_CollisionName, this));
            }

            _vec3 scale = (*meshBound)[meshBound->size()-1].scale;
            _vec3 vExtents = { 1.f * scale.x,1.f * scale.z,0.f };
            _vec3 vPos = { (*meshBound)[meshBound->size() - 1].center };
            m_pfrustomAABB = (CAABB::Create(&vPos, &vExtents, L"Optimization", this));
        }

    }
   
   

    return S_OK;
}

_int CTerrian::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);

    // 충돌체 그룹에 넣어줘야한다.
    for (auto i : m_pAABB) {
        CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Terrian", i);
        //i->Transform(m_pTransformCom->Get_World());
    }

    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Frustom", m_pfrustomAABB);

    return iExit;
}

void CTerrian::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CTerrian::Render_GameObject()
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

HRESULT CTerrian::Ready_Material()
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

HRESULT CTerrian::Ready_Component()
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



CTerrian* CTerrian::Create()
{
    CTerrian* pTerrian = new CTerrian;

    if (FAILED(pTerrian->Ready_GameObject()))
    {
        Safe_Release(pTerrian);
        MSG_BOX("pTerrian Create Failed");
        return nullptr;
    }

    return pTerrian;
}

CTerrian* CTerrian::Create(const wstring_view tex)
{
    CTerrian* pTerrian = new CTerrian(tex);

    if (FAILED(pTerrian->Ready_GameObject()))
    {
        Safe_Release(pTerrian);
        MSG_BOX("pTerrian Create Failed");
        return nullptr;
    }

    return pTerrian;
}

CTerrian* CTerrian::Create(const wstring_view tex, const wstring_view Name)
{
    CTerrian* pTerrian = new CTerrian(tex, Name);

    if (FAILED(pTerrian->Ready_GameObject()))
    {
        Safe_Release(pTerrian);
        MSG_BOX("CTestGlb Create Failed");
        return nullptr;
    }

    return pTerrian;
}

void CTerrian::Free()
{
    CGameObject::Free();

    // 충돌체 그룹에 넣어줘야한다.
    for (auto i : m_pAABB) {
        Safe_Release(i);
    }

    Safe_Release(m_pfrustomAABB);
  
}
