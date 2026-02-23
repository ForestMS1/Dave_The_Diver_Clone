#include "CAttackSubState.h"
#include "CDiveDaveAttack.h"
CAttackSubState::CAttackSubState(CDiveDaveAttack* pParentState)
	: CBaseState<CDiveDaveAttack>(pParentState)
{
	m_pDiveDave = m_pOwner->Get_OwnerDave();
}

CAttackSubState::~CAttackSubState()
{
}

void CAttackSubState::Free()
{
}
