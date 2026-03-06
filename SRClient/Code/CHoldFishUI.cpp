#include "pch.h"
#include "CHoldFishUI.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CHoldFishUIItemArea.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"
#include"CHoldFishUIDropPanel.h"
#include "CHoldFishUIItemGroup.h"

#include "CHoldFishUIImg.h"
#include "CGameMemMgr.h"
#include "CDiveDave.h"
#include "CSoundMgr.h"
#include "CDiveDaveGun.h"

CHoldFishUI::CHoldFishUI(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CHoldFishUI::~CHoldFishUI()
{
}


HRESULT		CHoldFishUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_HoldFishUI"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
            vScale = { fAspect, 1.f, 1.f };
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, -8.3f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fDbgX = 0.f;
    m_fDbgY = 0.f;
    
    m_fViewZ = 0.499;


    m_pDaveTextureFrameCom->RegistTextureFrame(L"Default", L"Tex_Ship_DiverBoxDave", 0.2f);
    m_pDaveTextureFrameCom->Set_TextureFrame(L"Default");
    
    if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
    {
        //float refY = 0.5f;
        //{
        //    auto pArea = CHoldFishUIItemArea::Create(0, 0.925f, refY);
        //    pArea->Set_Parent(this);
        //    pLayer->Add_GameObject(L"HoldFishUIItemArea", pArea);
        //}
        //refY -= 0.15f;
        //{
        //    auto pArea = CHoldFishUIItemArea::Create(1, 0.925f, refY);
        //    pArea->Set_Parent(this);
        //    pLayer->Add_GameObject(L"HoldFishUIItemArea", pArea);
        //}
        //refY -= 0.15f;
        //{
        //    auto pArea = CHoldFishUIItemArea::Create(2, 0.925f, refY);
        //    pArea->Set_Parent(this);
        //    pLayer->Add_GameObject(L"HoldFishUIItemArea", pArea);
        //}


        {
            auto pMask = CHoldFishUIImg::Create(0.925f, -0.17f);
            pMask->Set_Render(false);
            pMask->Set_Scale(0.745f);
            pMask->Set_ViewZ(0.49f);
            pMask->Set_AssetName(L"Tex_HoldFishUIMask");
            pMask->Set_Parent(this);
            pMask->Ready_After_Create();
            pLayer->Add_GameObject(L"HoldFishUIMask", pMask);
        }

        {
            //CHoldFishUIItemGroup
            auto pGroup = CHoldFishUIItemGroup::Create(0.925f, 0.f);
            pGroup->Set_Parent(this);
            pLayer->Add_GameObject(L"HoldFishUIItemAreaGroup", pGroup);
        }


        // 작살총
        {
            auto pJacksalChock = CHoldFishUIImg::Create(-1.07f, 0.62f);
            pJacksalChock->Set_Scale(0.040);
            pJacksalChock->Set_ViewZ(0.49f);
            pJacksalChock->Set_AssetName(L"Tex_Ship_InventoryUpperItem_Jaksal");
            pJacksalChock->Set_Parent(this);
            pJacksalChock->Ready_After_Create();
            pLayer->Add_GameObject(L"HoldFishUIJacksalChong", pJacksalChock);
        }

        // 작살촉
        {
            auto pJacksalChong = CHoldFishUIImg::Create(-0.6f, 0.62f);
            pJacksalChong->Set_Scale(0.040);
            pJacksalChong->Set_ViewZ(0.49f);
            pJacksalChong->Set_AssetName(L"Tex_FishUIJaksalChock");
            pJacksalChong->Set_Parent(this);
            pJacksalChong->Ready_After_Create();
            pLayer->Add_GameObject(L"HoldFishUIJacksalChong", pJacksalChong);
        }


        // 근접무기
        {
            auto pKnief = CHoldFishUIImg::Create(-0.14f, 0.62f);
            pKnief->Set_Scale(0.040);
            pKnief->Set_ViewZ(0.49f);
            pKnief->Set_AssetName(L"Tex_Ship_InventoryUpperItem_Knief");
            pKnief->Set_Parent(this);
            pKnief->Ready_After_Create();
            pLayer->Add_GameObject(L"HoldFishUIKnief", pKnief);
        }

        // 총
        {

            std::wstring sAssetName = L"Tex_Ship_InventoryUpperItem_Gun";
            if (auto pGun = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst<CDiveDaveGun>(L"DiveDaveGun"))
            {
                switch (pGun->Get_eGun())
                {
                case CGameMemMgr::CDaveInfo::GUN_DEFAULT:
                    break;
                case CGameMemMgr::CDaveInfo::GUN_PENTA_ACCEL:
                    sAssetName = L"Tex_UI_Gun_Penta_Accel";
                    break;
                case CGameMemMgr::CDaveInfo::GUN_TRIPLE_ACCEL:
                    sAssetName = L"Tex_UI_Gun_Triple_Accel";
                    break;
                }
            }

            //auto gunAssetAnem = CGameMemMgr::GetInstance()->Get_DaveInfo().Get_Inventory()[33].sAssetName;

            auto pGun = CHoldFishUIImg::Create(-1.07f, 0.43f);
            pGun->Set_Scale(0.040);
            pGun->Set_ViewZ(0.49f);
            pGun->Set_AssetName(sAssetName);//L"Tex_Ship_InventoryUpperItem_Gun"
            pGun->Set_Parent(this);
            pGun->Ready_After_Create();
            pLayer->Add_GameObject(L"HoldFishUIGun", pGun);
        }

        // 부적1
        {
            auto pBujeok = CHoldFishUIImg::Create(-0.72f, 0.43f);
            pBujeok->Set_Scale(0.040);
            pBujeok->Set_ViewZ(0.49f);
            pBujeok->Set_AssetName(L"Tex_Ship_InventoryUpperItem_Jusin");
            pBujeok->Set_Parent(this);
            pBujeok->Ready_After_Create();
            pLayer->Add_GameObject(L"HoldFishUIBujeok1", pBujeok);
        }

        // 부적2
        {
            auto pBujeok = CHoldFishUIImg::Create(-0.5f, 0.43f);
            pBujeok->Set_Scale(0.040);
            pBujeok->Set_ViewZ(0.49f);
            pBujeok->Set_AssetName(L"Tex_Ship_InventoryUpperItem_Jusin");
            pBujeok->Set_Parent(this);
            pBujeok->Ready_After_Create();
            pLayer->Add_GameObject(L"HoldFishUIBujeok2", pBujeok);
        }

       
    }

    CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Ship_ui_button_click", CSoundMgr::SFX, 1.f);
    return S_OK;
}

_int		CHoldFishUI::Update_GameObject(const _float& fTimeDelta)
{
    if (CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
    {
        if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
        {
            if (auto pObjs = pLayer->Get_GameObjects(L"HoldFishUIItemArea"))
            {
                bool bReSorting = false;
                int idx = 0;
                auto iter = CGameMemMgr::GetInstance()->Get_DiveInfos().back().Get_Fishes().begin();
                for (auto& pObj : *pObjs)
                {
                    if (auto pArea = dynamic_cast<CHoldFishUIItemArea*>(pObj))
                    {
                        if (pArea->Get_EdgeVisible())
                        {
                            if (auto pPanel = pLayer->Get_GameObjectFirst(L"HoldFishUIDropPanel"))
                            {
                                CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Ship_ui_button_click", CSoundMgr::SFX, 1.f);
                                pArea->Set_DeadCascade();
                                pPanel->Set_DeadCascade();
                                bReSorting = true;

                                if (auto pDave = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst<CDiveDave>(L"DiveDave"))
                                {
                                    pDave->Change_Weight(-iter->fWeight);
                                }

                                iter = CGameMemMgr::GetInstance()->Get_DiveInfos().back().Get_Fishes().erase(iter);
                                
                                break;
                            }
                            else
                            {
                                auto pArea = CHoldFishUIDropPanel::Create(0.f, 0.f);
                                pArea->Set_Parent(this);
                                pArea->Set_Title(iter->sFishName);
                                pLayer->Add_GameObject(L"HoldFishUIDropPanel", pArea);
                            }
                        }
                    }
                    ++iter;
                    ++idx;
                }

                if (bReSorting)
                {
                    vector< CHoldFishUIItemArea*> vArea;
                    for (auto& pObj : *pObjs)
                    {
                        if (!pObj->Get_Dead())
                        {
                            if (auto pArea = dynamic_cast<CHoldFishUIItemArea*>(pObj))
                            {
                                vArea.push_back(pArea);
                            }
                        }
                    }

                    float refY = 0.5f;
                    for (auto& p : vArea)
                    {
                        p->Set_PosY(refY);
                        refY -= 0.15;
                    }
                }
            }
        }
    }

    if (CDInputMgr::GetInstance()->Key_Down(DIK_C))
    {
        if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"2_Fish_Layer"))
        {
            if (auto pPanel = pLayer->Get_GameObjectFirst(L"HoldFishUIDropPanel"))
            {
                pPanel->Set_DeadCascade();
            }
        }
    }


    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    _matrix matView, matInvView;
    pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
    D3DXMatrixInverse(&matInvView, 0, &matView);

    float x = matInvView.m[3][0];
    float y = matInvView.m[3][1];


    m_pTransformCom->Set_Pos(x + m_fPosX, y + m_fPosY, -8.3f);


    m_pUIDaveTransformCom->Set_Pos(x + -8.78f, y + 3.2f, 0.f);

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA_AFTER_ORTHO_UI, this);

    return iExit;
}

void		CHoldFishUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void		CHoldFishUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_HoldFishUI"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    m_pBufferCom->Render_Buffer();

    CTextureFrame::FRAME_INFO fInfo;
    if (m_pDaveTextureFrameCom->Get_CurrentInfo(fInfo))
    {
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(fInfo.sAssetTextureName))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(fInfo.iFrame)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }
    }
    pGraphicDev->SetTransform(D3DTS_WORLD, m_pUIDaveTransformCom->Get_World());
    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


     // 키로수
    {
        float fKg = 0.f;
        float fMaxKg = CGameMemMgr::GetInstance()->Get_DaveInfo().Get_JeokjaeWeight();

        // TODO: 피시뿐 아니라 아이템도
        for (auto& pFish : CGameMemMgr::GetInstance()->Get_DiveInfos().back().Get_Fishes())
        {
            fKg += pFish.fWeight;
        }
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.380f;
        float fOffsetY = 0.63f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            std::wstringstream wss;
            wss << std::fixed << std::setprecision(1) << fKg << L"/" << fMaxKg << L"Kg";
            std::wstring result = wss.str();
            pDefFont->Render_Font(result, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

}

void CHoldFishUI::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
}

HRESULT			CHoldFishUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    // 데이브 버퍼
    //if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer_UIDave", &m_pDaveBufferCom))))
    //    return E_FAIL;

    // 데이브 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform_UIDave", &m_pUIDaveTransformCom))))
        return E_FAIL;

    // 데이브 텍스쳐 프레임
    if (FAILED((AddComponent<Engine::CTextureFrame, ID_DYNAMIC>(L"Proto_TextureFrame", L"Com_TextureFrame_UIDave", &m_pDaveTextureFrameCom))))
        return E_FAIL;
}


CHoldFishUI* CHoldFishUI::Create(float fPosX, float fPosY)
{
    CHoldFishUI* pHoldFishUI = new CHoldFishUI{ fPosX , fPosY };

    if (FAILED(pHoldFishUI->Ready_GameObject()))
    {
        Safe_Release(pHoldFishUI);
        MSG_BOX("pHoldFishUI Create Failed");
        return nullptr;
    }

    return pHoldFishUI;
}

void CHoldFishUI::Free()
{
    CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Ship_ui_button_click", CSoundMgr::SFX, 1.f);
    CGameObject::Free();
}