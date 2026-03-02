#pragma once
#include "CBaseState.h"

class CJohn2;

class CJohn2AttackReady :
    public CBaseState<CJohn2>
{
private:
    explicit CJohn2AttackReady(CJohn2* pOwner);
    virtual ~CJohn2AttackReady();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CJohn2AttackReady* Create(CJohn2* pOwner);

private:
    _float m_fReadyTime = 0.f;
private:
    virtual void Free() override;
};

