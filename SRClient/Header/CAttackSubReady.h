#pragma once
#include "CAttackSubState.h"
class CAttackSubReady :
    public CAttackSubState
{
private:
    explicit CAttackSubReady(CGameObject* pPlayer, CDiveDaveAttack* pParentState);
    virtual ~CAttackSubReady();

public:
    void Enter() override;
    void Input(const _float& fTimeDelta) override;
    _int Update_State(const _float& fTimeDelta) override;
    void LateUpdate_State(const _float& fTimeDelta) override;
    void Render_State() override;
    void Exit() override;
    void Clear() override;

private:
    void Mouse_Check();
public:
    static CAttackSubReady* Create(CGameObject* pPlayer, CDiveDaveAttack* pParentState);

private:
    virtual void Free() override;
};

