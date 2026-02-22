#pragma once
#include "CPlayerState.h"
class CDiveDaveTanning : public CPlayerState
{
private:
	explicit CDiveDaveTanning(CGameObject* pOwner);
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
	static CDiveDaveTanning* Create(CGameObject* pOwner);
private:
	virtual void Free() override;
};

