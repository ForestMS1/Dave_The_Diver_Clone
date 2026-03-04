#pragma once
#include "CBaseState.h"
class CDiveDave;

class CDiveDaveSubMarine :
    public CBaseState<CDiveDave>
{

private:
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

private:
	explicit CDiveDaveSubMarine(CDiveDave* pOwner);
	~CDiveDaveSubMarine();

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
	_vec3 m_vLastSubMarineDir = { 0.f, 0.f, 0.f, };
	wstring_view m_wsTexName;

	_float m_fMoveSpeed = 0.f;
	_bool m_bCanKeyInput = true;

public:
	static CDiveDaveSubMarine* Create(CDiveDave* pOwner);
private:
	virtual void Free() override;
};

