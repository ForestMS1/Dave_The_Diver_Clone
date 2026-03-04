#include "CTerrian.h"
#include "CGlbTex.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CAssetMgr.h"
#include "CAssetGlb.h"
#include "CManagement.h"
#include "CDiveDave.h"
#include "CCoral.h"
#include "CMapMgr.h"


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
                _vec3 vExtents = { 1.f * scale.x,1.f * scale.z,0.f };
                _vec3 vPos = { (*meshBound)[i].center };
                m_pAABB.emplace_back(CAABB::Create(&vPos, &vExtents, m_CollisionName, this));
            }

            _vec3 scale = (*meshBound)[meshBound->size()-1].scale;
            _vec3 vExtents = { 1.f * scale.x,1.f * scale.z,0.f };
            _vec3 vPos = { (*meshBound)[meshBound->size() - 1].center };
            m_pfrustomAABB = (CAABB::Create(&vPos, &vExtents, L"Optimization_Terrian", this));
            m_bFrustum = false;
            m_backGround = false;
        }


        if (m_wsName == L"GLB_Terrian1") {
            int iCount = 0;

            CGameObject* pGameObject;

            // Coral11
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(11);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
               
                m_vecCoral.emplace_back(pGameObject);
                // ----------------------------------------------------------------------------------
                nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);

                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
            }

            // Coral1
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(1);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
                // ----------------------------------------------------------------------------------
                nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);

                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
            }

            // Coral3
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(3);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
                // ----------------------------------------------------------------------------------
                nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);

                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
            }

            // Coral0
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(0);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
                // ----------------------------------------------------------------------------------
                nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);

                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
            }
         

            // Coral5
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(5);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
                // ----------------------------------------------------------------------------------
                nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);

                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
            }

            // Coral6
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(6);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
                // ----------------------------------------------------------------------------------
                nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);

                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
            }

            // Coral10
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(10);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
                // ----------------------------------------------------------------------------------
                nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);

                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;
                m_vecCoral.emplace_back(pGameObject);
            }




            //// Test
            //{
            //    wstring nameCoral = L"Tex_Coral" + ::to_wstring(5);

            //    //------------------------------------------------------------------------------------
            //    wstring nameCoralObject = L"CoralObjectTest_" + m_wsName + L"_" + ::to_wstring(iCount++);
            //    pGameObject = CCoral::Create(nameCoral, nameCoralObject);

            //    CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
            //    if (nullptr == pGameObject)
            //        return E_FAIL;
            //    if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
            //        return E_FAIL;
            //    m_vecCoral.emplace_back(pGameObject);


            //}
            
        }
        if (m_wsName == L"GLB_Terrian2") {
            int iCount = 0;

            CGameObject* pGameObject;

            // Coral8
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(8);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;

                m_vecCoral.emplace_back(pGameObject);

            }

            // Coral2
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(2);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;

                m_vecCoral.emplace_back(pGameObject);

            }

            // Coral10
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(10);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;

                m_vecCoral.emplace_back(pGameObject);

            }

            // Coral7
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(7);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;

                m_vecCoral.emplace_back(pGameObject);
                //------------------------------------------------------------------------------------
                nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);


                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;

                m_vecCoral.emplace_back(pGameObject);

            }

        }
        if (m_wsName == L"GLB_Terrian3") {
            int iCount = 0;

            CGameObject* pGameObject;

            // Coral8
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(8);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;

                m_vecCoral.emplace_back(pGameObject);

            }

            // Coral2
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(2);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;

                m_vecCoral.emplace_back(pGameObject);

            }

            // Coral10
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(10);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;

                m_vecCoral.emplace_back(pGameObject);

            }

            // Coral7
            {
                wstring nameCoral = L"Tex_Coral" + ::to_wstring(7);

                //------------------------------------------------------------------------------------
                wstring nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);

                CLayer* m_pLayer = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_Environment_Layer");
                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;

                m_vecCoral.emplace_back(pGameObject);
                //------------------------------------------------------------------------------------
                nameCoralObject = L"CoralObject_" + m_wsName + L"_" + ::to_wstring(iCount++);
                pGameObject = CCoral::Create(nameCoral, nameCoralObject);


                if (nullptr == pGameObject)
                    return E_FAIL;
                if (FAILED(m_pLayer->Add_GameObject(nameCoralObject, pGameObject)))
                    return E_FAIL;

                m_vecCoral.emplace_back(pGameObject);

            }

        }
        

        
    }
   
   

    return S_OK;
}

_int CTerrian::Update_GameObject(const _float& fTimeDelta)
{

    // 충돌체 그룹에 넣어줘야한다.
    if (e_Terrian == TERRIAN_ON) {
        for (auto i : m_pAABB) {
            CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Terrian", i);
            //i->Transform(m_pTransformCom->Get_World());
        }
    }
     
    CColliderMgr::GetInstance()->AddColliderGroup(m_CollisionName, m_pfrustomAABB);
    if (m_bFrustum) {
        _int iExit = CGameObject::Update_GameObject(fTimeDelta);

        CRenderer::GetInstance()->Add_RenderGroup(RENDER_NONALPHA, this);


        return iExit;
    }
  
}

void CTerrian::LateUpdate_GameObject(const _float& fTimeDelta)
{
    ColliderFrustom();
    if (m_bFrustum) {

        CGameObject::LateUpdate_GameObject(fTimeDelta);
    }
}

void CTerrian::Render_GameObject()
{
    if (m_bFrustum) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();




        if (FAILED(Ready_Material()))
            return;

        Set_Fog();
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
        pGraphicDev->SetRenderState(D3DRS_FOGENABLE, FALSE);
        pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


    }
  
}

HRESULT CTerrian::Ready_Material()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    D3DMATERIAL9			tMtrl;
    ZeroMemory(&tMtrl, sizeof(D3DMATERIAL9));


    CGameObject* pDiveDave = CMapMgr::GetInstance()->GetScene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"DiveDave");
    CTransform* pDaveTransform = static_cast<CTransform*>(pDiveDave->Get_Component(ID_DYNAMIC, L"Com_Transform"));
    _vec3 floor{};
    pDaveTransform->Get_Info(INFO_POS, &floor);

    if (floor.y <= -50.f && m_dark > 0.5f) {
        m_dark -= 0.01f;
    }
    else if(floor.y > -50.f && m_dark <= 1.f){
        m_dark += 0.01f;
    }
    else if (floor.y > -100.f && m_dark <= 0.5f) {
        m_dark += 0.01f;
    }
    else if (floor.y <= -100.f && m_dark >= 0.0f) {
        m_dark -= 0.01f;

    }
    tMtrl.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tMtrl.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    tMtrl.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

    tMtrl.Emissive = D3DXCOLOR(m_dark, m_dark, m_dark, 0.f);
    tMtrl.Power = 0.f;

    pGraphicDev->SetMaterial(&tMtrl);


    D3DLIGHT9 light; 
    ZeroMemory(&light, sizeof(D3DLIGHT9));

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

void CTerrian::Set_Fog() {
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    float Start = -50.f;
    float End   = 400.f;


    // m_dark는 1 → 0 구조 유지
    float t = 1.f - m_dark;   // 0 = 밝음, 1 = 어두움

    // 시작색 (밝은)
    const float r0 = 28.f;
    const float g0 = 188.f;
    const float b0 = 250.f;

    // 목표색 (어두운)
    const float r1 = 8.f;
    const float g1 = 56.f;
    const float b1 = 74.f;

    // 선형보간
    BYTE r = (BYTE)(r0 + (r1 - r0) * t);
    BYTE g = (BYTE)(g0 + (g1 - g0) * t);
    BYTE b = (BYTE)(b0 + (b1 - b0) * t);

    pGraphicDev->SetRenderState(D3DRS_FOGENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_FOGCOLOR, D3DCOLOR_XRGB(r, g, b));

    pGraphicDev->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
    pGraphicDev->SetRenderState(D3DRS_FOGSTART, *((DWORD*)&Start));
    pGraphicDev->SetRenderState(D3DRS_FOGEND, *((DWORD*)&End));

}

void CTerrian::ColliderFrustom() {
    if (CColliderMgr::GetInstance()->Get_Colliders(L"Coll_TestCamera") != nullptr) {
        CCollider* CameraCollider = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_TestCamera")->front();
        if (m_CollisionName != L"") {
            list<CCollider*>* ColliderList = CColliderMgr::GetInstance()->Get_Colliders(m_CollisionName);
            for (auto& pCollider : *ColliderList)
            {

                if (CameraCollider->Intersect(pCollider) && !m_backGround && TERRIAN_OFF == e_Terrian)
                {
                    e_Terrian = TERRIAN_ON;
                    m_bFrustum = true;
                    if (m_vecCoral.size() != 0) {
                        for (auto i : m_vecCoral) {
                            static_cast<CCoral*>(i)->Set_CoralTerrian(true);
                        }
                    }
                  
                }

                if (!CameraCollider->Intersect(pCollider) && !m_backGround && TERRIAN_ON == e_Terrian) {
                    e_Terrian = TERRIAN_OFF;
                    m_bFrustum = false;
                    for (auto i : m_vecCoral) {
                        static_cast<CCoral*>(i)->Set_CoralTerrian(false);
                    }
                }

            }
        }
      
        
    }
}
