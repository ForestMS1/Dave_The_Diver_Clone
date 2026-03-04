#pragma once
#include "CBaseState.h"

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

class CDiveDave;

class CDiveDaveMove : public CBaseState<CDiveDave>
{
private:
	explicit CDiveDaveMove(CDiveDave* pOwner);
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
	void Restore_Fov(const _float& fTimeDelta);
private:
	DIR m_eDir = DIR_END;

	_float m_fMoveSpeed = 2.f;
	_vec3 m_vMoveDir;
public:
	static CDiveDaveMove* Create(CDiveDave* pOwner);
private:
	virtual void Free() override;
};

