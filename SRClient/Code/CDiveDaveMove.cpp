#include "CDiveDaveMove.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
CDiveDaveMove::CDiveDaveMove(CGameObject* pOwner)
	:CPlayerState(pOwner)
{
}

CDiveDaveMove::~CDiveDaveMove()
{
}

void CDiveDaveMove::Enter()
{
	static_cast<CDiveDave*>(m_pPlayer)->Init_Frame();
	_float fWidth = 63.f;
	_float fHeight = 39.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
}

void CDiveDaveMove::Input(const _float& fTimeDelta)
{
	if (CDInputMgr::GetInstance()->Key_Pressing(DIK_W))
	{
		if (CDInputMgr::GetInstance()->Key_Pressing(DIK_A))
			m_eDir = UP_LEFT;
		else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_D))
			m_eDir = UP_RIGHT;
		else
			m_eDir = UP;
	}

	else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_A))
	{
		if (CDInputMgr::GetInstance()->Key_Pressing(DIK_W))
			m_eDir = UP_LEFT;
		else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_S))
			m_eDir = DOWN_LEFT;
		else
			m_eDir = LEFT;
	}

	else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_S))
	{
		if (CDInputMgr::GetInstance()->Key_Pressing(DIK_A))
			m_eDir = DOWN_LEFT;
		else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_D))
			m_eDir = DOWN_RIGHT;
		else
			m_eDir = DOWN;
	}
	else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_D))
	{
		if (CDInputMgr::GetInstance()->Key_Pressing(DIK_W))
			m_eDir = UP_RIGHT;
		else if (CDInputMgr::GetInstance()->Key_Pressing(DIK_S))
			m_eDir = DOWN_RIGHT;
		else
			m_eDir = RIGHT;
	}

	else
	{
		static_cast<CDiveDave*>(m_pPlayer)->Set_State(DiveState::IDLE);
		m_eDir = DIR_END;
	}

}

_int CDiveDaveMove::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	Go_Dir(fTimeDelta);
	static_cast<CDiveDave*>(m_pPlayer)->AddFrame(fTimeDelta, 10.f, 8);

	return _int();
}

void CDiveDaveMove::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveMove::Render_State()
{
	CTexture* pPlayerTextureCom = static_cast<CDiveDave*>(m_pPlayer)->Get_TextureCom();

	_float fFrame = static_cast<CDiveDave*>(m_pPlayer)->Get_Frame();

	pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CDiveDaveMove::Exit()
{
	_float fWidth = 63.f;
	_float fHeight = 39.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);

	_vec3 vRotateDir = { 0.f, 0.f, 0.f };
	static_cast<CDiveDave*>(m_pPlayer)->Set_RotateDir(&vRotateDir);
}

void CDiveDaveMove::Clear()
{
}

void CDiveDaveMove::Go_Dir(const _float& fTimeDelta)
{
	CDiveDave* pPlayer = static_cast<CDiveDave*>(m_pPlayer);
	_vec3 vDir;
	_vec3 vRotateDir;

	switch (m_eDir)
	{
	case UP:
		vDir = { 0.f, 1.f, 0.f };
		pPlayer->Move(&vDir, fTimeDelta);
		pPlayer->Set_TextureCom(L"Com_MoveUpTexture");
		vRotateDir = { 0.f, 0.f, 90.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case UP_RIGHT:
		vDir = { 1.f, 1.f, 0.f };
		D3DXVec3Normalize(&vDir, &vDir);
		pPlayer->Move(&vDir, fTimeDelta);
		pPlayer->Set_TextureCom(L"Com_MoveSideUpTexture");
		vRotateDir = { 0.f, 0.f, 45.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case UP_LEFT:
		vDir = { -1.f, 1.f, 0.f };
		D3DXVec3Normalize(&vDir, &vDir);
		pPlayer->Move(&vDir, fTimeDelta);
		pPlayer->Set_TextureCom(L"Com_MoveSideUpTexture");
		vRotateDir = { 0.f, -180.f, -45.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case RIGHT:
		vDir = { 1.f, 0.f, 0.f };
		pPlayer->Move(&vDir, fTimeDelta);
		pPlayer->Set_TextureCom(L"Com_MoveSideTexture");
		vRotateDir = { 0.f, 0.f, 0.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case LEFT:
		vDir = { -1.f, 0.f, 0.f };
		pPlayer->Move(&vDir, fTimeDelta);
		pPlayer->Set_TextureCom(L"Com_MoveSideTexture");
		vRotateDir = { 0.f, -180.f, 0.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case DOWN:
		vDir = { 0.f, -1.f, 0.f };
		pPlayer->Move(&vDir, fTimeDelta);
		pPlayer->Set_TextureCom(L"Com_MoveDownTexture");
		vRotateDir = { 0.f, 0.f, -90.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case DOWN_RIGHT:
		vDir = { 1.f, -1.f, 0.f };
		D3DXVec3Normalize(&vDir, &vDir);
		pPlayer->Move(&vDir, fTimeDelta);
		pPlayer->Set_TextureCom(L"Com_MoveSideDownTexture");
		vRotateDir = { 0.f, 0.f, -45.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case DOWN_LEFT:
		vDir = { -1.f, -1.f, 0.f };
		D3DXVec3Normalize(&vDir, &vDir);
		pPlayer->Move(&vDir, fTimeDelta);
		pPlayer->Set_TextureCom(L"Com_MoveSideDownTexture");
		vRotateDir = { 0.f, -180.f, 45.f };
		pPlayer->Set_RotateDir(&vRotateDir);
		break;
	case DIR_END:
		break;
	default:
		break;
	}
}

CDiveDaveMove* CDiveDaveMove::Create(CGameObject* pOwner)
{
	CDiveDaveMove* pState = new CDiveDaveMove(pOwner);

	return pState;
}

void CDiveDaveMove::Free()
{
}
