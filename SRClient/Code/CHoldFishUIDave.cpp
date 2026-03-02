#include "pch.h"
#include "CHoldFishUIDave.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"

CHoldFishUIDave::CHoldFishUIDave()
    : CGameObject()
{
}


CHoldFishUIDave::~CHoldFishUIDave()
{
}

HRESULT CHoldFishUIDave::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    m_pTextureFrameCom->RegistTextureFrame(L"Default", L"Tex_Ship_DiverBoxDave", 0.2f);
    m_pTextureFrameCom->Set_TextureFrame(L"Default");


    m_fViewZ = 0.49f;


    _vec3 vScale = { 1.0f, 1.0f, 1.0f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Ship_DiverBoxDave"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width / 200.f;
            float fHeight = pTexture->Get_ImgInfo()->Height / 200.f;
            vScale = { fWidth, fHeight, 1.f };
        }
    }

    m_pTransformCom->Set_Scale(&vScale);
    m_pTransformCom->Set_Pos(0.f, -10.f, 0.f);




    return S_OK;
}

_int CHoldFishUIDave::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA_AFTER_ORTHO_UI, this);



    _vec3 vPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

    float fOffsetX = -3.f;
    float fOffsetY = 2.5;
    vPos.x += fOffsetX;
    vPos.y += fOffsetY;


    m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
    return iExit;
}

void CHoldFishUIDave::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CHoldFishUIDave::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    CTextureFrame::FRAME_INFO fInfo;

    if (m_pTextureFrameCom->Get_CurrentInfo(fInfo))
    {
        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(fInfo.sAssetTextureName))
        {
            if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(fInfo.iFrame)))
            {
                pGraphicDev->SetTexture(0, pTexture->Get_Texture());
            }
        }
    }



    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CHoldFishUIDave::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    // 텍스쳐프레임
    if (FAILED((AddComponent<Engine::CTextureFrame, ID_DYNAMIC>(L"Proto_TextureFrame", L"Com_TextureFrame", &m_pTextureFrameCom))))
        return E_FAIL;

    return S_OK;
}

CHoldFishUIDave* CHoldFishUIDave::Create()
{
    CHoldFishUIDave* pShipDave = new CHoldFishUIDave;

    if (FAILED(pShipDave->Ready_GameObject()))
    {
        Safe_Release(pShipDave);
        MSG_BOX("pShipDave Create Failed");
        return nullptr;
    }

    return pShipDave;
}

void CHoldFishUIDave::Free()
{
    CGameObject::Free();
}
