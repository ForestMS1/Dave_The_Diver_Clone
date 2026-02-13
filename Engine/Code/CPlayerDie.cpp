//#include "pch.h"
#include "CPlayerDie.h"

CPlayerDie::CPlayerDie(CDSPlayer* pPlayer)
    : CPlayerState(pPlayer)
{
}

CPlayerDie::~CPlayerDie()
{
}

void CPlayerDie::Enter()
{
    Clear();
}

_int CPlayerDie::Update_State(const _float& fTimeDelta)
{
    return _int();
}

void CPlayerDie::LateUpdate_State(const _float& fTimeDelta)
{
}

void CPlayerDie::Render_State()
{
}

void CPlayerDie::Exit()
{
}

void CPlayerDie::Clear()
{
}

CPlayerDie* CPlayerDie::Create(CDSPlayer* pPlayer)
{
    CPlayerDie* pState = new CPlayerDie(pPlayer);
    return pState;
}

void CPlayerDie::Free()
{
    CPlayerState::Free();
}