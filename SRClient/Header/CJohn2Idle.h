#pragma once
#include "CBaseState.h"
class CJohn2;

class CJohn2Idle :
    public CBaseState<CJohn2>
{
private:
    explicit CJohn2Idle(CJohn2* pOwner);
    virtual ~CJohn2Idle();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CJohn2Idle* Create(CJohn2* pOwner);

private:
    _float m_fBreakTime = 0.f;
    _uint  m_iRand = 0;
private:
    virtual void Free() override;
};

