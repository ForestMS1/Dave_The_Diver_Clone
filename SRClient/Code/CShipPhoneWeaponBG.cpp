#include "pch.h"
#include "CShipPhoneWeaponBG.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"
#include "CShipPhone.h"
#include "CGameMemMgr.h"
#include "CShipPhoneWeaponImg.h"
#include "CShipPhoneWeaponArea.h"
#include "CShipPhoneWeaponCreateSuccess.h"

_uint CShipPhoneWeaponBG::iCurrentIdx = 0;

CShipPhoneWeaponBG::CShipPhoneWeaponBG()
    : CGameObject()
    , m_pEdge(nullptr)
{
}

CShipPhoneWeaponBG::~CShipPhoneWeaponBG()
{
}


HRESULT		CShipPhoneWeaponBG::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    //m_iSelectIdx = 0;

    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_Weapon_BG"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 296.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 308.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

   



    {
        CShipPhoneWeaponImg* pImg = CShipPhoneWeaponImg::Create(0.f, 3.3f);
        pImg->Set_Parent(this);
        pImg->Set_AssetName(L"Tex_Ship_Weapon_Logo");
        //pImg->Set_ViewZ(0.5f);
        pImg->Set_CustomScaleX(0.95f);

        pImg->Ready_AfterCreate();
        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneWeaponLogo", pImg);
    }

    //  왼위
    {
        CShipPhoneWeaponArea* pArea = CShipPhoneWeaponArea::Create(0, -1.f, 2.f);
        pArea->Set_Parent(this);
        pArea->Set_AssetName(L"Tex_UI_Gun_Basic");
        pArea->Ready_AfterCreate();


        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneWeaponArea", pArea);
    }

    // 오위
    {
        CShipPhoneWeaponArea* pArea = CShipPhoneWeaponArea::Create(1, 1.f, 2.f);
        pArea->Set_Parent(this);
        pArea->Set_AssetName(L"Tex_UI_Gun_Triple_Accel");
        pArea->Ready_AfterCreate();

        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneWeaponArea", pArea);
    }

    // 왼아
    {
        CShipPhoneWeaponArea* pArea = CShipPhoneWeaponArea::Create(2, -1.f, 0.f);
        pArea->Set_Parent(this);
        pArea->Set_AssetName(L"Tex_UI_Gun_Quadro_Accel");
        pArea->Ready_AfterCreate();

        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneWeaponArea", pArea);
    }

    // 오아
    {
        CShipPhoneWeaponArea* pArea = CShipPhoneWeaponArea::Create(3, 1.f, 0.f);
        pArea->Set_Parent(this);
        pArea->Set_AssetName(L"Tex_UI_Gun_Penta_Accel");
        pArea->Ready_AfterCreate();

        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneWeaponArea", pArea);
    }


    {
        CShipPhoneWeaponImg* pImg = CShipPhoneWeaponImg::Create(-1.f, 2.f);
        pImg->Set_Parent(this);
        pImg->Set_AssetName(L"Tex_Ship_Weapon_UI_Edge");
        pImg->Set_ViewZ(0.5f);

        m_pEdge = pImg;

        pImg->Ready_AfterCreate();
        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneWeaponEdge", pImg);
    }

    {
        CShipPhoneWeaponImg* pImg = CShipPhoneWeaponImg::Create(0.f, -2.7f);
        pImg->Set_Parent(this);
        pImg->Set_AssetName(L"Tex_Ship_Weapon_Close");
        //pImg->Set_ViewZ(0.5f);
        //pImg->Set_CustomScaleX(0.95f);

        pImg->Ready_AfterCreate();
        CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Add_GameObject(L"ShipPhoneWeaponClose", pImg);
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(0.f, 0.1f, 0.f);
    m_pTransformCom->Set_Scale(&vScale);
    return S_OK;
}

_int		CShipPhoneWeaponBG::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);


    if (auto pPhone = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst<CShipPhone>(L"ShipPhone"))
    {
        if (CDInputMgr::GetInstance()->Key_Down(DIK_C))
        {
            pPhone->UnFocus_App();
        }
    }

    if (CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
    {
        auto pExitst = CManagement::GetInstance()
            ->Get_Scene()
            ->Get_Layer(L"0_GameLogic_Layer")
            ->Get_GameObjectFirst<CShipPhoneWeaponCreateSuccess>(L"ShipPhoneCreateSuccess");
        if (!pExitst)
        {
            CShipPhoneWeaponCreateSuccess* pSuccess = CShipPhoneWeaponCreateSuccess::Create();

            if (iCurrentIdx == 0)
            {
                pSuccess->Set_AssetName(L"Tex_UI_Gun_Basic");
            }
            else if (iCurrentIdx == 1)
            {
                pSuccess->Set_AssetName(L"Tex_UI_Gun_Triple_Accel");
            }
            else if (iCurrentIdx == 2)
            {
                pSuccess->Set_AssetName(L"Tex_UI_Gun_Quadro_Accel");
            }
            else if (iCurrentIdx == 3)
            {
                pSuccess->Set_AssetName(L"Tex_UI_Gun_Penta_Accel");
            }
            pSuccess->Set_Parent(this);
            pSuccess->Ready_AfterCreate();

            CManagement::GetInstance()
                ->Get_Scene()
                ->Get_Layer(L"0_GameLogic_Layer")
                ->Add_GameObject(L"ShipPhoneCreateSuccess", pSuccess);
        }
       
    }


    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CShipPhoneWeaponBG::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
    {
        _vec3 vRayPos, vRayDir;
        CHelper::GetMousePointRay(&vRayPos, &vRayDir);
        if (auto pColliders = CColliderMgr::GetInstance()->Get_Colliders(L"Coll_Weapon"))
        { // 충돌체 순회
            for (auto& pCollider : *pColliders)
            {
                float fDist{};
                if (pCollider->Intersect(&vRayPos, &vRayDir, fDist))
                {
                    // 만약 충돌한다면 태그비교후 보이드포인터 들고와서 캐스팅한다음 조작한다.
                    if (pCollider->Get_Tag() == L"AABB_WeaponArea_0")
                    {
                        m_pEdge->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Set_Pos(-1.f, 2.f, 0.f);
                        iCurrentIdx = 0;
                    }
                    else if (pCollider->Get_Tag() == L"AABB_WeaponArea_1")
                    {
                        m_pEdge->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Set_Pos(1.f, 2.f, 0.f);
                        iCurrentIdx = 1;
                    }
                    else if (pCollider->Get_Tag() == L"AABB_WeaponArea_2")
                    {
                        m_pEdge->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Set_Pos(-1.f, 0.f, 0.f);
                        iCurrentIdx = 2;
                    }
                    else if (pCollider->Get_Tag() == L"AABB_WeaponArea_3")
                    {
                        m_pEdge->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Set_Pos(1.f, 0.f, 0.f);
                        iCurrentIdx = 3;
                    }
                }
            }
        }
    }
    
}

void		CShipPhoneWeaponBG::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();



    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_Weapon_BG"))
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

}

HRESULT			CShipPhoneWeaponBG::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipPhoneWeaponBG* CShipPhoneWeaponBG::Create()
{
    CShipPhoneWeaponBG* pIDiverBG = new CShipPhoneWeaponBG;

    if (FAILED(pIDiverBG->Ready_GameObject()))
    {
        Safe_Release(pIDiverBG);
        MSG_BOX("pIDiverBG Create Failed");
        return nullptr;
    }

    return pIDiverBG;
}

void CShipPhoneWeaponBG::Free()
{
    CGameObject::Free();

}