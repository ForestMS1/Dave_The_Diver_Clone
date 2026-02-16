#include "pch.h"
#include "CTestAmericanLobster.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CAssetSpine.h"

CTestAmericanLobster::CTestAmericanLobster()
    : CGameObject()
{
}

//CTestAmericanLobster::CTestAmericanLobster(const CTestAmericanLobster& rhs)
//    : CGameObject(rhs)
//{
//}

CTestAmericanLobster::~CTestAmericanLobster()
{
}

HRESULT CTestAmericanLobster::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };

    _vec3 vPos = { 00.0f, 0.0f, 0.0f };

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Lobster", this);

    m_iFrame = 0;
    m_fAccFrameDelta = 0.f;
    m_sCurrentMotion = L"";
    m_bSeeRight = true;

    _vec3 vScale = {0.1f, 0.1f, 0.1f};
    m_pTransformCom->Set_Scale(&vScale);

    CAssetMgr::GetInstance()->Get_AssetFirst<CAssetSpine>(L"Test_Spine")->Set_AniState(L"move");

    return S_OK;
}

_int CTestAmericanLobster::Update_GameObject(const _float& fTimeDelta)
{
    Key_Input(fTimeDelta);

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    //m_fFrame += 2.f * fTimeDelta;

    m_fAccFrameDelta += fTimeDelta;

    if (m_fAccFrameDelta > 0.2f)
    {
        ++m_iFrame;
        m_fAccFrameDelta = 0;
    }

    //auto zz = CAssetMgr::GetInstance()->Get_Asset(m_sCurrentMotion)->size();
    //if (CAssetMgr::GetInstance()->Get_Asset(m_sCurrentMotion)->size() <= m_iFrame)
    //    m_iFrame = 0;

    CAssetMgr::GetInstance()->Get_AssetFirst<CAssetSpine>(L"Test_Spine")->TempUpdate(fTimeDelta);


    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    // 충돌체 그룹에 넣어줘야한다.
    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_TestLobster", m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());

    return iExit;
}

void CTestAmericanLobster::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    // Test 레이어에있는 충돌체 리스트를 들고온다. 널체크
    if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_Ship"))
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

void CTestAmericanLobster::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    //if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sCurrentMotion))
    //{
    //    if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(m_iFrame)))
    //    {
    //        pGraphicDev->SetTexture(0, pTexture->Get_Texture());
    //    }
    //}

    //m_pBufferCom->Render_Buffer();


    auto pAssSpine = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetSpine>(L"Test_Spine");


    pGraphicDev->SetTexture(0, CAssetMgr::GetInstance()->Get_AssetFirst<CAssetTexture>(pAssSpine->Get_TextureName())->Get_Texture());

    {
        
        pAssSpine->TEMP_LOCK_BUFFER(m_pBufferCom->Get_VertexBuffer(), m_pBufferCom->Get_IndexBuffer());
        
    }

    m_pBufferCom->Render_Buffer();



    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CTestAmericanLobster::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CDynamicBuffer, ID_STATIC>(L"Proto_DynamicBuffer_Spine", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    auto pAssSpine =CAssetMgr::GetInstance()->Get_AssetFirst<CAssetSpine>(L"Test_Spine");
    
    m_pBufferCom->Set_VertexCnt(pAssSpine->Get_NumVertex());
    m_pBufferCom->Set_TriCnt(pAssSpine->Get_NumTri());
    m_pBufferCom->Ready_Buffer();

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

void CTestAmericanLobster::Key_Input(const _float& fTimeDelta)
{

    _vec3 vDirUp = { 0.f, 1.f, 0.f };
    _vec3 vDirRight = { 1.f, 0.f, 0.f };
    m_pTransformCom->Get_Info(INFO_UP, &vDirUp);
    m_pTransformCom->Get_Info(INFO_RIGHT, &vDirRight);
    bool m_bKeyInput = false;;
    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_W))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDirUp, &vDirUp), 3.f, fTimeDelta);
    }

    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_S))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDirUp, &vDirUp), -3.f, fTimeDelta);
    }

    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_A))
    {
        m_bKeyInput = true;
        //Motion_Change(L"Tex_ShipDave_Walk");
        m_pTransformCom->Move_Pos(&vDirRight, 6.f, fTimeDelta);
        if (m_bSeeRight)
        {
            m_bSeeRight = false;
            m_pTransformCom->Rotation(ROT_Y, 180.f);
        }
    }

    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_D))
    {
        m_bKeyInput = true;
        //Motion_Change(L"Tex_ShipDave_Walk");
        m_pTransformCom->Move_Pos(&vDirRight, 6.f, fTimeDelta);
        if (!m_bSeeRight)
        {
            m_bSeeRight = true;
            m_pTransformCom->Rotation(ROT_Y, 180.f);
        }
    }

    if (!m_bKeyInput)
    {
        //Motion_Change(L"Tex_ShipDave_Idle");
    }

}

void CTestAmericanLobster::Motion_Change(wstring_view svMotion)
{
    if (m_sCurrentMotion != svMotion)
    {
        m_sCurrentMotion = svMotion;
        m_iFrame = 0;
        m_fAccFrameDelta = 0.f;
    }
}

CTestAmericanLobster* CTestAmericanLobster::Create()
{
    CTestAmericanLobster* pShipDave = new CTestAmericanLobster;

    if (FAILED(pShipDave->Ready_GameObject()))
    {
        Safe_Release(pShipDave);
        MSG_BOX("pShipDave Create Failed");
        return nullptr;
    }

    return pShipDave;
}

void CTestAmericanLobster::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}
