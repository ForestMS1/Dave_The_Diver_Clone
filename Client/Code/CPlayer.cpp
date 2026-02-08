#include "pch.h"
#include "CPlayer.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include "CParticleMgr.h"
#include "Engine_Define.h"
#include "CGraphicDev.h"
#include "CPlayerCam.h"
CPlayer::CPlayer()
    : CGameObject()
{
}

CPlayer::CPlayer(const CGameObject& rhs)
    : CGameObject(rhs)
{
}

CPlayer::~CPlayer()
{
}

HRESULT CPlayer::Ready_GameObject()
{
    if (FAILED(Ready_Component()))
        return E_FAIL;



    return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
    Key_Input(fTimeDelta);
    //Mouse_Move();
    Set_Cam();
    _int iExit = CGameObject::Update_GameObject(fTimeDelta);

    CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

    return iExit;
}

void CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CPlayer::Render_GameObject()
{
    LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);



    pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

    m_pTextureCom->Set_Texture(0);

    m_pBufferCom->Render_Buffer();

    D3DXMATRIX matTmp;
    D3DXMatrixIdentity(&matTmp);
    pGraphicDev->SetTransform(D3DTS_WORLD, &matTmp);

    //m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

HRESULT CPlayer::Ready_Component()
{
    // 버퍼
    if (FAILED((AddComponent<Engine::CRcTex, ID_STATIC>(L"Proto_RcTex", L"Com_Buffer", &m_pBufferCom))))
        return E_FAIL;
    
    // 텍스쳐
    if (FAILED((AddComponent<Engine::CTexture, ID_STATIC>(L"Proto_PlayerTexture", L"Com_Texture", &m_pTextureCom))))
        return E_FAIL;

    // 트랜스폼
    if (FAILED((AddComponent<Engine::CTransform, ID_DYNAMIC>(L"Proto_Transform", L"Com_Transform", &m_pTransformCom))))
        return E_FAIL;

    // 칼큐레다
    if (FAILED((AddComponent<Engine::CCalculator, ID_STATIC>(L"Proto_Calculator", L"Com_Calculator", &m_pCalculatorCom))))
        return E_FAIL;

    return S_OK;
}

void CPlayer::Key_Input(const _float& fTimeDelta)
{

    _vec3      vDir, vRight;
    _vec3      vUp(0.f, 1.f, 0.f);
    m_pTransformCom->Get_Info(INFO_LOOK, &vDir);
    D3DXVec3Cross(&vRight, &vDir, &vUp);
    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_W))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDir, &vDir), 10.f, fTimeDelta);
    }

    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_S))
    {
        m_pTransformCom->Move_Pos(D3DXVec3Normalize(&vDir, &vDir), -10.f, fTimeDelta);
    }

    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_A))
    {
        m_pTransformCom->Move_Pos(&vRight, 10.f, fTimeDelta);
    }

    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_D))
    {
        m_pTransformCom->Move_Pos(&vRight, -10.f, fTimeDelta);
    }

    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_Q))
    {
        m_pTransformCom->Move_Pos(&vUp, 10.f, fTimeDelta);
    }
    if (CDInputMgr::GetInstance()->Get_DIKeyState(DIKEYBOARD_E))
    {
        m_pTransformCom->Move_Pos(&vUp, -10.f, fTimeDelta);
    }


    _long dwMouseMove(0);
    if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_Y))
        m_pTransformCom->Rotation(ROT_X, dwMouseMove / 10.f);

    if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_X))
        m_pTransformCom->Rotation(ROT_Y, dwMouseMove / 10.f);
}
void CPlayer::Set_Cam()
{
    CPlayerCam* pPlayerCam = static_cast<CPlayerCam*>(CManagement::GetInstance()->Get_Scene()->Get_Layer(L"Environment_Layer")->Get_GameObjectFirst(L"PlayerCam"));
    if (pPlayerCam == nullptr)
        return;

    _vec3 vPos, vLook;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);
    m_pTransformCom->Get_Info(INFO_LOOK, &vLook);

    _vec3 vEye, vAt;
    vEye = vPos + vLook * 1.2f;
    vAt = vEye + vLook;
    pPlayerCam->Set_vEye(&vEye);
    pPlayerCam->Set_vAt(&vAt);
}

void CPlayer::Set_OnTerrain()
{
    _vec3      vPos;
    m_pTransformCom->Get_Info(INFO_POS, &vPos);

    Engine::CTerrainTex* pTerrainVtxCom = dynamic_cast<Engine::CTerrainTex*>
        (CManagement::GetInstance()->Get_FirstObjectComponent(ID_STATIC, L"GameLogic_Layer", L"Terrain", L"Com_Buffer"));

    if (nullptr == pTerrainVtxCom)
        return;

    _float   fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainVtxCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

    m_pTransformCom->Set_Pos(vPos.x, fHeight + 1.f, vPos.z);
}

CPlayer* CPlayer::Create()
{
    CPlayer* pBackGround = new CPlayer;

    if (FAILED(pBackGround->Ready_GameObject()))
    {
        Safe_Release(pBackGround);
        MSG_BOX("pBackGround Create Failed");
        return nullptr;
    }

    return pBackGround;
}

void CPlayer::Free()
{
    CGameObject::Free();
}
