#include "pch.h"
#include "CLeah.h"
#include "CDInputMgr.h"
#include "CRenderer.h"
#include "CGraphicDev.h"
#include "CColliderMgr.h"
#include "CCollisionMgr.h"
#include "CAssetMgr.h"
#include "CAssetTexture.h"

CLeah::CLeah()
    : CGameObject()
{
}


CLeah::~CLeah()
{
}

HRESULT CLeah::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_pTextureFrameCom->RegistTextureFrame(L"Default", L"Tex_Transition_Leah", 0.2f);
    m_pTextureFrameCom->RegistTextureFrame(L"Leah2", L"Tex_Transition_Leah2", 0.2f);
    m_pTextureFrameCom->Set_TextureFrame(L"Default");

    m_fViewZ = 0.49f;


    _vec3 vScale = { 1.0f, 1.0f, 1.0f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Transition_Leah2"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fAspect = pTexture->Get_ImgInfo()->Width / pTexture->Get_ImgInfo()->Height;
            vScale = { fAspect, 1.f, 1.f };
            
            float fScale = 0.1;
            vScale.x *= fScale;
            vScale.y *= fScale;
        }
    }

    m_pTransformCom->Set_Scale(&vScale);
    m_pTransformCom->Set_Pos(0.f, -0.f, 0.f);




    return S_OK;
}

_int CLeah::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);
    CRenderer::GetInstance()->Add_RenderGroup(RENDER_TRANSITION, this);



    _vec3 vPos;
    m_pParentGameObject->GetComponent<CTransform, ID_DYNAMIC>(L"Com_Transform")->Get_Info(INFO_POS, &vPos);

    float fOffsetX = 0.3f;
    float fOffsetY = -0.45;
    vPos.x += fOffsetX;
    vPos.y += fOffsetY;


    m_pTransformCom->Set_Pos(vPos.x, vPos.y, vPos.z);
    return iExit;
}

void CLeah::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

}

void CLeah::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);


    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    CTextureFrame::FRAME_INFO fInfo;

    if (m_pTextureFrameCom->Get_CurrentInfo(fInfo))
    {
        if (fInfo.sFrameName == L"Default")
        {
            if (fInfo.iFrame == fInfo.iMaxFrame - 1)
            {
                m_pTextureFrameCom->Set_TextureFrame(L"Leah2");
            }
        }
       
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

HRESULT CLeah::Ready_Component()
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

CLeah* CLeah::Create()
{
    CLeah* pShipDave = new CLeah;

    if (FAILED(pShipDave->Ready_GameObject()))
    {
        Safe_Release(pShipDave);
        MSG_BOX("pShipDave Create Failed");
        return nullptr;
    }

    return pShipDave;
}

void CLeah::Free()
{
    CGameObject::Free();
}
