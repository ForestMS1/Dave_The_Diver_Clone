#pragma once
#include "CPlayerState.h"
class CDiveDaveOpen : public CPlayerState
{
private:
	explicit CDiveDaveOpen(CGameObject* pOwner);
	~CDiveDaveOpen();

public:
	void Enter() override;
	void Input(const _float& fTimeDelta) override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;


private:
	_float m_fOpenTime = 0.f;

public:
	static CDiveDaveOpen* Create(CGameObject* pOwner);
private:
	virtual void Free() override;
};

