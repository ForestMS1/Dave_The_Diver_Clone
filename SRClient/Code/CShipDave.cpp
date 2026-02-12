#include "CShipDave.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"

CShipDave::CShipDave()
    : CGameObject()
{
}

CShipDave::CShipDave(const CShipDave& rhs)
    : CGameObject(rhs)
{
}

CShipDave::~CShipDave()
{
}

HRESULT CShipDave::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };

    _vec3 vPos = { 00.0f, 0.0f, 0.0f };

    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Dave", this);
  

    return S_OK;
}

_int CShipDave::Update_GameObject(const _float& fTimeDelta)
{
    Key_Input(fTimeDelta);
    
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    // 충돌체 그룹에 넣어줘야한다.
    CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Ship", m_pAABB);
    m_pAABB->Transform(m_pTransformCom->Get_World());

    return iExit;
}

void CShipDave::LateUpdate_GameObject(const _float& fTimeDelta)
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

                    if (pCollider->Get_Tag() == L"AABB_Boat")
                    {
                        CCollisionMgr::COLL_RECT_EX_INFO info;
                        if (CCollisionMgr::GetInstance()->Collision_RectEx(m_pAABB, dynamic_cast<CAABB*>(pCollider), &info))
                        {
                            _vec3 vPos;
                            m_pTransformCom->Get_Info(INFO_POS, &vPos);
                            if (info.eDir == CCollisionMgr::DIR_DOWN)
                            {
                                vPos.y += info.fDistance;
                                m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
                            }
                            else if (info.eDir == CCollisionMgr::DIR_UP)
                            {
                                vPos.y -= info.fDistance;
                                m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
                            }
                            else if (info.eDir == CCollisionMgr::DIR_LEFT)
                            {
                                vPos.x -= info.fDistance;
                                m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
                            }
                            else if (info.eDir == CCollisionMgr::DIR_RIGHT)
                            {
                                vPos.x += info.fDistance;
                                m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
                            }

                            m_pTransformCom->Update_Component(fTimeDelta);
                            m_pAABB->Transform(m_pTransformCom->Get_World());
                        }
                    }
                }
            }
        }
    }
}

void CShipDave::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    //m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CShipDave::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    //if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_PlayerTexture", L"Com_Texture", &m_pTextureCom))))
    //    return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}

void CShipDave::Key_Input(const _float& fTimeDelta)
{

    _vec3		vDirUp, vDirRight;
    _vec3		vUp(0.f, 1.f, 0.f);
    m_pTransformCom->Get_Info(INFO_UP, &vDirUp);
    m_pTransformCom->Get_Info(INFO_RIGHT, &vDirRight);
    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_W))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDirUp, &vDirUp), 10.f, fTimeDelta);
    }

    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_S))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDirUp, &vDirUp), -10.f, fTimeDelta);
    }

    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_A))
    {
        m_pTransformCom->Move_Pos(&vDirRight, -10.f, fTimeDelta);
    }

    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_D))
    {
        m_pTransformCom->Move_Pos(&vDirRight, 10.f, fTimeDelta);
    }

}

CShipDave* CShipDave::Create()
{
    CShipDave* pBackGround = new CShipDave;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("pBackGround Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CShipDave::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}
