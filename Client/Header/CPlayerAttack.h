#pragma once
#include "CPlayerState.h"
class CPlayerAttack : public CPlayerState
{
private:
	explicit CPlayerAttack(CDSPlayer* pPlayer);
	virtual ~CPlayerAttack();

public:
	void Enter() override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;

private:
	virtual void Clear() override;

private:
	_float m_fAttackTime; //공격 상태 진입 후 흐른 총 시간
	_float m_fMaxAttakcTime; // 다른 상태로 넘어가야 할 시간
public:
	static CPlayerAttack* Create(CDSPlayer* pPlayer);
private:
	void Free() override;
};

