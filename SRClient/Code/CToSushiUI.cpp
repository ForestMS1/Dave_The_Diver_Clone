#include "pch.h"
#include "CToSushiUI.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"

#include "CToSushiUIItem.h"

#include "CGameMemMgr.h"

#include "CGoToSushiUI.h"

#include "CBanchoGood.h"
#include "CDaveConversation.h"

#include "CSoundMgr.h"

CToSushiUI::CToSushiUI(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CToSushiUI::~CToSushiUI()
{
}


HRESULT		CToSushiUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_ToSushiUI"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
            vScale = { fAspect, 1.f, 1.f };

            float fScale = 5.f;
            vScale.x *= fScale;
            vScale.y *= fScale;
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fDbgX = 0.f;
    m_fDbgY = 0.f;

    m_fViewZ = 0.9f;

    m_bOpenTween = true;
    m_tweenOpen = m_tweenOpen.from(-10.f).to(0.f).during(200);
    m_pTransformCom->Set_Pos(0.f, -10.f, 0.f);
    m_pTransformCom->Update_Component(0.f);

    if (auto pLayer = CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer"))
    {
        if (!CGameMemMgr::GetInstance()->Get_DiveInfos().empty())
        {
            auto originalFishes = CGameMemMgr::GetInstance()->Get_DiveInfos().back().Get_Fishes();
            map<std::wstring, CGameMemMgr::CDiveInfo::DIVE_FISH> mapFish;

            for (auto& fish : originalFishes)
            {
                if (fish.bFish)
                {
                    auto iter = mapFish.find(fish.sFishName);
                    if (iter == mapFish.end())
                    {
                        mapFish.insert({ fish.sFishName , fish });
                    }
                    else
                    {
                        mapFish[fish.sFishName].iMeatCnt += fish.iMeatCnt;
                    }
                }
            }


            float fRefY = 3.16f;

            int i = 0;

            //정체불명의 고기

            for(auto iter = mapFish.begin(); iter != mapFish.end();)
            {
                if (iter->first == L"정체불명의 고기")
                {
					auto pCaughtFishes = *iter;

                    auto pToSushiItem = CToSushiUIItem::Create(-2.73f, fRefY);
                    pToSushiItem->Set_Parent(this);
                    pToSushiItem->Set_Title(pCaughtFishes.first);
                    pToSushiItem->Set_Cnt(std::to_wstring(pCaughtFishes.second.iMeatCnt));
                    pToSushiItem->Set_Money(::to_wstring(pCaughtFishes.second.iSushiMoney));
                    pToSushiItem->Set_Lv(L"Lv " + ::to_wstring(pCaughtFishes.second.iSushiLv));
                    pToSushiItem->Set_FishImgAssetName(pCaughtFishes.second.sThumbNailAssetName);
                    pToSushiItem->Set_SushiImgAssetName(pCaughtFishes.second.sSushiThumbNailAssetName);
                    pToSushiItem->Set_Rank(L"Rank" + to_wstring(pCaughtFishes.second.iRank));
                    pToSushiItem->Ready_AfterCreate();
                    pLayer->Add_GameObject(L"pToSushiItem", pToSushiItem);

                    fRefY -= 1.16;

                    iter = mapFish.erase(iter);

                    i = 1;
                    break;
                }
                else
                {
					++iter;
                }

			}



            for (auto& pCaughtFishes : mapFish)
            {
                if (i > 5) break;

                auto pToSushiItem = CToSushiUIItem::Create(-2.73f, fRefY);
                pToSushiItem->Set_Parent(this);
                pToSushiItem->Set_Title(pCaughtFishes.first);
                pToSushiItem->Set_Cnt(std::to_wstring(pCaughtFishes.second.iMeatCnt));
                pToSushiItem->Set_Money(::to_wstring(pCaughtFishes.second.iSushiMoney));
                pToSushiItem->Set_Lv(L"Lv " + ::to_wstring(pCaughtFishes.second.iSushiLv));
                pToSushiItem->Set_FishImgAssetName(pCaughtFishes.second.sThumbNailAssetName);
                pToSushiItem->Set_SushiImgAssetName(pCaughtFishes.second.sSushiThumbNailAssetName);
                pToSushiItem->Set_Rank(L"Rank" + to_wstring(pCaughtFishes.second.iRank));
                pToSushiItem->Ready_AfterCreate();
                pLayer->Add_GameObject(L"pToSushiItem", pToSushiItem);

                fRefY -= 1.16;

                ++i;
            }
        }
       

        //{
        //    auto pToSushiItem = CToSushiUIItem::Create(-2.73f, 3.16f);
        //    pToSushiItem->Set_Parent(this);
        //    pLayer->Add_GameObject(L"pToSushiItem", pToSushiItem);
        //}

        //{
        //    auto pToSushiItem = CToSushiUIItem::Create(-2.73f, 2.f);
        //    pToSushiItem->Set_Parent(this);
        //    pLayer->Add_GameObject(L"pToSushiItem", pToSushiItem);
        //}
    }

    CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Ship_ui_button_choice", CSoundMgr::SFX_SHIP_UI_CHOICE, 1.f);

    return S_OK;
}

HRESULT CToSushiUI::Ready_AfterCreate()
{
    return S_OK;
}

_int		CToSushiUI::Update_GameObject(const _float& fTimeDelta)
{
    if (CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
    {
        if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer"))
        {
            if (auto pObj = pLayer->Get_GameObjectFirst(L"BanchoGood"))
            {
                pObj->Set_DeadCascade();
            }
            else
            {
                if (!CGameMemMgr::GetInstance()->Get_DiveInfos().empty())
                {
                    if (!CGameMemMgr::GetInstance()->Get_DiveInfos().back().Get_Fishes().empty())
                    {
                        CBanchoGood* pBanchoGood = CBanchoGood::Create(5.5f, -1.f);
                        pLayer->Add_GameObject(L"BanchoGood", pBanchoGood);
                    }
                }
                
            }
        }

        Set_DeadCascade();



        if (CGameMemMgr::GetInstance()->Get_DiveInfos().size() == 0 + 1 ) {
            if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer"))
            {
                if (auto pObj = pLayer->Get_GameObjectFirst(L"DaveConversation"))
                {
                    pObj->Set_DeadCascade();
                }
                else
                {
                    CDaveConversation* pDaveConversation = CDaveConversation::Create(0.f, -2.f);
                    pDaveConversation->SetCurrentConversation(CDaveConversation::CONVERSATION::CONV_1);
                    pLayer->Add_GameObject(L"DaveConversation", pDaveConversation);
                }
            }
        }
        if (CGameMemMgr::GetInstance()->Get_DiveInfos().size() == 1+1) {
            if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer"))
            {
                if (auto pObj = pLayer->Get_GameObjectFirst(L"DaveConversation"))
                {
                    pObj->Set_DeadCascade();
                }
                else
                {
                    CDaveConversation* pDaveConversation = CDaveConversation::Create(0.f, -2.f);
                    pDaveConversation->SetCurrentConversation(CDaveConversation::CONVERSATION::CONV_2);
                    pLayer->Add_GameObject(L"DaveConversation", pDaveConversation);
                }
            }
        }
        else if (CGameMemMgr::GetInstance()->Get_DiveInfos().size() == 2+1) {
            if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer"))
            {
               if (auto pObj = pLayer->Get_GameObjectFirst(L"DaveConversation"))
               {
                   pObj->Set_DeadCascade();
               }
               else
               {
                   CDaveConversation* pDaveConversation = CDaveConversation::Create(0.f, -2.f);
                   pDaveConversation->SetCurrentConversation(CDaveConversation::CONVERSATION::CONV_4);
                   pLayer->Add_GameObject(L"DaveConversation", pDaveConversation);
               }
            }
        }
    }

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (m_bOpenTween)
    {
        // PosY, RotX, RotY
        auto val = m_tweenOpen.step(int(fTimeDelta * 1000.f));
        _vec3 dir = { 0.f, 1.f, 0.f };
        _vec3 vPos;

        m_pTransformCom->Get_Info(INFO_POS, &vPos);
        m_pTransformCom->Set_Pos(vPos.x, val, vPos.z);

        if (m_tweenOpen.progress() >= 1.0f) {
            m_bOpenTween = false;
            m_bOpen = true;
        }
    }

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CToSushiUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CToSushiUI::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_ToSushiUI"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }
    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());
    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);


     // 식재료
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 3.37f;
        float fOffsetY = 0.f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL_Size15"))
        {
            pDefFont->Render_Font(L"획득한 식재료가 없습니다.", &vPos, D3DXCOLOR(0.345f, 0.682f, 0.792f, 1.0f), (DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_NOCLIP));
        }
    }

}

void CToSushiUI::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
}

HRESULT			CToSushiUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

}


CToSushiUI* CToSushiUI::Create(float fPosX, float fPosY)
{
    CToSushiUI* pIDiverUpgrade = new CToSushiUI{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CToSushiUI::Free()
{
    CGameObject::Free();
}