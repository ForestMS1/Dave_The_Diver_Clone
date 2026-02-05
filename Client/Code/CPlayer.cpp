#include "pch.h"
#include "CPlayer.h"
#include "CProtoMgr.h"
#include "CRenderer.h"
#include "CManagement.h"
#include "CDInputMgr.h"
#include "CPlayerCam.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
	, m_pBufferCom(nullptr)
	, m_pCalculatorCom(nullptr)
	, m_pTextureCom(nullptr)
	, m_pTransformCom(nullptr)
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
	if (FAILED(Add_Component()))
		return E_FAIL;

 

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	Mouse_Move();
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
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_World());

	m_pTextureCom->Set_Texture(0);

	m_pBufferCom->Render_Buffer();

#ifdef _DEBUG
	_vec3 vPos, vLook;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);
	m_pTransformCom->Get_Info(INFO_LOOK, &vLook);
	ImGui::Begin("Player");
	ImGui::DragFloat3("Pos", (float*)&vPos, 1.f - 1.f, 1.f);
	ImGui::DragFloat3("Look", (float*)&vLook, 1.f - 1.f, 1.f);
	ImGui::End();
#endif
}

HRESULT CPlayer::Add_Component()
{
	Engine::CComponent* pComponent = nullptr;

	// buffer 
	pComponent = m_pBufferCom = 
		dynamic_cast<Engine::CRcTex*>
		(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_RcTex"));
	if (nullptr == pComponent)
		return E_FAIL;

	m_mapComponent[ID_STATIC].insert({ L"Com_Buffer", pComponent});

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

void CPlayer::Key_Input(const _float& fTimeDelta)
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
}

void CPlayer::Mouse_Move()
{
	_long   dwMouseMove(0);

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
	_vec3		vPos;
	m_pTransformCom->Get_Info(INFO_POS, &vPos);

	Engine::CTerrainTex* pTerrainVtxCom = dynamic_cast<Engine::CTerrainTex*>
		(CManagement::GetInstance()->Get_FirstObjectComponent(ID_STATIC, L"GameLogic_Layer", L"Terrain", L"Com_Buffer"));

	if (nullptr == pTerrainVtxCom)
		return;

	_float	fHeight = m_pCalculatorCom->Compute_HeightOnTerrain(&vPos, pTerrainVtxCom->Get_VtxPos(), VTXCNTX, VTXCNTZ);

	m_pTransformCom->Set_Pos(vPos.x, fHeight + 1.f, vPos.z);
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPlayer* pBackGround = new CPlayer(pGraphicDev);

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
