#include "pch.h"
#include "CShipDiverBoxInventoryItemImg.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"

CShipDiverBoxInventoryItemImg::CShipDiverBoxInventoryItemImg(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_sAssetName({})
{
}

CShipDiverBoxInventoryItemImg::~CShipDiverBoxInventoryItemImg()
{
}


HRESULT		CShipDiverBoxInventoryItemImg::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sAssetName))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 180.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 180.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(0.0f, -10.0f, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fViewZ = 0.48f;

    m_iCnt = 0;
    return S_OK;
}

_int		CShipDiverBoxInventoryItemImg::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sAssetName))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 180.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 180.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }
    m_pTransformCom->Set_Scale(&vScale);

    _vec3 vPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

    float fOffsetX = m_fPosX;
    float fOffsetY = m_fPosY;
    vPos.x += fOffsetX;
    vPos.y += fOffsetY;

    m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);

    if (!m_sAssetName.empty())
    {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    }

    return iExit;
}

void		CShipDiverBoxInventoryItemImg::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipDiverBoxInventoryItemImg::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    pGraphicDev->SetTexture(0, nullptr);

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sAssetName))
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

    
    if (m_iCnt > 0)
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = 0.3f;
        float fOffsetY = -0.1f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(to_wstring(m_iCnt), &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
    }

}

HRESULT			CShipDiverBoxInventoryItemImg::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipDiverBoxInventoryItemImg* CShipDiverBoxInventoryItemImg::Create(float fPosX, float fPosY)
{
    CShipDiverBoxInventoryItemImg* pShipDiverBoxInventoryItemImg = new CShipDiverBoxInventoryItemImg{ fPosX , fPosY };

    if (FAILED(pShipDiverBoxInventoryItemImg->Ready_GameObject()))
    {
        Safe_Release(pShipDiverBoxInventoryItemImg);
        MSG_BOX("pShipDiverBoxInventoryItemImg Create Failed");
        return nullptr;
    }

    return pShipDiverBoxInventoryItemImg;
}

void CShipDiverBoxInventoryItemImg::Free()
{
    CGameObject::Free();
}