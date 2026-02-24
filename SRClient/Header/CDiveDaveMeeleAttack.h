#pragma once
#include "CBaseState.h"

class CDiveDave;

class CDiveDaveMeeleAttack :
    public CBaseState<CDiveDave>
{
private:
	explicit CDiveDaveMeeleAttack(CDiveDave* pOwner);
	~CDiveDaveMeeleAttack();

public:
	void Enter() override;
	void Input(const _float& fTimeDelta) override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;

private:
	void Mouse_Check();

public:
	static CDiveDaveMeeleAttack* Create(CDiveDave* pOwner);
private:
	virtual void Free() override;
};

