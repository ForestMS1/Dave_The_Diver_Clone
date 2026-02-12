#include "pch.h"
#include "CDiveDaveAttack.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
#include "CHelper.h"
CDiveDaveAttack::CDiveDaveAttack(CGameObject* pOwner)
	:CPlayerState(pOwner)
{
}

CDiveDaveAttack::~CDiveDaveAttack()
{
}

void CDiveDaveAttack::Enter()
{
	static_cast<CDiveDave*>(m_pPlayer)->Init_Frame();
	_float fWidth = 29.f;
	_float fHeight = 54.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fWidth / fAspect, fHeight / fAspect, 1.f };
	static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
	static_cast<CDiveDave*>(m_pPlayer)->Set_TextureCom(L"Com_AttackReadyTexture");
}

void CDiveDaveAttack::Input(const _float& fTimeDelta)
{
	if (CDInputMgr::GetInstance()->Mouse_Up(DIM_LB))
	{
		static_cast<CDiveDave*>(m_pPlayer)->Set_State(DiveState::IDLE);
	}
}

_int CDiveDaveAttack::Update_State(const _float& fTimeDelta)
{
	Input(fTimeDelta);
	static_cast<CDiveDave*>(m_pPlayer)->AddFrame(fTimeDelta, 10.f, 1);
	Mouse_Check();
	return _int();
}

void CDiveDaveAttack::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveAttack::Render_State()
{
	CTexture* pPlayerTextureCom = static_cast<CDiveDave*>(m_pPlayer)->Get_TextureCom();

	_float fFrame = static_cast<CDiveDave*>(m_pPlayer)->Get_Frame();

	pPlayerTextureCom->Set_Texture((_uint)fFrame);
}

void CDiveDaveAttack::Exit()
{
	_float fWidth = 29.f;
	_float fHeight = 54.f;
	_float fAspect = fWidth + fHeight;
	fAspect /= 2.f;

	_vec3 vScale = { fAspect / fWidth, fAspect / fHeight, 1.f };
	static_cast<CDiveDave*>(m_pPlayer)->Multiply_Scale(&vScale);
}

void CDiveDaveAttack::Clear()
{
}

void CDiveDaveAttack::Mouse_Check()
{
	_vec3 vRayPos, vRayDir;
	CHelper::GetMousePointRay(&vRayPos, &vRayDir);
	
	if (vRayDir.x <= 0.f)
	{
		_vec3 vRotateDir = { 0.f, -180.f, 0.f };
		static_cast<CDiveDave*>(m_pPlayer)->Set_RotateDir(&vRotateDir);
	}
	else
	{
		_vec3 vRotateDir = { 0.f, 0.f, 0.f };
		static_cast<CDiveDave*>(m_pPlayer)->Set_RotateDir(&vRotateDir);
	}
}

CDiveDaveAttack* CDiveDaveAttack::Create(CGameObject* pOwner)
{
	CDiveDaveAttack* pState = new CDiveDaveAttack(pOwner);

	return pState;
}

void CDiveDaveAttack::Free()
{
}
