#pragma once
#include "CAttackSubState.h"

class CAttackSubFail : public CAttackSubState
{
private:
    explicit CAttackSubFail(CDiveDaveAttack* pParentState);
    virtual ~CAttackSubFail();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CAttackSubFail* Create(CDiveDaveAttack* pParentState);

private:
    _float m_fFailDelay = 0.f;
private:
    virtual void Free() override;
};

