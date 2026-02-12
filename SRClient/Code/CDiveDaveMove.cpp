#include "CDiveDaveMove.h"

CDiveDaveMove::CDiveDaveMove(CGameObject* pOwner)
	:CPlayerState(pOwner)
{
}

CDiveDaveMove::~CDiveDaveMove()
{
}

void CDiveDaveMove::Enter()
{
}

void CDiveDaveMove::Input()
{

}

_int CDiveDaveMove::Update_State(const _float& fTimeDelta)
{
	Input();
	return _int();
}

void CDiveDaveMove::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveMove::Render_State()
{
}

void CDiveDaveMove::Exit()
{
}

void CDiveDaveMove::Clear()
{
}

CDiveDaveMove* CDiveDaveMove::Create(CGameObject* pOwner)
{
	CDiveDaveMove* pState = new CDiveDaveMove(pOwner);

	return pState;
}

void CDiveDaveMove::Free()
{
}
