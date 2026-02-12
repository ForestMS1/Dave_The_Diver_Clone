#include "CDiveDaveAttack.h"

CDiveDaveAttack::CDiveDaveAttack(CGameObject* pOwner)
	:CPlayerState(pOwner)
{
}

CDiveDaveAttack::~CDiveDaveAttack()
{
}

void CDiveDaveAttack::Enter()
{
}

void CDiveDaveAttack::Input()
{

}

_int CDiveDaveAttack::Update_State(const _float& fTimeDelta)
{
	Input();
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
