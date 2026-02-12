#pragma once
#include "CPlayerState.h"

enum DIR
{
	UP,
	UP_RIGHT,
	UP_LEFT,
	RIGHT,
	LEFT,
	DOWN,
	DOWN_RIGHT,
	DOWN_LEFT,
	DIR_END
};

class CDiveDaveMove : public CPlayerState
{
private:
	explicit CDiveDaveMove(CGameObject* pOwner);
	~CDiveDaveMove();

public:
	void Enter() override;
	void Input(const _float& fTimeDelta) override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;

private:
	void Go_Dir(const _float& fTimeDelta);
private:
	DIR m_eDir = DIR_END;

public:
	static CDiveDaveMove* Create(CGameObject* pOwner);
private:
	virtual void Free() override;
};

