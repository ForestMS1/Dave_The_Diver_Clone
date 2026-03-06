#include "pch.h"
#include "CFinalConversation.h"
#include "CAssetMgr.h"
#include "CGraphicDev.h"
#include "CAssetTexture.h"
#include "CRenderer.h"
#include "CHelper.h"
#include "CAssetDefaultFont.h"
#include "CManagement.h"
#include "CColliderMgr.h"
#include "CDInputMgr.h"
#include "CSoundMgr.h"


CFinalConversation::CFinalConversation(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CFinalConversation::~CFinalConversation()
{
}


HRESULT		CFinalConversation::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_finalConv5"))
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

    m_vecConversation[CONV_1].push_back(L"벌써 한달이 지나 SR시연회가 끝이 나네요.");
    m_vecConversation[CONV_1].push_back(L"마지막으로 한마디씩 하고 마무리 하겠습니다.");
    m_vecConversation[CONV_1].push_back(L"팀장님 한마디");
    m_vecConversation[CONV_1].push_back(L"민수 한마디");
    m_vecConversation[CONV_1].push_back(L"아 예비군");
    m_vecConversation[CONV_1].push_back(L"TextRpg는 꼭 많이 치세요.");
    m_vecConversation[CONV_1].push_back(L"시청해주셔서 감사합니다!");

    m_iCurrentConversationIdx = 0;
    m_eCurrentConversation = CONV_1;

    m_iCurrentTxtIdx = 0;
    m_sCurrentTxt = L"";
    m_sTex = L"Tex_finalConv5";
    return S_OK;
}

_int		CFinalConversation::Update_GameObject(const _float& fTimeDelta)
{
    if (m_iCurrentConversationIdx <= 1) {
        m_sTex = L"Tex_finalConv5";
    }
    else  if (m_iCurrentConversationIdx == 2) {
        m_sTex = L"Tex_finalConv1";
    }
    else if (m_iCurrentConversationIdx == 3) {
        m_sTex = L"Tex_finalConv2";
    }
    else if (m_iCurrentConversationIdx == 4) {
        m_sTex = L"Tex_finalConv3";
    }
    else if (m_iCurrentConversationIdx == 5) {
        m_sTex = L"Tex_finalConv4";
    }
    else if (m_iCurrentConversationIdx == 6) {
        m_sTex = L"Tex_finalConv5";
    }
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

    if (m_fTimer > 0.05f)
    {
        if (m_iCurrentTxtIdx < m_vecConversation[m_eCurrentConversation][m_iCurrentConversationIdx].size())
        {
            ++m_iCurrentTxtIdx;
            if (m_iCurrentTxtIdx % 2 == 0) {
                CSoundMgr::GetInstance()->PlaySoundOne(L"talk", CSoundMgr::SFX, 1.f);

            }

        }
        m_fTimer = 0.f;
    }

    m_sCurrentTxt.assign(m_vecConversation[m_eCurrentConversation][m_iCurrentConversationIdx], 0, m_iCurrentTxtIdx);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void		CFinalConversation::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CFinalConversation::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(m_sTex))
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
        float fOffsetX = -1.8f;
        float fOffsetY = -1.f;
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

void CFinalConversation::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
}

HRESULT			CFinalConversation::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

}


CFinalConversation* CFinalConversation::Create(float fPosX, float fPosY)
{
    CFinalConversation* pDaveConversation = new CFinalConversation{ fPosX , fPosY };

    if (FAILED(pDaveConversation->Ready_GameObject()))
    {
        Safe_Release(pDaveConversation);
        MSG_BOX("pDaveConversation Create Failed");
        return nullptr;
    }

    return pDaveConversation;
}

void CFinalConversation::Free()
{
    CGameObject::Free();
}