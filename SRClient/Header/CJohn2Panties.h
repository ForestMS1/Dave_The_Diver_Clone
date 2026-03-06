#pragma once
#include "CBaseState.h"

class CJohn2;

class CJohn2Panties :
    public CBaseState<CJohn2>
{
private:
	explicit CJohn2Panties(CJohn2* pOwner);
	~CJohn2Panties();

public:
	void Enter() override;
	void Input(const _float& fTimeDelta) override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;

public:
	static CJohn2Panties* Create(CJohn2* pOwner);
private:
	virtual void Free() override;
};

