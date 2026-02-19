#include "CAttackSubState.h"

CAttackSubState::CAttackSubState(CGameObject* pPlayer, CDiveDaveAttack* pParentState)
	: m_pPlayer(pPlayer)
	, m_pParentState(pParentState)
{
}

CAttackSubState::~CAttackSubState()
{
}

void CAttackSubState::Free()
{
}
