#pragma once
#include "CBaseState.h"
class CJohn;

class CJohnHit :
    public CBaseState<CJohn>
{
private:
	explicit CJohnHit(CJohn* pOwner);
	~CJohnHit();

public:
	void Enter() override;
	void Input(const _float& fTimeDelta) override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;

public:
	static CJohnHit* Create(CJohn* pOwner);
private:
	virtual void Free() override;
};

