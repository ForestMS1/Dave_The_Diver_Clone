#include "CDiveDaveAttack.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
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
	return _int();
}

void CDiveDaveAttack::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveAttack::Render_State()
{
}

void CDiveDaveAttack::Exit()
{
}

void CDiveDaveAttack::Clear()
{
}

CDiveDaveAttack* CDiveDaveAttack::Create(CGameObject* pOwner)
{
	CDiveDaveAttack* pState = new CDiveDaveAttack(pOwner);

	return pState;
}

void CDiveDaveAttack::Free()
{
}
