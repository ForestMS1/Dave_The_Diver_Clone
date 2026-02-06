//#include "pch.h"
#include "CPlayerIdle.h"

CPlayerIdle::CPlayerIdle(CDSPlayer* pPlayer)
	: CPlayerState(pPlayer)
{
}

CPlayerIdle::~CPlayerIdle()
{
}

void CPlayerIdle::Enter()
{
	Clear();
}

_int CPlayerIdle::Update_State(const _float& fTimeDelta)
{
	return _int();
}

void CPlayerIdle::LateUpdate_State(const _float& fTimeDelta)
{
}

void CPlayerIdle::Render_State()
{
}

void CPlayerIdle::Exit()
{
}

void CPlayerIdle::Clear()
{
}

CPlayerIdle* CPlayerIdle::Create(CDSPlayer* pPlayer)
{
	CPlayerIdle* pState = new CPlayerIdle(pPlayer);
	return pState;
}

void CPlayerIdle::Free()
{
	CPlayerState::Free();
}
