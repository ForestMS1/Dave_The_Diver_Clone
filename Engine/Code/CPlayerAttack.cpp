//#include "pch.h"
#include "CPlayerAttack.h"
#include "CDSPlayer.h"
CPlayerAttack::CPlayerAttack(CDSPlayer* pPlayer)
    : CPlayerState(pPlayer)
    , m_fAttackTime(0.f), m_fMaxAttakcTime(0.02f)
{
}

CPlayerAttack::~CPlayerAttack()
{
}

void CPlayerAttack::Enter()
{
    Clear();
}

_int CPlayerAttack::Update_State(const _float& fTimeDelta)
{
    m_fAttackTime += fTimeDelta;

    if (m_fMaxAttakcTime <= m_fAttackTime)
    {
        m_pPlayer->Set_State(PlayerState::IDLE);
    }
    return _int();
}

void CPlayerAttack::LateUpdate_State(const _float& fTimeDelta)
{
}

void CPlayerAttack::Render_State()
{
}

void CPlayerAttack::Exit()
{
}

void CPlayerAttack::Clear()
{
    m_fAttackTime = 0.f;
}

CPlayerAttack* CPlayerAttack::Create(CDSPlayer* pPlayer)
{
    CPlayerAttack* pState = new CPlayerAttack(pPlayer);
    return pState;
}

void CPlayerAttack::Free()
{
    CPlayerState::Free();
}