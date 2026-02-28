#include "pch.h"
#include "CGoToSushiUI.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"


CGoToSushiUI::CGoToSushiUI(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CGoToSushiUI::~CGoToSushiUI()
{
}


HRESULT		CGoToSushiUI::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_GoToSushiUI"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
            vScale = { fAspect, 1.f, 1.f };

            float fScale = 1.f;
            vScale.x *= fScale;
            vScale.y *= fScale;
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fDbgX = 0.f;
    m_fDbgY = 0.f;

    m_fViewZ = 0.9f;


    m_fAlpha = 0.f;
    m_tween = m_tween
        .from(0.f).to(1.f).during(1000)
        .to(1.f).to(1.f).during(1000)
        .to(1.f).to(0.f).during(1000)
        .via(tweeny::easing::linear);

    return S_OK;
}

_int		CGoToSushiUI::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    m_fAlpha = m_tween.step(int(fTimeDelta * 1000.f));

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CGoToSushiUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    if (m_tween.progress() >= 1.0f) {

        Set_DeadCascade();
    }
}

void		CGoToSushiUI::Render_GameObject()
{

    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    float alphaRatio = m_fAlpha;
    DWORD tfactor = D3DCOLOR_ARGB(
        (BYTE)(alphaRatio * 255.f),
        255, 255, 255
    );
    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tfactor);

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_GoToSushiUI"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    // COLOR = Texture * TFACTOR
    pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

    // ALPHA = TextureAlpha * TFACTORAlpha
    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);
    m_pBufferCom->Render_Buffer();

    pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE); // 보통 기본값도 MODULATE인 경우가 많지만 확인 필요
    pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE); // TFACTOR를 DIFFUSE로 원복

    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE); // TFACTOR를 DIFFUSE로 원복



    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);





}

void CGoToSushiUI::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
}

HRESULT			CGoToSushiUI::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

}


CGoToSushiUI* CGoToSushiUI::Create(float fPosX, float fPosY)
{
    CGoToSushiUI* pIDiverUpgrade = new CGoToSushiUI{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CGoToSushiUI::Free()
{
    CGameObject::Free();
}