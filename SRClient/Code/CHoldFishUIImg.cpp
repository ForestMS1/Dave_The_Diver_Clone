#include "pch.h"
#include "CHoldFishUIImg.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"

CHoldFishUIImg::CHoldFishUIImg(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_sAssetName({})
{
}

CHoldFishUIImg::~CHoldFishUIImg()
{
}


HRESULT		CHoldFishUIImg::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fDbgX = 0.f;
    m_fDbgY = 0.f;
    m_fDbgScale = 1.f;
    m_fScale = 1.f;

    m_bRender = true;
    return S_OK;
}

_int		CHoldFishUIImg::Update_GameObject(const _float& fTimeDelta)
{
    auto pTransform = m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform");

    _vec3 vParentPos;
    pTransform->Get_Info(INFO_POS, &vParentPos);
    vParentPos.x += m_fPosX;
    vParentPos.y += m_fPosY;

    m_pTransformCom->Set_Pos(vParentPos.x + m_fDbgX, vParentPos.y + m_fDbgY, vParentPos.z);


    if (!m_sAssetName.empty())
    {
        _vec3 vScale = { 1.f , 1.f, 1.f };
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sAssetName))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                float fWidth = pTexture->Get_ImgInfo()->Width;
                float fHeight = pTexture->Get_ImgInfo()->Height;
                float fAspect = fWidth / fHeight;

                vScale = { fAspect, 1.f, 1.f };

                //float fScale = 0.15;
                //vScale.x *= fScale;
                //vScale.y *= fScale;

                vScale.x *= m_fScale;
                vScale.y *= m_fScale;

                vScale.x *= m_fDbgScale;
                vScale.y *= m_fDbgScale;
            }
        }

        m_pTransformCom->Set_Scale(&vScale);
    }

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    if (m_bRender)
    {
        CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);
    }
   

    return iExit;
}
void CHoldFishUIImg::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
    ImGui::DragFloat("m_fDbgScale", &m_fDbgScale, 0.01);
}

void		CHoldFishUIImg::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CHoldFishUIImg::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


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

}

void CHoldFishUIImg::Ready_After_Create()
{
    if (!m_sAssetName.empty())
    {
        _vec3 vScale = { 1.f , 1.f, 1.f };
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sAssetName))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            {
                float fWidth = pTexture->Get_ImgInfo()->Width;
                float fHeight = pTexture->Get_ImgInfo()->Height;
                float fAspect = fWidth / fHeight;

                vScale = { fAspect, 1.f, 1.f };

                //float fScale = 0.15;
                //vScale.x *= fScale;
                //vScale.y *= fScale;

                vScale.x *= m_fScale;
                vScale.y *= m_fScale;

                vScale.x *= m_fDbgScale;
                vScale.y *= m_fDbgScale;
            }
        }

        //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
        m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
        m_pTransformCom->Set_Scale(&vScale);
    }
}

HRESULT			CHoldFishUIImg::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CHoldFishUIImg* CHoldFishUIImg::Create(float fPosX, float fPosY)
{
    CHoldFishUIImg* pStar = new CHoldFishUIImg{ fPosX , fPosY };

    if (FAILED(pStar->Ready_GameObject()))
    {
        Safe_Release(pStar);
        MSG_BOX("pStar Create Failed");
        return nullptr;
    }

    return pStar;
}

void CHoldFishUIImg::Free()
{
    CGameObject::Free();
}