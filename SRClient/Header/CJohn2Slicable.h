#pragma once
#include "CBaseState.h"

class CJohn2;

class CJohn2Slicable :
    public CBaseState<CJohn2>
{
private:
	explicit CJohn2Slicable(CJohn2* pOwner);
	~CJohn2Slicable();

public:
	void Enter() override;
	void Input(const _float& fTimeDelta) override;
	_int Update_State(const _float& fTimeDelta) override;
	void LateUpdate_State(const _float& fTimeDelta) override;
	void Render_State() override;
	void Exit() override;
	void Clear() override;

private:
	_vec3 m_vRotDir;
	tweeny::tween<float> m_tween;
	float m_fAlpha;
public:
	static CJohn2Slicable* Create(CJohn2* pOwner);
private:
	virtual void Free() override;
};

