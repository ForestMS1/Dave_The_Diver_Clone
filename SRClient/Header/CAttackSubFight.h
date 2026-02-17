#pragma once
#include "CAttackSubState.h"
class CAttackSubFight :
    public CAttackSubState
{
private:
    explicit CAttackSubFight(CGameObject* pPlayer, CDiveDaveAttack* pParentState);
    virtual ~CAttackSubFight();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

public:
    static CAttackSubFight* Create(CGameObject* pPlayer, CDiveDaveAttack* pParentState);

private:
    _float m_fFailDelay = 0.f;

private:
    virtual void Free() override;
};

