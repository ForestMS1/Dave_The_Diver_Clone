#include "pch.h"
#include "CSushiDave.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CDInputMgr.h"


CSushiDave::CSushiDave()
    : CGameObject()
{
    curState = IDLE;
    holdingSushi = false;
    curDir = LEFT;
    prevDir = LEFT;
}

CSushiDave::CSushiDave(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CSushiDave::~CSushiDave()
{
}

HRESULT CSushiDave::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;

    m_fFrame = 0.f;


    return S_OK;
}

_int CSushiDave::Update_GameObject(const _float& fTimeDelta)
{
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    switch (curState)
    {
    case IDLE:
        m_fFrame += 2.f * fTimeDelta;
        if (2.f < m_fFrame)
            m_fFrame = 0.f;

        break;
    case SUSHI_IDLE:
        m_fFrame += 1.f * fTimeDelta;
        if (1.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    default:
        m_fFrame += 8.f * fTimeDelta;
        if (8.f < m_fFrame)
            m_fFrame = 0.f;
        break;
    }

 

    return iExit;
}

void CSushiDave::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    _vec3		vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Compute_ViewZ(&vPos);
    Key_Input(fTimeDelta);
    if (curDir != prevDir) {
        m_pTransformCom->Rotation(ROT_Y, 180.f);
    }
    prevDir = curDir;
}

void CSushiDave::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    switch (curState)
    {
    case IDLE:
        m_pIdleTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case WALK:
        m_pWalkTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case RUN:
        m_pRunTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case TIRED:
        m_pTiredTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case SUSHI_IDLE:
        m_pSushiIdleTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case SUSHI_WALK:
        m_pSushiWalkTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case SUSHI_RUN:
        m_pSushiRunTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    case SUSHI_TIRED:
        m_pSushiTiredTextureCom->Set_Texture((_uint)m_fFrame);
        break;
    }

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CSushiDave::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;

    // 스시 없을때
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveIdleTexture", L"Com_Texture", &m_pIdleTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveWalkTexture", L"Com_Texture", &m_pWalkTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveRunTexture", L"Com_Texture", &m_pRunTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveTiredTexture", L"Com_Texture", &m_pTiredTextureCom))))
        return E_FAIL;

    //스시 들고 있을때
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveSushiIdleTex", L"Com_Texture", &m_pSushiIdleTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveSushiWalkTexture", L"Com_Texture", &m_pSushiWalkTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveSushiRunTexture", L"Com_Texture", &m_pSushiRunTextureCom))))
        return E_FAIL;
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_DaveSushiTiredTexture", L"Com_Texture", &m_pSushiTiredTextureCom))))
        return E_FAIL;
    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    m_pTransformCom->m_vScale = { 1.f, 1.5f, 1.f };
    m_pTransformCom->m_vInfo[INFO_POS] = { 8.8f, -2.0f, 0.f };
    return S_OK;
}

void CSushiDave::Key_Input(const _float& fTimeDelta)
{
    bool bMove = false;
    if (!holdingSushi) {
        if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_A))
        {
            _vec3 left = { -1,0,0 };
            curState = WALK;
            curDir = LEFT;
            m_pTransformCom->Move_Pos(&left, 2.f, fTimeDelta);
            bMove = true;
        }

        if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_D))
        {
            _vec3 right = { 1,0,0 };
            curState = WALK;
            curDir = RIGHT;
            m_pTransformCom->Move_Pos(&right, 2.f, fTimeDelta);
            bMove = true;
        }
        if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
        {
            curState = RUN;
            if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_A))
            {
                _vec3 left = { -1,0,0 };
                curDir = LEFT;
                m_pTransformCom->Move_Pos(&left, 2.5f, fTimeDelta);
                bMove = true;
            }
            if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_D))
            {
                _vec3 right = { 1,0,0 };
                curDir = RIGHT;
                m_pTransformCom->Move_Pos(&right, 2.5f, fTimeDelta);
                bMove = true;
            }
        }
    }
    else {
        if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_A))
        {
            _vec3 left = { -1,0,0 };
            curState = SUSHI_WALK;
            curDir = LEFT;
            m_pTransformCom->Move_Pos(&left, 2.f, fTimeDelta);
            bMove = true;
        }

        if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_D))
        {
            _vec3 right = { 1,0,0 };
            curState = SUSHI_WALK;
            curDir = RIGHT;
            m_pTransformCom->Move_Pos(&right, 2.f, fTimeDelta);
            bMove = true;
        }
        if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_LSHIFT))
        {
            curState = SUSHI_RUN;
            if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_A))
            {
                _vec3 left = { -1,0,0 };
                curDir = LEFT;
                m_pTransformCom->Move_Pos(&left, 2.5f, fTimeDelta);
                bMove = true;
            }
            if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_D))
            {
                _vec3 right = { 1,0,0 };
                curDir = RIGHT;
                m_pTransformCom->Move_Pos(&right, 2.5f, fTimeDelta);
                bMove = true;
            }
        }
    }
   
    if (!bMove) {
        m_fFrame = 0;
        curState = IDLE;
    }
}
CSushiDave* CSushiDave::Create()
{
    CSushiDave* pBackGround = new CSushiDave;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("Dave Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CSushiDave::Free()
{
    CGameObject::Free();
    Safe_Release(m_pIdleTextureCom);
    Safe_Release(m_pWalkTextureCom);
    Safe_Release(m_pRunTextureCom);
    Safe_Release(m_pTiredTextureCom);
    Safe_Release(m_pSushiIdleTextureCom);
    Safe_Release(m_pSushiRunTextureCom);
    Safe_Release(m_pSushiTiredTextureCom);
    Safe_Release(m_pSushiWalkTextureCom);
}
