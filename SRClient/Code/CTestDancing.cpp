#include "pch.h"
#include "CTestDancing.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetSpine.h"
#include "CAssetFbx2.h"

CTestDancing::CTestDancing()
    : CGameObject()
{
}

//CTestDancing::CTestDancing(const CTestDancing& rhs)
//    : CGameObject(rhs)
//{
//}

CTestDancing::~CTestDancing()
{
}

HRESULT CTestDancing::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };

    _vec3 vPos = { 00.0f, 0.0f, 0.0f };

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Dancing", this);

    _vec3 vScale = { 1.0f, 1.0f, 1.0f };
    m_pTransformCom->Set_Scale(&vScale);

    return S_OK;
}

_int CTestDancing::Update_GameObject(const _float& fTimeDelta)
{
    if (CDInputMgr::GetInstance()->Key_Down(DIK_R))
    {

        {
            _vec3 vExtents = { 10.0f, 10.0f, 10.0f };
            _vec3 vPos = { 0.0f, 0.0f, 0.0f };
            m_pAABB->Set_BoundingBox(&vPos, &vExtents);
        }
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    //m_fFrame += 2.f * fTimeDelta;

    //CAssetMgr::GetInstance()->Get_AssetFirst<CAssetFbx2>(L"FBX_Boat")->Update_Animation(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_PRIORITY, this);

    // 충돌체 그룹에 넣어줘야한다.
    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_TestDancing", m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());

    return iExit;
}

void CTestDancing::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    // Test 레이어에있는 충돌체 리스트를 들고온다. 널체크
    if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_TestDancing"))
    {
        // 충돌체 순회
        for (auto& pCollider : *pColliders)
        {
            // 내가 아닌것들과 체크
            if (m_pAABB != pCollider)
            {
                // 충돌체 끼리 충돌 체크
                if (m_pAABB->Intersect(pCollider))
                {
                    // Some Logic
                    // 

                }
            }
        }
    }
}

void CTestDancing::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    //if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sCurrentMotion))
    //{
    //    if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(m_iFrame)))
    //    {
    //        pGraphicDev->SetTexture(0, pTexture->Get_Texture());
    //    }
    //}

    //m_pBufferCom->Render_Buffer();



    //m_pBufferCom->Render_Buffer();

    if (auto pTestFbx = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetFbx2>(L"FBX_Boat"))
    {
        for (int i = 0; i < pTestFbx->Get_Meshes()->size(); ++i)
        {
            const auto pVB = m_vecBufferCom[i]->Get_VertexBuffer();
            const auto pMesh = pTestFbx->Get_Meshes()->at(i);
            pMesh.VertexBuffer_Lock(pVB);

            pGraphicDev->SetTexture(0, nullptr);
            if (!pMesh.m_vecTexture.empty())
            {
                auto pAssetTex = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetTexture>(pMesh.m_vecTexture[0]);
                pGraphicDev->SetTexture(0, pAssetTex->Get_Texture());
            }

            m_vecBufferCom[i]->Render_Buffer();
        }
    }
    ;
   


    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTestDancing::Ready_Component()
{
    if (auto pTestFbx = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetFbx2>(L"FBX_Boat"))
    {
        int i = 0;
        for (const auto& pMesh : *pTestFbx->Get_Meshes())
        {
            // 버퍼
            CDynamicBuffer* pBuf;
            wstring sBufferComName = L"Com_Buffer" + to_wstring(i++);
            if (FAILED((AddComponent<Engine::CDynamicBuffer, ID_STATIC>(L"Proto_DynamicBuffer_Fbx", sBufferComName, &pBuf))))
                return E_FAIL;
            pBuf->Set_VertexCnt(pMesh.m_vecVertex.size());
            pBuf->Set_TriCnt(pMesh.m_vecIndex.size());
            pBuf->Ready_Buffer();

            pMesh.IndexBuffer_Lock(pBuf->Get_IndexBuffer());

            m_vecBufferCom.push_back(pBuf);
        }
    }
   
   

    

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

CTestDancing* CTestDancing::Create()
{
    CTestDancing* pShipDave = new CTestDancing;

    if (FAILED(pShipDave->Ready_GameObject()))
    {
        Safe_Release(pShipDave);
        MSG_BOX("pShipDave Create Failed");
        return nullptr;
    }

    return pShipDave;
}

void CTestDancing::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}
