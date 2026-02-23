#pragma once
#include "CBaseState.h"
#include "CAttackSubState.h"
#include "CFSM.h"

class CDiveDave;


//enum class ATTACKSUBSTATE
//{
//	ATTACK_READY,
//	ATTACK_FIRE,
//	ATTACK_FIGHT,
//	ATTACK_FAIL,
//	SUB_END
//};

class CDiveDaveAttack : public CBaseState<CDiveDave>
{
private:
	explicit CDiveDaveAttack(CDiveDave* pOwner);
	~CDiveDaveAttack();

public:
	void Enter() override;
	void Input(const _float& fTimeDelta) override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;

public:
	ATTACKSUBSTATE		Get_State() const				{ return m_eCurSubState; }
	void				Set_State(ATTACKSUBSTATE state);
	CDiveDave*			Get_OwnerDave()					{ return m_pOwner; } //DiveDave

private:
	HRESULT				Add_SubState();

private:
	//CFSM<CDiveDaveAttack, ATTACKSUBSTATE>* m_pFSM = nullptr;
	CAttackSubState* m_pSubState = nullptr;
	ATTACKSUBSTATE m_eCurSubState;
	unordered_map<ATTACKSUBSTATE, CAttackSubState*> m_mapSubState;

public:
	static CDiveDaveAttack* Create(CDiveDave* pOwner);
private:
	virtual void Free() override;
};

