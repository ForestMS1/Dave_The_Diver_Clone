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
#include "CSoundMgr.h"
#include "CGameMemMgr.h"
#include "CTimerMgr.h"



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
	m_fAppear = 0.f;
 /*   m_vecConversation[CONV_1].push_back(L"선생님");
    m_vecConversation[CONV_1].push_back(L"저 사실\n...\n...\n...");
    m_vecConversation[CONV_1].push_back(L"중퇴가 하고싶어요");*/
    m_vecConversation[CONV_1].push_back(L"하아...\n날씨는 맑은데...");
    m_vecConversation[CONV_1].push_back(L"내 통장은 비가 오는구나...");
    m_vecConversation[CONV_1].push_back(L"부자가 되서 이 일도 그만하고 싶다...");
    m_vecConversation[CONV_1].push_back(L"이 일로는 부자는 어림도 없으니\n장비 몇개 강화하고 주식이나 해볼까?");
    m_vecConversation[CONV_1].push_back(L"맞아... 누군가가 지금은 국장을 넣으라고 할때라고 했어...");
    m_vecConversation[CONV_1].push_back(L"믿습니다...제발!!!!!!");

    m_vecConversation[CONV_2].push_back(L"으흐흐...\n어디~ 돈이 얼마나 불어있는지 볼까?");
    m_vecConversation[CONV_2].push_back(L"역시 말을 잘들어야 된다니까~");

    m_vecConversation[CONV_3].push_back(L"화성 갈끄니까!!");

    m_vecConversation[CONV_4].push_back(L"후.. 화성에 도착했나?");
    m_vecConversation[CONV_4].push_back(L"음??????????");

    m_vecConversation[CONV_5].push_back(L"어디 전쟁이라도 났나?");
    m_vecConversation[CONV_5].push_back(L"XX!!!!!!!!!!!!");

    m_vecConversation[BOSS_1].push_back(L"뭐라고 하는거야?");
    m_vecConversation[BOSS_1].push_back(L"말도 안되는소리 하지말고 당장 사라져!");

    m_vecConversation[BOSS_2].push_back(L"정말이지 짜증나게 하고있네.");
    m_vecConversation[BOSS_2].push_back(L"죽어.");
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

    if (m_fTimer > 0.05f)
    {
        if (m_iCurrentTxtIdx < m_vecConversation[m_eCurrentConversation][m_iCurrentConversationIdx].size())
        {
            ++m_iCurrentTxtIdx;
            if (m_iCurrentTxtIdx % 2 == 0) {
                CSoundMgr::GetInstance()->PlaySoundOne(L"talk", CSoundMgr::SFX_DAVETALK1, 1.f);

            }

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

    if (m_eCurrentConversation == CONV_2 || m_eCurrentConversation == CONV_4) {
        m_fAppear += CTimerMgr::GetInstance()->Get_TimeDelta(L"Timer_FPS60");
        if (m_fAppear > 2.5f) {
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
    }
    else {
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
    CDaveConversation* pDaveConversation = new CDaveConversation{ fPosX , fPosY };

    if (FAILED(pDaveConversation->Ready_GameObject()))
    {
        Safe_Release(pDaveConversation);
        MSG_BOX("pDaveConversation Create Failed");
        return nullptr;
    }

    return pDaveConversation;
}

void CDaveConversation::Free()
{
    CGameObject::Free();
}