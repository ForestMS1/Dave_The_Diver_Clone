#include "pch.h"
#include "CShipPhoneIDiverBG.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CShipPhoneIDiverItem.h"
#include "CManagement.h"
#include "CShipPhoneIDiverSelectEdge.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"
#include "CShipPhoneIDiverUpgrade.h"
#include "CShipPhone.h"
#include "CGameMemMgr.h"

CShipPhoneIDiverBG::CShipPhoneIDiverBG()
	: CGameObject()
{
}

CShipPhoneIDiverBG::~CShipPhoneIDiverBG()
{
}


HRESULT		CShipPhoneIDiverBG::Ready_GameObject()
{
	if (FAILED(Ready_Component()))
		return E_FAIL;

    m_iSelectIdx = 0;

    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_PhoneIDiverBG"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 284.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 260.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

    ;
    {
        
        CShipPhoneIDiverItem* pShipItem = CShipPhoneIDiverItem::Create(0, 0.f, 2.f);
        pShipItem->Set_AssetName(L"Tex_Ship_IDiver_Item_Sanso");
        pShipItem->Set_Parent(this);
        //pShipItem->Set_Title(L"공기통");
        //pShipItem->Set_LeftTop(L"Lv.1");
        //pShipItem->Set_LeftMiddle(L"공기량");
        //pShipItem->Set_LeftBottom(L"90 bar");
        //pShipItem->Set_RightTop(L"Lv.2");
        //pShipItem->Set_RightMiddle(L"공기량");
        //pShipItem->Set_RightBottom(L"115 bar");
        //pShipItem->Set_Money(L"123");
        //pShipItem->Set_MoneyLack(false);

        pShipItem->Ready_AfterCreate();
        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneIDIverItem", pShipItem);
    }

    {
        CShipPhoneIDiverItem* pShipItem = CShipPhoneIDiverItem::Create(1, 0.f, 0.7f);
        pShipItem->Set_AssetName(L"Tex_Ship_IDiver_Item_Clothes");
        pShipItem->Set_Parent(this);
        pShipItem->Set_Title(L"잠수복");
        pShipItem->Set_LeftTop(L"Lv.1");
        pShipItem->Set_LeftMiddle(L"한계수심");
        pShipItem->Set_LeftBottom(L"90 bar");
        pShipItem->Set_RightTop(L"80 m");
        pShipItem->Set_RightMiddle(L"한계수심");
        pShipItem->Set_RightBottom(L"110 m");
        pShipItem->Set_Money(L"111");
        pShipItem->Set_MoneyLack(false);

        pShipItem->Ready_AfterCreate();
        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneIDIverItem", pShipItem);
    }

    {
        CShipPhoneIDiverItem* pShipItem = CShipPhoneIDiverItem::Create(2, 0.f, -0.6f);
        pShipItem->Set_AssetName(L"Tex_Ship_IDiver_Item_Cage");
        pShipItem->Set_Parent(this);
        pShipItem->Set_Title(L"적재함");
        pShipItem->Set_LeftTop(L"Lv.1");
        pShipItem->Set_LeftMiddle(L"허용무게");
        pShipItem->Set_LeftBottom(L"9 kg");
        pShipItem->Set_RightTop(L"80 m");
        pShipItem->Set_RightMiddle(L"허용무게");
        pShipItem->Set_RightBottom(L"12 kg");
        pShipItem->Set_Money(L"222");
        pShipItem->Set_MoneyLack(false);

        pShipItem->Ready_AfterCreate();
        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneIDIverItem", pShipItem);
    }

    {
        CShipPhoneIDiverItem* pShipItem = CShipPhoneIDiverItem::Create(3, 0.f, -1.9f);
        pShipItem->Set_AssetName(L"Tex_Ship_IDiver_Item_Jaksal");
        pShipItem->Set_Parent(this);
        pShipItem->Set_Title(L"작살총");
        pShipItem->Set_LeftTop(L"낡은작살");
        pShipItem->Set_LeftMiddle(L"데미지");
        pShipItem->Set_LeftBottom(L"1");
        pShipItem->Set_RightTop(L"강철작살");
        pShipItem->Set_RightMiddle(L"데미지");
        pShipItem->Set_RightBottom(L"3");
        pShipItem->Set_Money(L"321");
        pShipItem->Set_MoneyLack(false);

        pShipItem->Ready_AfterCreate();
        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneIDIverItem", pShipItem);
    }



    {
        CShipPhoneIDiverSelectEdge* pEdge = CShipPhoneIDiverSelectEdge::Create(0.f, 0.f);
        pEdge->Set_Parent(this);

        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneIDiverEdge", pEdge);
        m_pEdge = pEdge;
    }


    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(0.f, 0.1f, 0.f);
    m_pTransformCom->Set_Scale(&vScale);
    return S_OK;    
}

_int		CShipPhoneIDiverBG::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    auto pExists = CManagement::GetInstance()
        ->Get_Scene()
        ->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"ShipPhoneIDiverUpgrade");

    if (pExists)
    {
        m_bUpgradeOpen = true;
    }
    else
    {
        m_bUpgradeOpen = false;
    }

    if (m_bUpgradeOpen)
    {
        
    }
    else
    {
        if (auto pPhone = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst<CShipPhone>(L"ShipPhone"))
        {
            if (CDInputMgr::GetInstance()->Key_Down(DIK_C))
            {
                pPhone->UnFocus_App();
            }
        }
        //CShipPhone
    }

    if (m_iSelectIdx == 0)
    {
        m_pEdge->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Set_Pos(0.f, 2.0f, 0.f);
    }
    else if (m_iSelectIdx == 1)
    {
        m_pEdge->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Set_Pos(0.0f, 0.7f, 0.f);
    }
    else if (m_iSelectIdx == 2)
    {
        m_pEdge->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Set_Pos(0.f, -0.6f, 0.f);
    }
    else if (m_iSelectIdx == 3)
    {
        m_pEdge->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Set_Pos(0.f, -1.9f, 0.f);
    }


   
   CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
       


    return iExit;
}

void		CShipPhoneIDiverBG::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    
    _vec3 vRayPos, vRayDir;
    CHelper::GetMousePointRay(&vRayPos, &vRayDir);
    if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_IDiver"))
    { // 충돌체 순회
        for (auto& pCollider : *pColliders)
        {
            float fDist{};
            if (pCollider->Intersect(&vRayPos, &vRayDir, fDist))
            {
                if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
                {
                    // 만약 충돌한다면 태그비교후 보이드포인터 들고와서 캐스팅한다음 조작한다.
                    if (pCollider->Get_Tag() == L"AABB_IDiverItem")
                    {
                        _uint idx = reinterpret_cast<CShipPhoneIDiverItem*>(pCollider->Get_VoidPtr())->Get_Idx();
                        if (m_iSelectIdx == idx)
                        {
                            auto pExists = CManagement::GetInstance()
                                ->Get_Scene()
                                ->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst(L"ShipPhoneIDiverUpgrade");
                            if (!pExists)
                            {
                                CShipPhoneIDiverUpgrade* pUp = CShipPhoneIDiverUpgrade::Create(0.f, 0.f);
                                pUp->Set_Parent(this);
                                pUp->Set_Title(L"공기통");
                                pUp->Set_LeftTop(L"Lv1");
                                pUp->Set_LeftMiddle(L"허용무게");
                                pUp->Set_LeftBottom(L"9 kg");
                                pUp->Set_RightTop(L"Lv2");
                                pUp->Set_RightMiddle(L"허용무게");
                                pUp->Set_RightBottom(L"13kg");
                                pUp->Set_Desc(L"DESSCSC");
                                pUp->Set_Money(L"12");
                                pUp->Set_MoneyLack(true);

                                if (idx == 0)
                                {
                                    pUp->Set_AssetName(L"Tex_Ship_IDiver_Item_Sanso");
                                }
                                else if (idx == 1)
                                {
                                    pUp->Set_AssetName(L"Tex_Ship_IDiver_Item_Clothes");
                                }
                                else if (idx == 2)
                                {
                                    pUp->Set_AssetName(L"Tex_Ship_IDiver_Item_Cage");
                                }
                                else if (idx == 3)
                                {
                                    pUp->Set_AssetName(L"Tex_Ship_IDiver_Item_Jaksal");
                                }
                               
                                pUp->Ready_AfterCreate();

                                CManagement::GetInstance()
                                    ->Get_Scene()
                                    ->Get_Layer(L"0_GameLogic_Layer")
                                    ->Add_GameObject(L"ShipPhoneIDiverUpgrade", pUp);
                            }
                        }

                        if (!m_bUpgradeOpen)
                        {
                            m_iSelectIdx = idx;
                        }
                    }
                }
            }
        }
    }
}

void		CShipPhoneIDiverBG::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_PhoneIDiverBG"))
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

HRESULT			CShipPhoneIDiverBG::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipPhoneIDiverBG* CShipPhoneIDiverBG::Create()
{
    CShipPhoneIDiverBG* pIDiverBG = new CShipPhoneIDiverBG;

    if (FAILED(pIDiverBG->Ready_GameObject()))
    {
        Safe_Release(pIDiverBG);
        MSG_BOX("pIDiverBG Create Failed");
        return nullptr;
    }

    return pIDiverBG;
}

void CShipPhoneIDiverBG::Free()
{
    CGameObject::Free();
}