#pragma once
#include "CBaseState.h"

class CJohn;

class CJohnAttackShoot :
    public CBaseState<CJohn>
{
private:
    explicit CJohnAttackShoot(CJohn* pOwner);
    virtual ~CJohnAttackShoot();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CJohnAttackShoot* Create(CJohn* pOwner);
private:
    virtual void Free() override;
};

