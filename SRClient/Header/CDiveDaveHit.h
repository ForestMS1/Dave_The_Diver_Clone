#pragma once
#include "CBaseState.h"

class CDiveDave;


class CDiveDaveHit :
    public CBaseState<CDiveDave>
{
private:
	explicit CDiveDaveHit(CDiveDave* pOwner);
	~CDiveDaveHit();

public:
	void Enter() override;
	void Input(const _float& fTimeDelta) override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;

public:
	static CDiveDaveHit* Create(CDiveDave* pOwner);
private:
	virtual void Free() override;
};

