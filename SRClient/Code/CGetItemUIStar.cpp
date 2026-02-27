#include "pch.h"
#include "CGetItemUIStar.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"

CGetItemUIStar::CGetItemUIStar(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CGetItemUIStar::~CGetItemUIStar()
{
}


HRESULT		CGetItemUIStar::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_GetItemUIStar"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {

            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
          
            vScale = { fAspect, 1.f, 1.f };

            float fScale = 0.15;
            vScale.x *= fScale;
            vScale.y *= fScale;
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);
    return S_OK;
}

_int		CGetItemUIStar::Update_GameObject(const _float& fTimeDelta)
{
    auto pTransform = m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform");

    _vec3 vParentPos;
    pTransform->Get_Info(INFO_POS, &vParentPos);
    vParentPos.x += m_fPosX;
    vParentPos.y += m_fPosY;

    m_pTransformCom->Set_Pos(vParentPos.x, vParentPos.y, vParentPos.z);

    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CGetItemUIStar::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CGetItemUIStar::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    //pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_GetItemUIStar"))
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

HRESULT			CGetItemUIStar::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CGetItemUIStar* CGetItemUIStar::Create(float fPosX, float fPosY)
{
    CGetItemUIStar* pStar = new CGetItemUIStar{ fPosX , fPosY };

    if (FAILED(pStar->Ready_GameObject()))
    {
        Safe_Release(pStar);
        MSG_BOX("pStar Create Failed");
        return nullptr;
    }

    return pStar;
}

void CGetItemUIStar::Free()
{
    CGameObject::Free();
}