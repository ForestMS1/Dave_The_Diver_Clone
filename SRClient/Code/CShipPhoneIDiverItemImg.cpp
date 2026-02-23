#include "pch.h"
#include "CShipPhoneIDiverItemImg.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"

CShipPhoneIDiverItemImg::CShipPhoneIDiverItemImg(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_sAssetName({})
{
}

CShipPhoneIDiverItemImg::~CShipPhoneIDiverItemImg()
{
}


HRESULT		CShipPhoneIDiverItemImg::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 0.f , 0.f, 0.f };
    //if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sAssetName))
    //{
    //    if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
    //    {
    //        float fWidth = pTexture->Get_ImgInfo()->Width / 284.f;
    //        float fHeight = pTexture->Get_ImgInfo()->Height / 260.f;
    //        vScale = { fWidth, fHeight, 1.f };
    //    }
    //}

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);
    return S_OK;
}

_int		CShipPhoneIDiverItemImg::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    _vec3 vPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

    float fOffsetX = m_fPosX;
    float fOffsetY = m_fPosY;
    vPos.x += fOffsetX;
    vPos.y += fOffsetY;

    m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);



    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sAssetName))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 284.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 284.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }
    m_pTransformCom->Set_Scale(&vScale);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CShipPhoneIDiverItemImg::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CShipPhoneIDiverItemImg::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


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
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT			CShipPhoneIDiverItemImg::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CShipPhoneIDiverItemImg* CShipPhoneIDiverItemImg::Create(float fPosX, float fPosY)
{
    CShipPhoneIDiverItemImg* pIDiverItemImg = new CShipPhoneIDiverItemImg{ fPosX , fPosY };

    if (FAILED(pIDiverItemImg->Ready_GameObject()))
    {
        Safe_Release(pIDiverItemImg);
        MSG_BOX("pIDiverItemImg Create Failed");
        return nullptr;
    }

    return pIDiverItemImg;
}

void CShipPhoneIDiverItemImg::Free()
{
    CGameObject::Free();
}