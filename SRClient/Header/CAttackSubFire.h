#pragma once
#include "CAttackSubState.h"
class CAttackSubFire : public CAttackSubState
{
private:
    explicit CAttackSubFire(CGameObject* pPlayer, CDiveDaveAttack* pParentState);
    virtual ~CAttackSubFire();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CAttackSubFire* Create(CGameObject* pPlayer, CDiveDaveAttack* pParentState);

private:
    virtual void Free() override;
};

