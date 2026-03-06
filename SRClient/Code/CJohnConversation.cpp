#include "pch.h"
#include "CJohnConversation.h"
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


CJohnConversation::CJohnConversation(float fPosX, float fPosY)
    : CGameObject()
    , m_fPosX(fPosX)
    , m_fPosY(fPosY)
{
}

CJohnConversation::~CJohnConversation()
{
}


HRESULT		CJohnConversation::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;


    _vec3 vScale = { 1.f , 1.f, 1.f };
    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnConversation"))
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

    /*   m_vecConversation[CONV_1].push_back(L"선생님");
       m_vecConversation[CONV_1].push_back(L"저 사실\n...\n...\n...");
       m_vecConversation[CONV_1].push_back(L"중퇴가 하고싶어요");*/
    m_vecConversation[CONV_1].push_back(L"왔군. 나는 존 왓슨, 이 바다의 균형을 지키는 방위대장이다.");
    m_vecConversation[CONV_1].push_back(L"너를 몇일간 지켜봤지만\n너는 이 바다의 규칙을 어기고있다.");
    m_vecConversation[CONV_1].push_back(L"하루 인당 포획량은 10kg이지만\n너는 항상 그 이상을 가져가더군.");
    m_vecConversation[CONV_1].push_back(L"긴말은 안하겠다.");
    m_vecConversation[CONV_1].push_back(L"앞으로 규정을 지키겠다고 약속을 하면\n지금까지의 일은 없던걸로 해주겠다.");

    m_vecConversation[CONV_2].push_back(L"말이 안통하는군.");
    m_vecConversation[CONV_2].push_back(L"어쩔수 없지.\n제압하겠다.");

    m_vecConversation[CONV_3].push_back(L"평범한 다이버 주제에 제법이군.");
    m_vecConversation[CONV_3].push_back(L"진심으로 상대해주지.");

    m_iCurrentConversationIdx = 0;
    m_eCurrentConversation = CONV_1;

    m_iCurrentTxtIdx = 0;
    m_sCurrentTxt = L"";

    return S_OK;
}

_int		CJohnConversation::Update_GameObject(const _float& fTimeDelta)
{
    if (CDInputMgr::GetInstance()->Key_Down(DIK_SPACE))
    {
        ++m_iCurrentConversationIdx;
        if (m_iCurrentConversationIdx == m_vecConversation[m_eCurrentConversation].size())
        {
            Set_DeadCascade();
            return OBJ_DEAD;
        }
        else {
            CSoundMgr::GetInstance()->PlaySoundOne(L"bosstalk", CSoundMgr::SFX, 1.f);
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
            /*if (m_iCurrentTxtIdx % 12 == 0) {

            }*/
        }
        m_fTimer = 0.f;
    }

    m_sCurrentTxt.assign(m_vecConversation[m_eCurrentConversation][m_iCurrentConversationIdx], 0, m_iCurrentTxtIdx);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA_AFTER_ORTHO_UI, this);

    return iExit;
}

void		CJohnConversation::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void		CJohnConversation::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    if (auto vecAsset = CAssetMgr::GetInstance()->Get_Asset(L"Tex_JohnConversation"))
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

void CJohnConversation::Update_ImGui()
{
    CGameObject::Update_ImGui();

    ImGui::DragFloat("m_DbgX", &m_fDbgX, 0.01);
    ImGui::DragFloat("m_DbgY", &m_fDbgY, 0.01);
}

HRESULT			CJohnConversation::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

}


CJohnConversation* CJohnConversation::Create(float fPosX, float fPosY)
{
    CJohnConversation* pJohnConversation = new CJohnConversation{ fPosX , fPosY };

    if (FAILED(pJohnConversation->Ready_GameObject()))
    {
        Safe_Release(pJohnConversation);
        MSG_BOX("pJohnConversation Create Failed");
        return nullptr;
    }
    CSoundMgr::GetInstance()->PlaySoundOne(L"bosstalk", CSoundMgr::SFX, 1.f);

    return pJohnConversation;
}

void CJohnConversation::Free()
{
    CGameObject::Free();
}