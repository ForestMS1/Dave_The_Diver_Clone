#pragma once
#include "CBaseState.h"

class CJohn;
class CJohnMeleeAttackReady :
    public CBaseState<CJohn>
{
private:
    explicit CJohnMeleeAttackReady(CJohn* pOwner);
    virtual ~CJohnMeleeAttackReady();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CJohnMeleeAttackReady* Create(CJohn* pOwner);

private:
    _float m_fReadyTime = 0.f;
private:
    virtual void Free() override;

};

