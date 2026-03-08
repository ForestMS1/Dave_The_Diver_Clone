#include "pch.h"
#include "CDiveDaveAttack.h"
#include "CDInputMgr.h"
#include "CHelper.h"
#include "CDiveDave.h"
#include "CCameraMgr.h"
#include "CDiveDaveCam.h"
#include "CAttackSubReady.h"
#include "CAttackSubFire.h"
#include "CAttackSubFail.h"
#include "CAttackSubFight.h"

string debugSubState[(_uint)ATTACKSUBSTATE::SUB_END] = { "ATTACK_READY", "ATTACK_FIRE", "ATTACK_FIGHT", "ATTACK_FAIL" };


CDiveDaveAttack::CDiveDaveAttack(CDiveDave* pOwner)
	:CBaseState<CDiveDave>(pOwner)
{
}

CDiveDaveAttack::~CDiveDaveAttack()
{
}

void CDiveDaveAttack::Enter()
{
	//m_pFSM->Get_State() = ATTACKSUBSTATE::ATTACK_READY;
	//m_pSubState->Set_State(ATTACKSUBSTATE::ATTACK_READY);
	//m_pFSM->Get_pState()->Enter();

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
#ifdef _DEBUG
	ImGui::Begin("DiveDave Info");
	string state = "AttackSubState : " + debugSubState[(_uint)m_eCurSubState];
	ImGui::Text(state.c_str());
	ImGui::End();
#endif
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


HRESULT CDiveDaveAttack::Add_SubState()
{
	//m_pFSM = CFSM<CDiveDaveAttack, ATTACKSUBSTATE>::Create(this);
	//if (m_pFSM == nullptr)
	//	return E_FAIL;
	//
	//m_pFSM->Add_State<CAttackSubReady>(ATTACKSUBSTATE::ATTACK_READY);
	//m_pFSM->Add_State<CAttackSubFire>(ATTACKSUBSTATE::ATTACK_FIRE);
	//m_pFSM->Add_State<CAttackSubFight>(ATTACKSUBSTATE::ATTACK_FIGHT);
	//m_pFSM->Add_State<CAttackSubFail>(ATTACKSUBSTATE::ATTACK_FAIL);

	m_mapSubState.insert({ ATTACKSUBSTATE::ATTACK_READY, CAttackSubReady::Create(this) });
	m_mapSubState.insert({ ATTACKSUBSTATE::ATTACK_FIRE, CAttackSubFire::Create(this) });
	m_mapSubState.insert({ ATTACKSUBSTATE::ATTACK_FIGHT,  CAttackSubFight::Create(this) });
	m_mapSubState.insert({ ATTACKSUBSTATE::ATTACK_FAIL, CAttackSubFail::Create(this) });

	return S_OK;
}

CDiveDaveAttack* CDiveDaveAttack::Create(CDiveDave* pOwner)
{
	CDiveDaveAttack* pState = new CDiveDaveAttack(pOwner);

	if (FAILED(pState->Add_SubState()))
	{
		Safe_Release(pState);
		MSG_BOX("CDiveDaveAttack Create Failed");
		return nullptr;
	}

	return pState;
}

void CDiveDaveAttack::Free()
{
	for_each(m_mapSubState.begin(), m_mapSubState.end(), CDeleteMap());
	m_mapSubState.clear();
	//Safe_Release(m_pFSM);
}
