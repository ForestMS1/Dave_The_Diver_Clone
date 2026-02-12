#pragma once
#include "CPlayerState.h"
class CDiveDaveMove : public CPlayerState
{
private:
	explicit CDiveDaveMove(CGameObject* pOwner);
	~CDiveDaveMove();

public:
	void Enter() override;
	void Input() override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;

public:
	static CDiveDaveMove* Create(CGameObject* pOwner);
private:
	virtual void Free() override;
};

