#pragma once
#include "CBaseState.h"
class CDiveDave;

class CDiveDaveInitStart :
    public CBaseState<CDiveDave>
{
private:
	explicit CDiveDaveInitStart(CDiveDave* pOwner);
	~CDiveDaveInitStart();

public:
	void Enter() override;
	void Input(const _float& fTimeDelta) override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;
private:
	_float m_fInitReady = 0.f;
	_float m_fInitTime = 0.00001f;
	_vec3 m_vOriginPos;

	_bool m_bDivingSound = false;

public:
	static CDiveDaveInitStart* Create(CDiveDave* pOwner);
private:
	virtual void Free() override;
};

