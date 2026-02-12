#include "pch.h"
#include "CDiveDaveIdle.h"
#include "CDInputMgr.h"

CDiveDaveIdle::CDiveDaveIdle(CGameObject* pOwner)
	:CPlayerState(pOwner)
{
}

CDiveDaveIdle::~CDiveDaveIdle()
{
}

void CDiveDaveIdle::Enter()
{
}

void CDiveDaveIdle::Input()
{
	
}

_int CDiveDaveIdle::Update_State(const _float& fTimeDelta)
{
	Input();
	return _int();
}

void CDiveDaveIdle::LateUpdate_State(const _float& fTimeDelta)
{
}

void CDiveDaveIdle::Render_State()
{
}

void CDiveDaveIdle::Exit()
{
}

void CDiveDaveIdle::Clear()
{
}

CDiveDaveIdle* CDiveDaveIdle::Create(CGameObject* pOwner)
{
	CDiveDaveIdle* pState = new CDiveDaveIdle(pOwner);

	return pState;
}

void CDiveDaveIdle::Free()
{
}
