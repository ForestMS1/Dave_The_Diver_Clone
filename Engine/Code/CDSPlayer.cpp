#include "CDSPlayer.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include "CPlayerCam.h"

#include "CPlayerState.h"
#include "CPlayerIdle.h"
#include "CPlayerAttack.h"
#include "CPlayerDie.h"
#include "CGraphicDev.h"
#include "CCameraMgr.h"
#ifdef _DEBUG
// Imgui µð¹ö±ë¿ë
const char* enum_names[] = { "IDLE", "ATTCK", "DIE" };
#endif

CDSPlayer::CDSPlayer()
	: m_pBufferCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
	, m_eCurState(PlayerState::IDLE)
{
}

CDSPlayer::CDSPlayer(const CGameObject& rhs)
	: CGameObject(rhs)
{
}

CDSPlayer::~CDSPlayer()
{
}

HRESULT CDSPlayer::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	if (FAILED(Add_State()))
		return E_FAIL;

	m_pState = m_mapState[PlayerState::IDLE];
	m_eCurState = PlayerState::IDLE;

	return S_OK;
}

_int CDSPlayer::Update_GameObject(const _float& fTimeDelta)
{
	CRenderer::GetInstance()->Add_RenderGroup(RENDER_ALPHA, this);

	Key_Input(fTimeDelta);
	Mouse_Move();
	Set_Cam();
	_int iExit = CGameObject::Update_GameObject(fTimeDelta);

	m_pState->Update_State(fTimeDelta);

	return iExit;
}

void CDSPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	m_pState->LateUpdate_State(fTimeDelta);
}

void CDSPlayer::Render_GameObject()
{
	LPDIRECT3DDEVICE9 pGraphicDev = CGraphicDev::GetInstance()->Get_GraphicDev();

	pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

	m_pState->Render_State();

#ifdef _DEBUG
	_vec3 vPos, vLook;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	ImGui::Begin("Player");
	ImGui::DragFloat3("Pos", (float*)&vPos, 1.f - 1.f, 1.f);
	ImGui::DragFloat3("Look", (float*)&vLook, 1.f - 1.f, 1.f);
	ImGui::Text(enum_names[(_int)m_eCurState]);
	ImGui::End();
#endif
}

HRESULT CDSPlayer::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// buffer 
	pComponent = m_pBufferCom =
		dynamic_cast<Engine::CRcTex*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent });

	// texture 
	pComponent = m_pTextureCom =
		dynamic_cast<Engine::CTexture*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_PlayerTexture"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Texture", pComponent });

	// Transform
	pComponent = m_pTransformCom =
		dynamic_cast<Engine::CTransform*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

	// Calculator 
	pComponent = m_pCalculatorCom =
		dynamic_cast<Engine::CCalculator*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Calculator"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Calculator", pComponent });

	return S_OK;
}

HRESULT CDSPlayer::Add_State()
{
	m_mapState.insert({ PlayerState::IDLE, CPlayerIdle::Create(this)});
	m_mapState.insert({ PlayerState::ATTACK, CPlayerAttack::Create(this)});
	m_mapState.insert({ PlayerState::DIE, CPlayerDie::Create(this)});

	return S_OK;
}

void CDSPlayer::Key_Input(const _float& fTimeDelta)
{
	_vec3		vDir, vRight;
	_vec3		vUp(0.f, 1.f, 0.f);
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

	if (CDInputMgr::GetInstance()->Mouse_Down(DIM_LB))
	{
		Set_State(PlayerState::ATTACK);
	}
}

void CDSPlayer::Mouse_Move()
{
	_long   dwMouseMove(0);

	if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_Y))
		m_pTransformCom->Rotation(ROT_X, dwMouseMove / 10.f);

	if (dwMouseMove = CDInputMgr::GetInstance()->Get_DIMouseMove(DIMS_X))
		m_pTransformCom->Rotation(ROT_Y, dwMouseMove / 10.f);
}
void CDSPlayer::Set_Cam()
{
	CPlayerCam* pPlayerCam = dynamic_cast<CPlayerCam*>(CCameraMgr::GetInstance()->Get_CurCamera());
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

void CDSPlayer::Set_OnTerrain()
{
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex* pTerrainVtxCom = dynamic_cast<Engine::CTerrainTex*>
		(CManagement::GetInstance()->Get_FirstObjectComponent(ID_STATIC, L"GameLogic_Layer", L"Terrain", L"Com_Buffer"));

	if (nullptr == pTerrainVtxCom)
		return;

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainVtxCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight + 1.f, vPos.z);
}

void CDSPlayer::Set_State(PlayerState state)
{
	if (m_mapState[state] == m_pState)
		return;

	m_pState->Exit();

	m_pState = m_mapState[state];
	m_eCurState = state;

	m_pState->Enter();
}

CDSPlayer* CDSPlayer::Create()
{
	CDSPlayer* pBackGround = new CDSPlayer;

	if (FAILED(pBackGround->Ready_GameObject()))
	{
		Safe_Release(pBackGround);
		MSG_BOX("DSPlayer Create Failed");
		return nullptr;
	}

	return pBackGround;
}

void CDSPlayer::Free()
{
	for_each(m_mapState.begin(), m_mapState.end(), CDeleteMap());
	m_mapState.clear();
	CGameObject::Free();
}
