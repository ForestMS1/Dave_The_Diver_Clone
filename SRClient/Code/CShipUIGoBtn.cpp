#include "pch.h"
#include "CShipUIGoBtn.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CDInputMgr.h"
#include "CTransition.h"
#include "CManagement.h"
#include "CShipUIGoSushiBtn.h"
#include "CSoundMgr.h"

CShipUIGoBtn::CShipUIGoBtn(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CShipUIGoBtn::~CShipUIGoBtn()
{
}


HRESULT		CShipUIGoBtn::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fScaleX = 1.f;
    m_fScaleY = 1.f;
    m_fActiveScaleX = 1.f;
    m_fActiveScaleY = 1.f;
    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_UI_GoBtn"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            m_fScaleX = pTexture->Get_ImgInfo()->Width / 200.f;
            m_fScaleY = pTexture->Get_ImgInfo()->Height / 200.f;
            m_fActiveScaleX = m_fScaleX * 1.1f;
            m_fActiveScaleY = m_fScaleY * 1.1f;
            vScale = { m_fScaleX, m_fScaleY, 1.f };
        }
    }

    m_bActive = false;

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);
    return S_OK;
}

_int		CShipUIGoBtn::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    if (m_bActive)
    {
        m_sImgName = L"Tex_Ship_UI_GoBtn";
        _vec3 vScale = { m_fActiveScaleX , m_fActiveScaleY, 1.f };
        m_pTransformCom->Set_Scale(&vScale);

        if (CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
        {
            //CTransition::FadedTransition(CTransition::SCENE_SHIP, CTransition::SCENE_SUSHI);

            if (auto pLayer = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer"))
            {
                CShipUIGoSushiBtn* pGoSushiBtn = CShipUIGoSushiBtn::Create(-4.45f, -1.3f);
                if (nullptr == pGoSushiBtn)
                    return NOEVENT;
                if (FAILED(pLayer->Add_GameObject(L"ShipGoSushiBtn", pGoSushiBtn)))
                    return NOEVENT;

                if (auto pSpace = pLayer->Get_GameObjectFirst(L"ShipGoBtnSpaceKey"))
                {
                    pSpace->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Set_Pos(-3.25f, -1.3f, 0.f);
                }
            }
            CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Ship_ui_button_choice", CSoundMgr::SFX_SHIP_UI_APP_CLICK, 1.f);
            //ShipGoBtnSpaceKey
        }
        
    }
    else
    {
        m_sImgName = L"Tex_Ship_UI_GoBtnAlpha";
        _vec3 vScale = { m_fScaleX , m_fScaleY, 1.f };
        m_pTransformCom->Set_Scale(&vScale);
    }
    

    return iExit;
}

void		CShipUIGoBtn::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipUIGoBtn::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sImgName))
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
    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

}

HRESULT			CShipUIGoBtn::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipUIGoBtn* CShipUIGoBtn::Create(float fPosX, float fPosY)
{
    CShipUIGoBtn* pShipDiveBtn = new CShipUIGoBtn{ fPosX , fPosY };

    if (FAILED(pShipDiveBtn->Ready_GameObject()))
    {
        Safe_Release(pShipDiveBtn);
        MSG_BOX("pShipDiveBtn Create Failed");
        return nullptr;
    }

    return pShipDiveBtn;
}

void CShipUIGoBtn::Free()
{
    CGameObject::Free();
}