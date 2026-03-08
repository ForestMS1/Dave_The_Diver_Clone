#include "pch.h"
#include "CShipUIDiveBtn.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CDInputMgr.h"
#include "CManagement.h"
#include "CShipDave.h"
#include "CSoundMgr.h"
#include "CLog.h"

CShipUIDiveBtn::CShipUIDiveBtn(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CShipUIDiveBtn::~CShipUIDiveBtn()
{
}


HRESULT		CShipUIDiveBtn::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_UI_DiveSpriteBtn"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 200.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 200.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

    m_bAddRender = false;

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fAccFrameDelta = 0.f;
    m_iFrame = 0;

    return S_OK;
}

_int		CShipUIDiveBtn::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    bool m_bSpacePressed = false;

    if (m_bAddRender)
    {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

        if (CDInputMgr::GetInstance()->Key_Pressing(DIK_SPACE))
        {

            m_fAccFrameDelta += fTimeDelta;

            m_bSpacePressed = true;
            if (m_iFrame < 37)
            {
                if (m_fAccFrameDelta > 0.05f)
                {
                    ++m_iFrame;
                    m_fAccFrameDelta = 0;
                }


                if (m_iFrame == 1)
                {
                   
                    if (!CSoundMgr::GetInstance()->IsChannelPlaying(CSoundMgr::SFX_SHIP_DIVE_BTN))
                    {
                        //CLog::Debug(L"PlaySoundOne \n");
                        CSoundMgr::GetInstance()->PlaySoundOne(L"Sound_Ship_ui_lobby_dive_01", CSoundMgr::SFX_SHIP_DIVE_BTN, 1.f);
                    }
                }
            }
        }
        // 끝까지 채워지고 이제 다이브레디로
        if (m_iFrame == 37)
        {
            //ShipDave
      
            if (auto pDave = CManagement::GetInstance()->Get_Scene()->Get_Layer(L"0_GameLogic_Layer")->Get_GameObjectFirst<CShipDave>(L"ShipDave"))
            {
                pDave->DoDiveReady();
            }

            //CSoundMgr::GetInstance()->StopSound(CSoundMgr::SFX_SHIP_DIVE_BTN);

        }
        // 끝까지 채우지 않았으면 초기화
        if (!m_bSpacePressed)
        {
            if (m_iFrame < 37)
            {
                m_iFrame = 0;
            }
            if (CSoundMgr::GetInstance()->IsChannelPlaying(CSoundMgr::SFX_SHIP_DIVE_BTN))
            {
                //CLog::Debug(L"StopSound \n");
                CSoundMgr::GetInstance()->StopSound(CSoundMgr::SFX_SHIP_DIVE_BTN);
            }
        }
    }
    else
    {
        m_iFrame = 0;
    }

   


    return iExit;
}

void		CShipUIDiveBtn::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipUIDiveBtn::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_UI_DiveSpriteBtn"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(m_iFrame)))
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

HRESULT			CShipUIDiveBtn::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipUIDiveBtn* CShipUIDiveBtn::Create(float fPosX, float fPosY)
{
    CShipUIDiveBtn* pShipDiveBtn = new CShipUIDiveBtn{ fPosX , fPosY };

    if (FAILED(pShipDiveBtn->Ready_GameObject()))
    {
        Safe_Release(pShipDiveBtn);
        MSG_BOX("pShipDiveBtn Create Failed");
        return nullptr;
    }

    return pShipDiveBtn;
}

void CShipUIDiveBtn::Free()
{
    CGameObject::Free();
}