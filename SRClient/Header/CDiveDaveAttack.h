#pragma once
#include "CPlayerState.h"
#include "CAttackSubState.h"

//enum class ATTACKSUBSTATE
//{
//	ATTACK_READY,
//	ATTACK_FIRE,
//	ATTACK_FIGHT,
//	ATTACK_FAIL,
//	SUB_END
//};

class CDiveDaveAttack : public CPlayerState
{
private:
	explicit CDiveDaveAttack(CGameObject* pOwner);
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
	ATTACKSUBSTATE		Get_State() const { return m_eCurSubState; }
	void			Set_State(ATTACKSUBSTATE state);

private:
	void Add_SubState();

private:
	CAttackSubState* m_pSubState = nullptr;
	ATTACKSUBSTATE m_eCurSubState;
	unordered_map<ATTACKSUBSTATE, CAttackSubState*> m_mapSubState;

public:
	static CDiveDaveAttack* Create(CGameObject* pOwner);
private:
	virtual void Free() override;
};

