#include "pch.h"
#include "CMenuFrame.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CAddMenuButton.h"
#include "CDInputMgr.h"
#include "CConfirm.h"
#include "CRecipe.h"
#include "CSushiList.h"
CMenuFrame::CMenuFrame()
    : CGameObject()
{
    buttonCreated = false;
    addButtons.resize(0);
    curButton = 0;
    render = false;
    frameMoved = false;
}

CMenuFrame::CMenuFrame(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CMenuFrame::~CMenuFrame()
{
}

HRESULT CMenuFrame::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;
    


    return S_OK;
}

_int CMenuFrame::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

 
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_UI, this);

    if (!buttonCreated) {
        for (int i = 0; i < 4; i++) {
            CGameObject* pGameObject = CAddMenuButton::Create();

            if (nullptr == pGameObject)
                return E_FAIL;

            if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"AddButton", pGameObject)))
                return E_FAIL;
            CTransform* pTransform = static_cast<CTransform*> (pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
            pTransform->m_vInfo[INFO_POS] = { 0.f, (1.0f - (0.8f * i)), -5.f };
            addButtons.push_back(pGameObject);
        }

        CGameObject* pGameObject = CConfirm::Create();

        if (nullptr == pGameObject)
            return E_FAIL;

        if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"ConfirmButton", pGameObject)))
            return E_FAIL;
        CTransform* pTransform = static_cast<CTransform*> (pGameObject->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        pTransform->m_vInfo[INFO_POS] = { 2.2f, 1.0f , -5.f };
        buttonCreated = true;

       pGameObject = CRecipe::Create();

        if (nullptr == pGameObject)
            return E_FAIL;

        if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"Recipe", pGameObject)))
            return E_FAIL;

        pGameObject = CSushiList::Create();

        if (nullptr == pGameObject)
            return E_FAIL;

        if (FAILED(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Add_GameObject(L"List", pGameObject)))
            return E_FAIL;
    }
    if (render) {

        static_cast<CAddMenuButton*>(addButtons[curButton])->Set_Selected(true);
        Key_Input();
    }
    return iExit;
}

void CMenuFrame::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (render) {
        CGameObject::LateUpdate_GameObject(fTimeDelta);

        _vec3		vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        Compute_ViewZ(&vPos);
    }
   
}

void CMenuFrame::Render_GameObject()
{
    if (render) {
        LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

        m_pMenuTextureCom->Set_Texture(0);
        //m_pConfirmTextureCom->Set_Texture(0);
        m_pBufferCom->Render_Buffer();

        D3DXMATRIX matTmp;
        D3DXMatrixIdentity(&matTmp);
        pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

        //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
        pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    }
   
}

void CMenuFrame::Hide()
{
    render = false;

     list<CGameObject*>* button = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"ConfirmButton");
        list<CGameObject*>::iterator iter = button->begin();
        for (iter; iter != button->end(); iter++) {
            static_cast<CConfirm*>(*iter)->render = false;
        }

        list<CGameObject*>* AddButtons = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"AddButton");
        list<CGameObject*>::iterator iter1 = AddButtons->begin();
        for (iter1; iter1 != AddButtons->end(); iter1++) {
            static_cast<CAddMenuButton*>(*iter1)->render = false;
        } 
}

void CMenuFrame::Show()
{
    render = true;

    list<CGameObject*>* button = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"ConfirmButton");
    list<CGameObject*>::iterator iter = button->begin();
    for (iter; iter != button->end(); iter++) {
        static_cast<CConfirm*>(*iter)->render = true;
    }

    list<CGameObject*>* AddButtons = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"AddButton");
    list<CGameObject*>::iterator iter1 = AddButtons->begin();
    for (iter1; iter1 != AddButtons->end(); iter1++) {
        static_cast<CAddMenuButton*>(*iter1)->render = true;
    }
}

HRESULT CMenuFrame::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_MenuFrameTex", L"Com_Texture", &m_pMenuTextureCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
    m_pTransformCom->m_vScale = { 2.f, 3.f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 0,0,-4.5f };
    return S_OK;
}

void CMenuFrame::Key_Input()
{
    if (CDInputMgr::GetInstance()->Key_Up(DIKEYBOARD_W))
    {
        if (!frameMoved) {
            if (curButton == 0) {
                return;
            }
            static_cast<CAddMenuButton*>(addButtons[curButton])->Set_Selected(false);
            curButton--;
            list<CGameObject*>* button = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"ConfirmButton");
            list<CGameObject*>::iterator iter = button->begin();
            for (iter; iter != button->end(); iter++) {
                CTransform* pTransform = static_cast<CTransform*>((*iter)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
                _vec3 curPos;
                curPos = pTransform->m_vInfo[INFO_POS];
                curPos.y = curPos.y + 0.8f;
                pTransform->m_vInfo[INFO_POS] = curPos;
                return;
            }
        }
       
       // static_cast<CAddMenuButton*>(addButtons[curButton])->Set_Selected(true);
    }
    if (CDInputMgr::GetInstance()->Key_Up(DIKEYBOARD_S))
    {
        if (!frameMoved) {
            if (curButton == addButtons.size() - 1) {
                return;
            }
            else {
                static_cast<CAddMenuButton*>(addButtons[curButton])->Set_Selected(false);
                curButton++;
                list<CGameObject*>* button = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"ConfirmButton");
                list<CGameObject*>::iterator iter = button->begin();
                for (iter; iter != button->end(); iter++) {
                    CTransform* pTransform = static_cast<CTransform*>((*iter)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
                    _vec3 curPos;
                    curPos = pTransform->m_vInfo[INFO_POS];
                    curPos.y = curPos.y - 0.8f;
                    pTransform->m_vInfo[INFO_POS] = curPos;
                    return;
                }
        }
       
           // static_cast<CAddMenuButton*>(addButtons[curButton])->Set_Selected(true);
        }
    
    }
    if (CDInputMgr::GetInstance()->Key_Up(DIKEYBOARD_SPACE))
    {
        if(!frameMoved)
            Move_Frame();
    }
}

void CMenuFrame::Move_Frame()
{
  

    list<CGameObject*>* button = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"ConfirmButton");
    list<CGameObject*>::iterator iter = button->begin();
    for (iter; iter != button->end(); iter++) {
        static_cast<CConfirm*>(*iter)->render = false;
    }

    _vec3 pos = m_pTransformCom->m_vInfo[INFO_POS];
    pos.x = pos.x - 3.5f;
    m_pTransformCom->m_vInfo[INFO_POS] = pos;

    list<CGameObject*>* AddButtons = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"AddButton");
    list<CGameObject*>::iterator iter1 = AddButtons->begin();
    for (iter1; iter1 != AddButtons->end(); iter1++) {
        CTransform* pTransform = static_cast<CTransform*>((*iter1)->Get_Component(ID_DYNAMIC, L"Com_Transform"));
        _vec3 curPos;
        curPos = pTransform->m_vInfo[INFO_POS];
        curPos.x = curPos.x - 3.2f;
        pTransform->m_vInfo[INFO_POS] = curPos;
    }
    list<CGameObject*>* recipe = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"Recipe");
    list<CGameObject*>::iterator iter2 = recipe->begin();
    for (iter2; iter2 != recipe->end(); iter2++) {
        static_cast<CConfirm*>(*iter2)->render = true;
    }

    list<CGameObject*>* List = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"UI_Layer")->Get_GameObjects(L"List");
    list<CGameObject*>::iterator iter3 = List->begin();
    for (iter3; iter3 != List->end(); iter3++) {
        static_cast<CConfirm*>(*iter3)->render = true;
    }
    frameMoved = true;
}


CMenuFrame* CMenuFrame::Create()
{
    CMenuFrame* pBackGround = new CMenuFrame;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("Open Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CMenuFrame::Free()
{
    CGameObject::Free();
    for_each(addButtons.begin(), addButtons.end(), Safe_Release<CGameObject*>);
    addButtons.clear();
    Safe_Release(m_pMenuTextureCom);
    
}
