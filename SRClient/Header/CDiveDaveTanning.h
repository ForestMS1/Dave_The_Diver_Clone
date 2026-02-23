#pragma once
#include "CBaseState.h"

class CDiveDave;

class CDiveDaveTanning : public CBaseState<CDiveDave>
{
private:
	explicit CDiveDaveTanning(CDiveDave* pOwner);
	~CDiveDaveTanning();

public:
	void Enter() override;
	void Input(const _float& fTimeDelta) override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;

private:
	_float m_fTanningTime = 0.f;

public:
	static CDiveDaveTanning* Create(CDiveDave* pOwner);
private:
	virtual void Free() override;
};

