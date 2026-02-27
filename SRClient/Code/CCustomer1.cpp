#include "pch.h"
#include "CCustomer1.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CDInputMgr.h"
#include "CMenuBubble.h"
#include "CChair.h"
#include "CAABB.h"
#include "CColliderMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"



CCustomer1::CCustomer1()
    : CGameObject()
{
    curState = WALK;
    curDir = RIGHT;
    prevDir = RIGHT;
    ChairFound = false;
    Sitted = false;
    ChoosingMenu = false;
    gotSushi = false;
    Reacting = false;
    Eating = false;
    deltaTime = 0.f;
    ExitTime = 0.f;
    ReactionTime = 0.f;
    EatingTime = 0.f;
    EmotionTime = 0.f;
    sushiHanded = L"";
    MenuBubble = nullptr;
 
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
    random = rand() % 3;
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fFrame = 0.f;

    m_pTransformCom->m_vScale = { 0.4f,0.9f,1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { -7.f ,-1.84f,-2.999f };
  
    _vec3 vExtents = { 1.0f, 1.0f, 0.01f };
    _vec3 vPos = m_pTransformCom->m_vInfo[INFO_POS];
    m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Customer", this);
  
    return S_OK;
}

_int CCustomer1::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    if (curState == MENU) {
        CColliderMgr::GetInstance()->AddColliderGroup(L"Coll_Customer", m_pAABB);
    }

    _matrix vPos = *m_pTransformCom->Get_World();
    //vPos.m[3][2] = 0;
    //vPos.m[3][0] -= 0.3f;
    m_pAABB->Transform(&vPos);
    switch (curState)
    {
    case WALK:
        m_fFrame += 8.f * fTimeDelta;
        if (8.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    case MENU:
        m_fFrame = 0.f;
        
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
    case LEAVE:
        m_fFrame += 8.f * fTimeDelta;
        if (8.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    }

   
    if (curState == EAT) {
        EatingTime += fTimeDelta;
        if (EatingTime > 6.f) {
            curState = LEAVE;
            m_pTransformCom->Rotation(ROT_Y, 180.f);

        }
    }
    if (curState == LEAVE) {
        m_pTransformCom->m_vInfo[INFO_POS].x -= 0.02f;
        if (m_pTransformCom->m_vInfo[INFO_POS].x <= -8.f) {
            Empty_Chair();
            m_bDead = true;
        }
    }

    if (MenuBubble != nullptr && curState == MENU) {
        if (static_cast<CMenuBubble*>(MenuBubble)->tempY >= 0) {
            MenuBubble->Set_Render(false);
            curState = ANGER;
        }

    }


    return iExit;
}

void CCustomer1::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Compute_ViewZ(&vPos);
    if (!ChairFound) {
        Find_Chair();
    }

    if (!Sitted) {
        _vec3 right = { 1,0,0 };
        m_pTransformCom->Move_Pos(&right, 0.9f, fTimeDelta);
        if (fabsf(vPos.x - targetPos.x) < 0.1f) {
            curState = MENU;
            Sitted = true;
            ChoosingMenu = true;
        }
    }
    if (ChoosingMenu) {
        deltaTime += fTimeDelta;

        if (deltaTime >= 2.5f) {
            MenuBubble = CMenuBubble::Create();
            CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"MenuBubble", MenuBubble);
            CTransform* pTransform = static_cast<CTransform*> (MenuBubble->Get_Component(ID_DYNAMIC, L"Com_Transform"));
            pTransform->m_vInfo[INFO_POS] = m_pTransformCom->m_vInfo[INFO_POS];
            pTransform->m_vInfo[INFO_POS].z -= 0.01f;
            pTransform->m_vInfo[INFO_POS].y += 1.4f;
            ChoosingMenu = false;
        }
    }
    if (gotSushi) {
        if (sushiHanded == static_cast<CMenuBubble*>(MenuBubble)->m_sFishName) {
            curState = HAPPY;
            MenuBubble->Set_Render(false);

        }
        else {
            curState = ANGER;
            MenuBubble->Set_Render(false);
        }
        ReactionTime += fTimeDelta;
        if (ReactionTime > 2.f) {
            if (curState == HAPPY) {
                curState = EAT;
            }
            else if (curState = ANGER) {
                curState = LEAVE;
                m_pTransformCom->Rotation(ROT_Y, 180.f);
            }
            gotSushi = false;
        }
    }
    else {
        if (curState == ANGER) {
            ReactionTime += fTimeDelta;
            if (ReactionTime > 2.f) {
                m_pTransformCom->Rotation(ROT_Y, 180.f);
                curState = LEAVE;
            }
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
    case LEAVE:
        m_pWalkTextureCom->Set_Texture((_uint)m_fFrame);
        
        break;

    }

    m_pBufferCom->Render_Buffer();

    if (MenuBubble == nullptr && ChoosingMenu) {
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_MenuChoosing"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }

        _matrix scaleMat = *m_pTransformCom->Get_World();
        scaleMat.m[0][0] = 0.3f;
        scaleMat.m[1][1] = 0.3f;
        scaleMat.m[3][1] += 1.3f;
        scaleMat.m[3][0] -= 0.2f;

        pGraphicDev->SetTransform(D3DTS_WORLD, &scaleMat);
        m_pBufferCom->Render_Buffer();
    }
    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    if (curState == HAPPY) {
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Happy"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }
        _matrix scaleMat = *m_pTransformCom->Get_World();
        EmotionTime += 0.01f;
        scaleMat.m[0][0] = 0.3f;
        scaleMat.m[1][1] = 0.3f;
        scaleMat.m[3][1] = 0.2f + EmotionTime;

        pGraphicDev->SetTransform(D3DTS_WORLD, &scaleMat);
        m_pBufferCom->Render_Buffer();
    }
    else if (curState == ANGER) {
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Angry"))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }
        _matrix scaleMat = *m_pTransformCom->Get_World();
        EmotionTime += 0.01f;
        scaleMat.m[0][0] = 0.3f;
        scaleMat.m[1][1] = 0.3f;
        scaleMat.m[3][1] = 0.2f +EmotionTime;


        pGraphicDev->SetTransform(D3DTS_WORLD, &scaleMat);
        m_pBufferCom->Render_Buffer();
    }
   
}

void CCustomer1::Find_Chair()
{

    list<CGameObject*> *Chairs = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjects(L"Chair");
    list<CGameObject*>::iterator iter = Chairs->begin();
    for (iter; iter != Chairs->end(); iter++) {
        if (static_cast<CChair*>(*iter)->isEmtpy()) {
            CTransform* pTransform = static_cast<CTransform*>((*iter)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
            targetPos = pTransform->m_vInfo[INFO_POS];
            targetPos.x +=  0.05f;
            static_cast<CChair*>(*iter)->Set_Emtpy(false);
            ChairFound = true;
            return ;
        }
    }
    
    

}

void CCustomer1::Empty_Chair()
{
    list<CGameObject*>* Chairs = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjects(L"Chair");
    list<CGameObject*>::iterator iter = Chairs->begin();
    for (iter; iter != Chairs->end(); iter++) {
        CTransform* pTransform = static_cast<CTransform*>((*iter)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        _vec3 chair = pTransform->m_vInfo[INFO_POS];
        chair.x += 0.05f;
        if (targetPos == chair) {
            static_cast<CChair*>(*iter)->Set_Emtpy(true);
            return;

        }
    }
}



HRESULT CCustomer1::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    switch (random) {
    case 0:
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer1WalkTexture", L"Com_Texture", &m_pWalkTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer1MenuTex", L"Com_Texture1", &m_pMenuTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer1WaitTexture", L"Com_Texture2", &m_pWaitTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer1EatTexture", L"Com_Texture3", &m_pEatTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer1HappyTexture", L"Com_Texture4", &m_pHappyTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer1AngerTexture", L"Com_Texture5", &m_pAngerTextureCom))))
            return E_FAIL;
        break;
    case 1:
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer2WalkTexture", L"Com_Texture", &m_pWalkTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer2MenuTex", L"Com_Texture1", &m_pMenuTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer2WaitTexture", L"Com_Texture2", &m_pWaitTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer2EatTexture", L"Com_Texture3", &m_pEatTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer2HappyTexture", L"Com_Texture4", &m_pHappyTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer2AngerTexture", L"Com_Texture5", &m_pAngerTextureCom))))
            return E_FAIL;
        break;
    case 2:
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer3WalkTexture", L"Com_Texture", &m_pWalkTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer3MenuTex", L"Com_Texture1", &m_pMenuTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer3WaitTexture", L"Com_Texture2", &m_pWaitTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer3EatTexture", L"Com_Texture3", &m_pEatTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer3HappyTexture", L"Com_Texture4", &m_pHappyTextureCom))))
            return E_FAIL;
        if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_Customer3AngerTexture", L"Com_Texture5", &m_pAngerTextureCom))))
            return E_FAIL;
        break;
    }
    
    
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
        MSG_BOX("Customer1 Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CCustomer1::Free()
{
    Safe_Release(m_pAABB);
    Safe_Release(MenuBubble);
    CGameObject::Free();
 
}
