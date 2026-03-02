#include "pch.h"
#include "CTransitionFace.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"

CTransitionFace::CTransitionFace(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CTransitionFace::~CTransitionFace()
{
}


HRESULT		CTransitionFace::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    _vec3 vScale = { 0.03f * 12.f/16.f , 0.03f, 0.03f };

    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);
    m_tween = m_tween.from(-0.5f).to(-0.4f).during(300).to(-0.5f).during(300).to(-0.5f).during(300);

    m_fAccDelayTime = 0.f;
    m_fDelay = 0.f;
    m_iIdx = rand() % 9;
    return S_OK;
}

_int		CTransitionFace::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_TRANSITION, this);
    m_fAccDelayTime += fTimeDelta;
    if (m_fAccDelayTime > m_fDelay)
    {
        auto val = m_tween.step(int(fTimeDelta * 1000.f));

        _vec3 vPos;
        m_pTransformCom->Get_Info(INFO_POS, &vPos);

        m_pTransformCom->Set_Pos(vPos.x, val, vPos.z);

        if (m_tween.progress() >= 1.0f) {
            m_tween.seek(0.f);
        }
    }
    

    return iExit;
}

void		CTransitionFace::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CTransitionFace::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_Transition_LoadingFace"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(m_iIdx)))
        {
            pGraphicDev->SetTexture(0, pTexture->Get_Texture());
        }
    }

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

}

HRESULT			CTransitionFace::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;
}


CTransitionFace* CTransitionFace::Create(float fPosX, float fPosY)
{
    CTransitionFace* pIDiverUpgrade = new CTransitionFace{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CTransitionFace::Free()
{
    CGameObject::Free();
}