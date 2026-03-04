#include "pch.h"
#include "CDaveConversation.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"


CDaveConversation::CDaveConversation(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CDaveConversation::~CDaveConversation()
{
}


HRESULT		CDaveConversation::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DaveConversation"))
    {
        if (auto pTexture = dynamic_cast<CAssetTexture*>(vecAsset->at(0)))
        {
            float fWidth = pTexture->Get_ImgInfo()->Width;
            float fHeight = pTexture->Get_ImgInfo()->Height;
            float fAspect = fWidth / fHeight;
            vScale = { fAspect, 1.f, 1.f };

            float fScale = 2.f;
            vScale.x *= fScale;
            vScale.y *= fScale;
        }
    }

    //_vec3 vPos = { 0.0f, -10.0f, 0.0f };
    m_pTransformCom->Set_Pos(m_fPosX, m_fPosY, 0.f);
    m_pTransformCom->Set_Scale(&vScale);

    m_fDbgX = 0.f;
    m_fDbgY = 0.f;

    m_fViewZ = 0.3f;


    //m_fAlpha = 0.f;
    //m_tween = m_tween
    //    .from(0.f).to(1.f).during(1000)
    //    .to(1.f).to(1.f).during(1000)
    //    .to(1.f).to(0.f).during(1000)
    //    .via(tweeny::easing::linear);

    m_fTimer = 0.f;

    m_vecConversation[CONV_1].push_back(L"선생님");
    m_vecConversation[CONV_1].push_back(L"저 사실\n...\n...\n...");
    m_vecConversation[CONV_1].push_back(L"중퇴가 하고싶어요");

    m_iCurrentConversationIdx = 0;
    m_eCurrentConversation = CONV_1;

    m_iCurrentTxtIdx = 0;
    m_sCurrentTxt = L"";

    return S_OK;
}

_int		CDaveConversation::Update_GameObject(const _float& fTimeDelta)
{
    if (CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
    {
        ++m_iCurrentConversationIdx;
        if (m_iCurrentConversationIdx == m_vecConversation[m_eCurrentConversation].size())
        {
            Set_DeadCascade();
            return OBJ_DEAD;
        }
        m_iCurrentTxtIdx = 0;
        m_fTimer = 0.f;
    }
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);



    m_fTimer += fTimeDelta;

    if (m_fTimer > 0.2f)
    {
        if (m_iCurrentTxtIdx < m_vecConversation[m_eCurrentConversation][m_iCurrentConversationIdx].size())
        {
            ++m_iCurrentTxtIdx;
        }
        m_fTimer = 0.f;
    }
    
    m_sCurrentTxt.assign(m_vecConversation[m_eCurrentConversation][m_iCurrentConversationIdx], 0, m_iCurrentTxtIdx);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CDaveConversation::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CDaveConversation::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();
  
    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_DaveConversation"))
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


    // Conversation
    {
        _vec3 vInfoPos;
        m_pTransformCom->Get_Info(INFO_POS, &vInfoPos);
        float fOffsetX = -3.4f;
        float fOffsetY = 0.f;
        vInfoPos.x += fOffsetX;
        vInfoPos.y += fOffsetY;

        _vec3 vScreenPos;
        CHelper::GetScreenPointFromWorld(&vScreenPos, &vInfoPos);

        _vec2 vPos = { vScreenPos.x , vScreenPos.y };
        if (CAssetDefaultFont* pDefFont = CAssetMgr::GetInstance()->Get_AssetFirst<CAssetDefaultFont>(L"Font_210YouthL"))
        {
            pDefFont->Render_Font(m_sCurrentTxt, &vPos, D3DXCOLOR(1.f, 1.f, 1.f, 1.f));
        }
    }
}

void CDaveConversation::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
}

HRESULT			CDaveConversation::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

}


CDaveConversation* CDaveConversation::Create(float fPosX, float fPosY)
{
    CDaveConversation* pIDiverUpgrade = new CDaveConversation{ fPosX , fPosY };

    if (FAILED(pIDiverUpgrade->Ready_GameObject()))
    {
        Safe_Release(pIDiverUpgrade);
        MSG_BOX("pIDiverUpgrade Create Failed");
        return nullptr;
    }

    return pIDiverUpgrade;
}

void CDaveConversation::Free()
{
    CGameObject::Free();
}