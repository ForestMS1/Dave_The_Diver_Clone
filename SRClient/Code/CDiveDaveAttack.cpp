#include "pch.h"
#include "CDiveDaveAttack.h"
#include "CDInputMgr.h"
#include "CDiveDave.h"
#include "CHelper.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CAttackSubReady.h"
#include "CAttackSubFire.h"

string debugSubState[(_uint)ATTACKSUBSTATE::SUB_END] = { "ATTACK_READY", "ATTACK_FIRE", "ATTACK_FIGHT", "ATTACK_FAIL" };


CDiveDaveAttack::CDiveDaveAttack(CGameObject* pOwner)
	:CPlayerState(pOwner)
	,m_eCurSubState(ATTACKSUBSTATE::SUB_END)
{
}

CDiveDaveAttack::~CDiveDaveAttack()
{
}

void CDiveDaveAttack::Enter()
{
	m_eCurSubState = ATTACKSUBSTATE::ATTACK_READY;
	m_pSubState = m_mapSubState[m_eCurSubState];
	m_pSubState->Enter();
}

void CDiveDaveAttack::Input(const _float& fTimeDelta)
{

}

_int CDiveDaveAttack::Update_State(const _float& fTimeDelta)
{
	m_pSubState->Update_State(fTimeDelta);
	return 0;
}

void CDiveDaveAttack::LateUpdate_State(const _float& fTimeDelta)
{
	m_pSubState->LateUpdate_State(fTimeDelta);
	ImGui::Begin("DiveDave Info");
	string state = "AttackSubState : " + debugSubState[(_uint)m_eCurSubState];
	ImGui::Text(state.c_str());
	ImGui::End();
}

void CDiveDaveAttack::Render_State()
{
	m_pSubState->Render_State();
}

void CDiveDaveAttack::Exit()
{
	m_pSubState->Exit();
}

void CDiveDaveAttack::Clear()
{
	
}

void CDiveDaveAttack::Set_State(ATTACKSUBSTATE state)
{
	if (m_mapSubState[state] == m_pSubState)
		return;

	if (m_pSubState != nullptr)
		m_pSubState->Exit();

	m_pSubState = m_mapSubState[state];
	m_eCurSubState = state;

	m_pSubState->Enter();
}


void CDiveDaveAttack::Add_SubState()
{
	m_mapSubState.insert({ ATTACKSUBSTATE::ATTACK_READY, CAttackSubReady::Create(m_pPlayer, this) });
	m_mapSubState.insert({ ATTACKSUBSTATE::ATTACK_FIRE, CAttackSubFire::Create(m_pPlayer, this) });
	//m_mapSubState.insert({ ATTACKSUBSTATE::ATTACK_FIGHT,  CAttackSubFight::Create(m_pPlayer, this) });
	//m_mapSubState.insert({ ATTACKSUBSTATE::ATTACK_FAIL, CAttackSubFail::Create(m_pPlayer, this) });
}

CDiveDaveAttack* CDiveDaveAttack::Create(CGameObject* pOwner)
{
	CDiveDaveAttack* pState = new CDiveDaveAttack(pOwner);

	pState->Add_SubState();

	return pState;
}

void CDiveDaveAttack::Free()
{
	for_each(m_mapSubState.begin(), m_mapSubState.end(), CDeleteMap());
	m_mapSubState.clear();
}
