#pragma once
#include "CBaseState.h"
class CJohn;

class CJohnDie :
    public CBaseState<CJohn>
{
private:
	explicit CJohnDie(CJohn* pOwner);
	~CJohnDie();

public:
	void Enter() override;
	void Input(const _float& fTimeDelta) override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;

public:
	static CJohnDie* Create(CJohn* pOwner);

	_float m_fAccMoveDist = 0.f;
	_vec3 m_vAt;

	_bool m_bChangeMotion = false;

	wstring_view m_wsTexName;
private:
	virtual void Free() override;
};

