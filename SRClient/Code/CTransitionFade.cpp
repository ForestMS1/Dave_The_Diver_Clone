#include "pch.h"
#include "CTransitionFade.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"

CTransitionFade::CTransitionFade(float fPosX, float fPosY, FADE_ID eFade)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
    , m_eFade(eFade)
{
}

CTransitionFade::~CTransitionFade()
{
}


HRESULT		CTransitionFade::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 999.f, 999.f, 2.f };

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);
    
    if (m_eFade == FADE_IN)
    {
        m_tween = m_tween.from(1.f).to(0.f).during(300).via(tweeny::easing::linear);
    }
    else
    {
        m_tween = m_tween.from(0.f).to(1.f).during(300).via(tweeny::easing::linear);
    }

    m_fViewZ = -9999.f;
    return S_OK;
}

_int		CTransitionFade::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_TRANSITION, this);

    m_fAlpha = m_tween.step(int(fTimeDelta * 1000.f));

    return iExit;
}

void		CTransitionFade::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    if (m_tween.progress() >= 1.0f) {

        if (m_funcOnEnd)
        {
            m_funcOnEnd();
        }

        Set_DeadCascade();
    }
}

void		CTransitionFade::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
    float alphaRatio = m_fAlpha;
    DWORD tfactor = D3DCOLOR_ARGB(
        (BYTE)(alphaRatio * 255.f),
        255, 255, 255
    );
    //pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    //pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tfactor);

        pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

        if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Transition_Fade_Black"))
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

    //pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    //pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
}

HRESULT			CTransitionFade::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CTransitionFade* CTransitionFade::Create(float fPosX, float fPosY, FADE_ID eFade)
{
    CTransitionFade* pTransitionFade = new CTransitionFade{ fPosX , fPosY, eFade };

    if (FAILED(pTransitionFade->Ready_GameObject()))
    {
        Safe_Release(pTransitionFade);
        MSG_BOX("pTransitionFade Create Failed");
        return nullptr;
    }

    return pTransitionFade;
}

void CTransitionFade::Free()
{
    CGameObject::Free();
}