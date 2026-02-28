#include "pch.h"
#include "CRecipe.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CSushiFrame.h"
#include "CColliderMgr.h"
#include "CBluejongR.h"
#include "CCloseButton.h"
#include "CUpgradeImage.h"



CRecipe::CRecipe()
    : CGameObject()
{

}

CRecipe::CRecipe(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CRecipe::~CRecipe()
{
}

HRESULT CRecipe::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    int col = 0;
    int row = 0;
    CGameObject* pGameObject = nullptr;
    for (int i = 0; i < 5; i++) {
        //pGameObject = CSushiFrame::Create(L"블루");
        switch (i) {
        case 0:
            pGameObject = CSushiFrame::Create(L"블루종");
            break;
        case 1:
            pGameObject = CSushiFrame::Create(L"노랑백");
            break;

        case 2:
            pGameObject = CSushiFrame::Create(L"코반아지");
            break;
        case 3:
            pGameObject = CSushiFrame::Create(L"흰동가리");
            break;
        case 4:
            pGameObject = CSushiFrame::Create(L"노랑탕");
            break;
        }
      

        if (nullptr == pGameObject)
            return E_FAIL;

        if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"SushiFrame", pGameObject)))
            return E_FAIL;
        objects.push_back(pGameObject);

        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vScale = { 0.3f, 0.3f, 0.1f };
        if (i % 4 == 0) {
            row ++;
            col = 0;
        }
        pTransform->m_vInfo[INFO_POS] = { -0.98f + (0.65f*col),(2.8f - 0.8f* row),-5.0f};
        col++;
    }
   

    pGameObject = CCloseButton::Create();

    if (nullptr == pGameObject)
        return E_FAIL;

    if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Close_2", pGameObject)))
        return E_FAIL;
    static_cast<CCloseButton*>(pGameObject)->Set_Frame(L"RecipeFrame");
    {
        CTransform* pTransform = static_cast<CTransform*>(pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { 0.f,-2.35f,-5.f };
        pTransform->m_vScale = { 0.55f,0.15f,0.01f };
    }
    objects.push_back(pGameObject);

 
    return S_OK;
}

_int CRecipe::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
        
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    return iExit;
}

void CRecipe::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (m_bRender) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }

}

void CRecipe::Render_GameObject()
{
    if (m_bRender) {

        list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
        list<CGameObject*>::iterator iter = frame->begin();
        for (iter; iter != frame->end(); iter++) {
            (*iter)->Set_Render(true);

        }

        list<CGameObject*>* sushi = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"sushi");
        list<CGameObject*>::iterator iter1 = sushi->begin();
        for (iter1; iter1 != sushi->end(); iter1++) {
            (*iter1)->Set_Render(true);
        }
        CGameObject* fishframe = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"FishCell");
        fishframe->Set_Render(true);
  
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
        m_pRecipeTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


    }
    else {
        list<CGameObject*>* frame = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"SushiFrame");
        list<CGameObject*>::iterator iter = frame->begin();
        for (iter; iter != frame->end(); iter++) {
            (*iter)->Set_Render(false);
        }
        list<CGameObject*>* sushi = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"sushi");
        list<CGameObject*>::iterator iter1 = sushi->begin();
        for (iter1; iter1 != sushi->end(); iter1++) {
            (*iter1)->Set_Render(false);
        }
        CGameObject* fishframe = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjectFirst(L"FishCell");
        fishframe->Set_Render(false);
    }

}

HRESULT CRecipe::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_RecipeTex", L"Com_Texture", &m_pRecipeTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 1.5f, 3.0f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 0.f,0.f,-4.5f };
    return S_OK;
}


CRecipe* CRecipe::Create()
{
    CRecipe* recipe = new CRecipe;

    if (FAILED(recipe->Ready_GameObject()))
    {
        Safe_Release(recipe);
        MSG_BOX("recipe Create Failed");
        return nullptr;
    }

    return recipe;
}

void CRecipe::Free()
{
    CGameObject::Free();
}
