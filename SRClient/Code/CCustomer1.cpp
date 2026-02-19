#include "pch.h"
#include "CCustomer1.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CDInputMgr.h"
#include "CManagement.h"
#include "CChair.h"


CCustomer1::CCustomer1()
    : CGameObject()
{
    curState = WALK;
    curDir = RIGHT;
    prevDir = RIGHT;
    Sitted = false;
    ChoosingMenu = false;
    Waiting = false;
}

CCustomer1::CCustomer1(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CCustomer1::~CCustomer1()
{
}

HRESULT CCustomer1::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fFrame = 0.f;

    m_pTransformCom->m_vScale = { 0.4f,0.9f,1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { -5.f ,-1.9f,-2.8f };
    return S_OK;
}

_int CCustomer1::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    switch (curState)
    {
    case WALK:
        m_fFrame += 8.f * fTimeDelta;
        if (8.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    case MENU:
        m_fFrame = 0.f;
        /*m_fFrame += 1.f * fTimeDelta;
        if (1.f < m_fFrame)
            m_fFrame = 0.f;*/
        break;
    case WAIT:
        m_fFrame += 3.f * fTimeDelta;
        if (3.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    case EAT:
        m_fFrame += 3.f * fTimeDelta;
        if (3.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    case HAPPY:
        m_fFrame += 2.f * fTimeDelta;
        if (2.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    case ANGER:
        m_fFrame += 2.f * fTimeDelta;
        if (2.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    }



    return iExit;
}

void CCustomer1::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Compute_ViewZ(&vPos);
    if (!Sitted) {
        Find_Chair();
    }

    if (Sitted && !ChoosingMenu) {
        _vec3 right = { 1,0,0 };
        m_pTransformCom->Move_Pos(&right, 0.5f, fTimeDelta);
        if (fabsf(vPos.x - targetPos.x) < 0.1f) {
            curState = MENU;
            ChoosingMenu = true;
        }
    }
  /*  if (curDir != prevDir) {
        m_pTransformCom->Rotation(ROT_Y, 180.f);
    }
    prevDir = curDir;*/
}

void CCustomer1::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    switch (curState)
    {
 
    case WALK:
        m_pWalkTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case MENU:
        m_pMenuTextureCom->Set_Texture(0);
        break;
    case WAIT:
        m_pWaitTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case EAT:
        m_pEatTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case HAPPY:
        m_pHappyTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case ANGER:
        m_pAngerTextureCom->Set_Texture((_uint)m_fFrame);
        break;

    }

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

void CCustomer1::Find_Chair()
{

    list<CGameObject*> *Chairs = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjects(L"Chair");
    list<CGameObject*>::iterator iter = Chairs->begin();
    for (iter; iter != Chairs->end(); iter++) {
        if (static_cast<CChair*>(*iter)->isEmtpy()) {
            CTransform* pTransform = static_cast<CTransform*>((*iter)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
            targetPos = pTransform->m_vInfo[INFO_POS];
            static_cast<CChair*>(*iter)->Set_Emtpy(false);
            Sitted = true;
            return ;
        }
    }
    
    

}

HRESULT CCustomer1::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;


    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer1WalkTexture", L"Com_Texture", &m_pWalkTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer1MenuTex", L"Com_Texture", &m_pMenuTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer1WaitTexture", L"Com_Texture", &m_pWaitTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer1EatTexture", L"Com_Texture", &m_pEatTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer1HappyTexture", L"Com_Texture", &m_pHappyTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer1AngerTexture", L"Com_Texture", &m_pAngerTextureCom))))
        return E_FAIL;
    
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 1.f, 1.5f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 8.8f, -2.0f, 0.f };
    return S_OK;
}

CCustomer1* CCustomer1::Create()
{
    CCustomer1* pBackGround = new CCustomer1;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("Dave Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CCustomer1::Free()
{
    CGameObject::Free();
    Safe_Release(m_pWalkTextureCom);
    Safe_Release(m_pMenuTextureCom);
    Safe_Release(m_pWaitTextureCom);
    Safe_Release(m_pEatTextureCom);
    Safe_Release(m_pHappyTextureCom);
    Safe_Release(m_pAngerTextureCom);
}
