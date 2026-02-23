#include "pch.h"
#include "CTransitionImg.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CColliderMgr.h"

CTransitionImg::CTransitionImg(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_sAssetName({})
    , m_fCustomScaleX(1.f)
    , m_fCustomScaleY(1.f)
    , m_pAABB(nullptr)
{
}

CTransitionImg::~CTransitionImg()
{
}


HRESULT		CTransitionImg::Ready_GameObject()
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

void CTransitionImg::Ready_AfterCreate()
{
    _vec3 vScale = { 0.f , 0.f, 0.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sAssetName))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 284.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 260.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    //m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    vScale.x *= m_fCustomScaleX;
    vScale.y *= m_fCustomScaleY;
    m_pTransformCom->Set_Scale(&vScale);

    _vec3 vPos;
    GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

    float fOffsetX = m_fPosX;
    float fOffsetY = m_fPosY;
    vPos.x += fOffsetX;
    vPos.y += fOffsetY;

    m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);


    _vec3 vExtents = { 1.0f, 1.0f, 1.0f };


    m_pTransformCom->Update_Component(0.f);

}

_int		CTransitionImg::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);





    //_vec3 vScale = { 1.f , 1.f, 1.f };
    //if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sAssetName))
    //{
    //    if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
    //    {
    //        float fWidth = pTexture->Get_ImgInfo()->Width / 284.f;
    //        float fHeight = pTexture->Get_ImgInfo()->Height / 284.f;
    //        vScale = { fWidth, fHeight, 1.f };
    //    }
    //}
    //m_pTransformCom->Set_Scale(&vScale);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    if (!m_sColliderGroup.empty())
    {
        if (m_pAABB)
        {
            CColliderMgr::GetInstance()->AddColliderGroup(m_sColliderGroup, m_pAABB);
            m_pAABB->Transform(m_pTransformCom->Get_World());
        }
    }
    return iExit;
}

void		CTransitionImg::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CTransitionImg::Render_GameObject()
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

HRESULT			CTransitionImg::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CTransitionImg* CTransitionImg::Create(float fPosX, float fPosY)
{
    CTransitionImg* pWeaponImg = new CTransitionImg{ fPosX , fPosY };

    if (FAILED(pWeaponImg->Ready_GameObject()))
    {
        Safe_Release(pWeaponImg);
        MSG_BOX("pWeaponImg Create Failed");
        return nullptr;
    }

    return pWeaponImg;
}

void CTransitionImg::Free()
{
    CGameObject::Free();
    Safe_Release(m_pAABB);
}