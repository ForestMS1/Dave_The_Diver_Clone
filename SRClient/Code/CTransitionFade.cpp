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
        m_tween = m_tween.from(1.f).to(0.f).during(700).via(tweeny::easing::linear);
    }
    else
    {
        m_tween = m_tween.from(0.f).to(1.f).during(700).via(tweeny::easing::linear);
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

void CTransitionFade::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    // 1. 기존 행렬(View, Proj) 백업
    D3DXMATRIX matOldView, matOldProj;
    pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
    pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

    // 2. 오소그래픽 및 뷰 행렬 설정 (UI 모드)
    D3DXMATRIX matView, matProj;
    D3DXMatrixIdentity(&matView); // 카메라는 원점에 고정
    D3DXMatrixOrthoLH(&matProj, (float)WINCX, (float)WINCY, 0.f, 1.f);

    pGraphicDev->SetTransform(D3DTS_VIEW, &matView);
    pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);

    
    float alphaRatio = m_fAlpha;
    DWORD tfactor = D3DCOLOR_ARGB((BYTE)(alphaRatio * 255.f), 255, 255, 255);

    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
    pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, tfactor);

   
    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Transition_Fade_Black"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
    }

    // Texture Stage States (TFACTOR 사용)
    pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR);

    m_pBufferCom->Render_Buffer();

    // 스테이트 원복
    pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

    D3DXMATRIX matIdentity;
    D3DXMatrixIdentity(&matIdentity);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matIdentity);

    pGraphicDev->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
    pGraphicDev->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    // --- 기존 렌더링 로직 끝 ---

    // 3. 기존 행렬 복구 (다시 3D 세상을 그릴 수 있도록)
    pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
    pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);
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