#include "pch.h"
#include "CShipDiverBoxInventory.h"
#include "CGraphicDev.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CDInputMgr.h"
#include "CShipDiverBoxDave.h"
#include "CManagement.h"
#include "CShipDiverBoxInventoryEdge.h"
#include "CShipDiverBoxInventoryItemArea.h"
#include "CColliderMgr.h"
#include "CShipDiverBoxInventoryItemImg.h"
#include "CShipDiverBoxinventoryDesc.h"
#include "CSoundMgr.h"

CShipDiverBoxInventory::CShipDiverBoxInventory()
    : CGameObject()
{
}

CShipDiverBoxInventory::CShipDiverBoxInventory(const CShipDiverBoxInventory& rhs)
    : CGameObject(rhs)
{
}

CShipDiverBoxInventory::~CShipDiverBoxInventory()
{
}


HRESULT		CShipDiverBoxInventory::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };

    _vec3 vPos = { 00.0f, 0.0f, 0.0f };
    _vec3 v = { 5.f , 5.f, 5.f };
    m_pTransformCom->Set_Scale(&v);
    m_pTransformCom->Set_Pos(0.f, -10.f, 0.f);
    m_pTransformCom->Update_Component(0.f);
    //m_pAABB = CAABB::Create(&vPos, &vExtents, L"AABB_Dave", this);

    m_fViewZ = 0.5f;


    m_bOpen = false;

    m_bOpenTween = true;
    m_tweenOpen = m_tweenOpen.from(-10.f).to(0.f).during(200);

    m_bCloseTween = false;
    m_tweenClose = m_tweenClose.from(0.f).to(-10.f).during(200);

    m_iSelectItemIdx = 32;


    //CShipDiverBoxDave
    CShipDiverBoxDave* pDave = CShipDiverBoxDave::Create();
    pDave->Set_Parent(this);
    CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxDave", pDave);






    // idx 0~31: 아이템 칸
    for (int i = 0; i < 32; ++i)
    {
        float fPosX = (i % 8) * (1.04f) - 3.7f;
        float fPosY = (i / 8) * -(0.9f) + 0.25f;
        CShipDiverBoxInventoryItemArea* pItemArea = CShipDiverBoxInventoryItemArea::Create(i, fPosX, fPosY);
        pItemArea->Set_Parent(this);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemArea", pItemArea);

        CShipDiverBoxInventoryItemImg* pItemImg = CShipDiverBoxInventoryItemImg::Create(0.f, 0.f);
        pItemImg->Set_Parent(pItemArea);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemAreaImg", pItemImg);
    }

    //idx 32: 작살 총
    {
        CShipDiverBoxInventoryItemArea* pItemArea = CShipDiverBoxInventoryItemArea::Create(32, -0.3f, 3.1f);
        pItemArea->Set_Parent(this);
        pItemArea->Set_OffsetScaleX(3.0);
        pItemArea->Set_OffsetScaleY(1.0);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemArea", pItemArea);
    
    
        CShipDiverBoxInventoryItemImg* pItemImg = CShipDiverBoxInventoryItemImg::Create(0.f, -0.2f);
        pItemImg->Set_Parent(pItemArea);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemAreaImg", pItemImg);
   

        CShipDiverBoxInventoryEdge* pShipEdge = CShipDiverBoxInventoryEdge::Create(0.f, 0.f);
        pShipEdge->Set_Parent(pItemArea);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxEdge", pShipEdge);
    }

    //idx 33: 총기
    {
        CShipDiverBoxInventoryItemArea* pItemArea = CShipDiverBoxInventoryItemArea::Create(33, -0.3f, 2.0f);
        pItemArea->Set_Parent(this);
        pItemArea->Set_OffsetScaleX(3.0);
        pItemArea->Set_OffsetScaleY(1.0);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemArea", pItemArea);

        //, L"Tex_Ship_InventoryUpperItem_Gun"
        CShipDiverBoxInventoryItemImg* pItemImg = CShipDiverBoxInventoryItemImg::Create(0.f, -0.2f);
        pItemImg->Set_Parent(pItemArea);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemAreaImg", pItemImg);
    }

    //idx 34: 근접무기
    {
        CShipDiverBoxInventoryItemArea* pItemArea = CShipDiverBoxInventoryItemArea::Create(34, 2.6f, 3.1f);
        pItemArea->Set_Parent(this);
        pItemArea->Set_OffsetScaleX(3.0);
        pItemArea->Set_OffsetScaleY(1.0);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemArea", pItemArea);

        //, L"Tex_Ship_InventoryUpperItem_Knief"
        CShipDiverBoxInventoryItemImg* pItemImg = CShipDiverBoxInventoryItemImg::Create(0.f, -0.2f);
        pItemImg->Set_Parent(pItemArea);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemAreaImg", pItemImg);
    }

    //idx 35: 부적1
    {
        CShipDiverBoxInventoryItemArea* pItemArea = CShipDiverBoxInventoryItemArea::Create(35, 1.9f, 2.0f);
        pItemArea->Set_Parent(this);
        pItemArea->Set_OffsetScaleX(1.5);
        pItemArea->Set_OffsetScaleY(1.0);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemArea", pItemArea);

        //, L"Tex_Ship_InventoryUpperItem_Jusin"
        CShipDiverBoxInventoryItemImg* pItemImg = CShipDiverBoxInventoryItemImg::Create(0.f, -0.2f);
        pItemImg->Set_Parent(pItemArea);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemAreaImg", pItemImg);
    }
    //idx 36: 부적2
    {
        CShipDiverBoxInventoryItemArea* pItemArea = CShipDiverBoxInventoryItemArea::Create(36, 3.4f, 2.0f);
        pItemArea->Set_Parent(this);
        pItemArea->Set_OffsetScaleX(1.5);
        pItemArea->Set_OffsetScaleY(1.0);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemArea", pItemArea);

        //, L"Tex_Ship_InventoryUpperItem_Jusin"
        CShipDiverBoxInventoryItemImg* pItemImg = CShipDiverBoxInventoryItemImg::Create(0.f, -0.2f);
        pItemImg->Set_Parent(pItemArea);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemAreaImg", pItemImg);
    }



    //Tex_Ship_Inventory_ItemDescUI
    {
        CShipDiverBoxinventoryDesc* pItemImg = CShipDiverBoxinventoryDesc::Create(7.3f, -0.f, L"Tex_Ship_Inventory_ItemDescUI");
        pItemImg->Set_Parent(this);
        CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Add_GameObject(L"ShipDiverBoxItemDescUI", pItemImg);
    }




    //for (int i = 0; i < 37; ++i)
    //{
    //    CGameMemMgr::DIVERBOX_ITEM item1{};
    //    m_mapInventory.insert({ i, item1 });
    //}


    //{;
    //    m_mapInventory[32].sItemName = L"작살";
    //    m_mapInventory[32].sItemDesc = L"작살난다.";
    //    m_mapInventory[32].sAssetName = L"Tex_Ship_InventoryUpperItem_Jaksal";
    //    m_mapInventory[32].iCnt = 0;
    //}

    //{
    //    m_mapInventory[33].sItemName = L"총";
    //    m_mapInventory[33].sItemDesc = L"살살 맞으면 살지도?";
    //    m_mapInventory[33].sAssetName = L"Tex_Ship_InventoryUpperItem_Gun";
    //    m_mapInventory[33].iCnt = 0;
    //}

    //{
    //    m_mapInventory[34].sItemName = L"칼";
    //    m_mapInventory[34].sItemDesc = L"목이 칼칼하군.";
    //    m_mapInventory[34].sAssetName = L"Tex_Ship_InventoryUpperItem_Knief";
    //    m_mapInventory[34].iCnt = 0;
    //}

    //{
    //    m_mapInventory[35].sItemName = L"쥬신";
    //    m_mapInventory[35].sItemDesc = L"저 취업 할수 있는거 맞죠?";
    //    m_mapInventory[35].sAssetName = L"Tex_Ship_InventoryUpperItem_Jusin";
    //    m_mapInventory[35].iCnt = 0;
    //}

    //{
    //    m_mapInventory[36].sItemName = L"쥬신";
    //    m_mapInventory[36].sItemDesc = L"저 취업 할수 있는거 맞죠?";
    //    m_mapInventory[36].sAssetName = L"Tex_Ship_InventoryUpperItem_Jusin";
    //    m_mapInventory[36].iCnt = 0;
    //}

    //{
    //    //Tex_Ship_InventoryUpperItem_WoodPanel
    //    m_mapInventory[0].sItemName = L"나무";
    //    m_mapInventory[0].sItemDesc = L"카더가든";
    //    m_mapInventory[0].sAssetName = L"Tex_Ship_InventoryUpperItem_WoodPanel";
    //    m_mapInventory[0].iCnt = 1;
    //}

    return S_OK;
}

_int		CShipDiverBoxInventory::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    auto& m_mapInventory = CGameMemMgr::GetInstance()->Get_DaveInfo().Get_Inventory();
    if (auto pItems = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjects(L"ShipDiverBoxItemArea"))
    {
        int i = 0;
        for (auto& pItemArea : *pItems)
        {
            
            for (auto pA: *pItemArea->Get_Children())
            {
                if (auto pAA = dynamic_cast<CShipDiverBoxInventoryItemImg*>(pA))
                {
                    pAA->Set_AssetName(m_mapInventory[i].sAssetName);
                    pAA->Set_ItemCnt(m_mapInventory[i].iCnt);
                }
            }
            

            if(i == m_iSelectItemIdx)
            {
                //ShipDiverBoxEdge
                if (auto pEdge = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst<CShipDiverBoxInventoryEdge>(L"ShipDiverBoxEdge"))
                {
                    if (m_iSelectItemIdx == 32 || m_iSelectItemIdx == 33 || m_iSelectItemIdx == 34)
                    {
                        pEdge->Set_ScaleX(3.0f);
                        pEdge->Set_ScaleY(1.1f);
                    }
                    else if (m_iSelectItemIdx == 35 || m_iSelectItemIdx == 36)
                    {
                        pEdge->Set_ScaleX(1.5f);
                        pEdge->Set_ScaleY(1.1f);
                    }
                    else
                    {
                        pEdge->Set_ScaleX(1.f);
                        pEdge->Set_ScaleY(1.f);
                    }
                    pEdge->Set_Parent(pItemArea);
                }
            }

            i++;
        }
    }

    if (auto pDesc = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst<CShipDiverBoxinventoryDesc>(L"ShipDiverBoxItemDescUI"))
    {
        pDesc->Set_DescTitle(m_mapInventory[m_iSelectItemIdx].sItemName);
        pDesc->Set_DescContent(m_mapInventory[m_iSelectItemIdx].sItemDesc);
    }

    if (CDInputMgr::GetInstance()->Key_Down(DIK_C))
    {
        m_bCloseTween = true;
        CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Ship_ui_button_click", CSoundMgr::SFX_SHIP_UI_CLICK, 1.f);
    }

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

    if (m_bCloseTween)
    {
        auto val = m_tweenClose.step(int(fTimeDelta * 1000.f));
        _vec3 dir = { 0.f, 1.f, 0.f };
        _vec3 vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);
        m_pTransformCom->Set_Pos(vPos.x, val, vPos.z);
        if (m_tweenClose.progress() >= 1.0f) {

            Set_DeadCascade();
        }
    }

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CShipDiverBoxInventory::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
    // 레이와 충돌 체크
    _vec3 vRayPos, vRayDir;
    CHelper::GetMousePointRay(&vRayPos, &vRayDir);
    if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_DiverBoxItemArea"))
    { // 충돌체 순회
        for (auto& pCollider : *pColliders)
        {
            float fDist{};
            if (pCollider->Intersect(&vRayPos, &vRayDir, fDist))
            {
                if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
                {
                    // 만약 충돌한다면 태그비교후 보이드포인터 들고와서 캐스팅한다음 조작한다.
                    if (pCollider->Get_Tag() == L"AABB_DiverBoxItemArea")
                    {
                        m_iSelectItemIdx = reinterpret_cast<CShipDiverBoxInventoryItemArea*>(pCollider->Get_VoidPtr())->Get_Idx();
                        CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Ship_ui_button_choice", CSoundMgr::SFX_SHIP_UI_CLICK, 1.f);
                    }
                }
            }
        }
    }

    
    
}

void		CShipDiverBoxInventory::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DiverBoxInvenTmp"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }


    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT			CShipDiverBoxInventory::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    return S_OK;
}
CShipDiverBoxInventory* CShipDiverBoxInventory::Create()
{
    CShipDiverBoxInventory* pShipDiverBox = new CShipDiverBoxInventory;

    if (FAILED(pShipDiverBox->Ready_GameObject()))
    {
        Safe_Release(pShipDiverBox);
        MSG_BOX("pShipDiverBox Create Failed");
        return nullptr;
    }

    return pShipDiverBox;
}

void CShipDiverBoxInventory::Free()
{
    CGameObject::Free();
    //Safe_Release(m_pAABB);
}
